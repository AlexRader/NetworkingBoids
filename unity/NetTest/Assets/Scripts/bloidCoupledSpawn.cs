using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

/*
the following file was modified by
Alex Rader 1023256 Rosser Martinez 1020967
EGP-405-01/02 Project2 | due 10/29/2018
current date 10/3/2018

“We certify that this work is entirely our own. The assessor of this project
may reproduce this project and provide copies to other academic staff, and/or
communicate a copy of this project to a plagiarism-checking service, which may
retain a copy of the project on its database.”
*/
public class bloidCoupledSpawn : myDataStructs
{
    public float MAX_TIME = 3.0f;
    [Tooltip("public reference to spawned boid or bloid in this case")]
    public GameObject boid;

    [Tooltip("modifiable ip address so you can use this on diff computers")]
    public Text ipAddress;

    [Tooltip("list of the boids spawned here from server")]
    public List<GameObject> bloidListAll;
    [Tooltip("list of the boids spawned here from server that client can control")]
    public List<GameObject> bloidListLocal; // my objects to update locally 

    [Tooltip("set true when boid was found")]
    public bool found;
    [Tooltip("used to signal initial game state is loaded")]
    public bool allowUpdates;

    public float timer;
    int x = 0;
    BoidBehavior currentObj;
    Vector3 currObjTrans;
    // Use this for initialization
    void Start()
    {
        found = false;
        allowUpdates = false;

        raknetPeer();// initialize the connection
        connectToServer(ipAddress.text); // connect to specified server
        StartCoroutine("InitialLoad"); // this is where the server info will be loaded
        found = false;
        timer = MAX_TIME; // just for changing the direction of the object
    }

    // Update is called once per frame
    void Update()
    {
        if (allowUpdates)
        {
            //updates my objects
            timer -= Time.deltaTime;
            for (int i = 0; i < bloidListLocal.Count; ++i)
            {
                currentObj = bloidListLocal[i].GetComponent<BoidBehavior>();
                if (timer <= 0)
                    currentObj.SendMessage("SetDirection");
                currentObj.SendMessage("simulatePos");
            }
            BloidData newData = receiveData();

            //get timestamp from newdata and set relative times
            float currentTime = Time.deltaTime;
            float timeStamp = Time.deltaTime + newData.timeStamp;
            float timeStep = Time.deltaTime + (timeStamp * 2.0f);

            //tmp variables for loop iteration
            BoidBehavior tmp;
            Vector3 initialPos, timestepPos, /*timestepX2Pos,*/ average, velFinal;

            //update all objects
            if (newData.objectId >= 0)
            {
                for (int i = 0; i < bloidListAll.Count && !found; i++)
                {
                    tmp = bloidListAll[i].GetComponent<BoidBehavior>();

                    // find the object and update it
                    if (tmp.objId == newData.objectId)
                    {
                        //calculate position with dead reckoning
                        //at current time
                        initialPos = deadReckon(tmp.transform.position, tmp.velocity, tmp.acceleration, currentTime);
                        //at timestamp
                        timestepPos = deadReckon(tmp.transform.position, tmp.velocity, tmp.acceleration, timeStamp);
                        //at double the timestamp
                        //timestepX2Pos = deadReckon(tmp.transform.position, tmp.velocity, tmp.acceleration, timeStep);

                        //average vector of the three for the position
                        average.x = (initialPos.x + timestepPos.x) * .5f; //+ timestepX2Pos.x) / 3;
                        average.y = (initialPos.y + timestepPos.y) * .5f; //+ timestepX2Pos.y) / 3;
                        average.z = (initialPos.z + timestepPos.z) * .5f; //+ timestepX2Pos.z) / 3;

                        //set velocity of boid for future
                        velFinal = tmp.velocity + (tmp.acceleration * timeStamp);

                        //send position, direction, and final velocity to individual bloid, nice
                        tmp.velocity = velFinal;


                        tmp.SendMessage("setPos", new Vector3(newData.x, newData.y, newData.z));
                        tmp.direction = newData.direction;
                        found = true;
                    }
                } 
                // this was done for spawning events that never happened
                if (!found && newData.objectId <= 10000)
                {
                    GameObject dorkus = Instantiate(boid, new Vector3(newData.x, newData.y, newData.z), Quaternion.identity);
                    dorkus.GetComponent<BoidBehavior>().objId = newData.objectId;
                    bloidListAll.Add(dorkus);
                }
                found = false;
            }
            if (timer <= 0)
                timer = MAX_TIME;
        }
    }
    public Vector3 deadReckon(Vector3 cPos, Vector3 cVel, Vector3 cAcc, float time)
    {
        Vector3 updatedPosition;

        updatedPosition = cPos + (cVel * time) + (.5f * cAcc * (time * time));

        //Debug.Log(updatedPosition);

        return updatedPosition;
    }

    //loads sent server data for already spawned gameobjects.
    IEnumerator InitialLoad()
    {
        yield return new WaitForSeconds(.1f);
        BloidData newData = InitialData();
        if (newData.objectId >= 0)
        {
            GameObject dorkus = Instantiate(boid, new Vector3(newData.x, newData.y, newData.z), Quaternion.identity);
            dorkus.GetComponent<BoidBehavior>().objId = newData.objectId;
            bloidListAll.Add(dorkus);
            //Debug.Log(x++);
            StartCoroutine("InitialLoad");
        }
        else if (newData.objectId == -1)
            StartCoroutine("MyObjects");
        else
            StartCoroutine("InitialLoad");
    }

    // these are my gameobjects
    IEnumerator MyObjects()
    {
        yield return new WaitForSeconds(.1f);
        BloidData newData = InitialData();
        if (newData.objectId >= 0)
        {
            //spawn and track the object for updating
            GameObject dorkus = Instantiate(boid, new Vector3(newData.x, newData.y, newData.z), Quaternion.identity);
            dorkus.GetComponent<BoidBehavior>().SendMessage("SetDirection");
            dorkus.GetComponent<BoidBehavior>().objId = newData.objectId;
            dorkus.GetComponent<Renderer>().material.color = Color.red;
            bloidListAll.Add(dorkus);
            bloidListLocal.Add(dorkus);
            StartCoroutine("MyObjects");
        }
        else if (newData.objectId == -1)
        {
            allowUpdates = true;
            StartCoroutine("SendData");// this is here so send data is called
        }
        else
            StartCoroutine("MyObjects");
    }

    //used to send data
    IEnumerator SendData()
    {
        yield return new WaitForSeconds(.1f);
        for (int i = 0; i < bloidListLocal.Count; ++i)
        {
            currentObj = bloidListLocal[i].GetComponent<BoidBehavior>();
            currObjTrans = bloidListLocal[i].transform.position;
            sendData(currentObj.objId, Time.time, currObjTrans.x, currObjTrans.y, currObjTrans.z, currentObj.direction);
        }
        StartCoroutine("SendData"); // this is here so send data is called

    }

}

