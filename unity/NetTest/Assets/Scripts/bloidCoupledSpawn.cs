using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;


public class bloidCoupledSpawn : myDataStructs
{
    public float MAX_TIME = 3.0f;
    [Tooltip("public reference to spawned boid or bloid in this case")]
    public GameObject boid;

    [Tooltip("modifiable ip address so you can use this on diff computers")]
    public string ipAddress;

    [Tooltip("list of the boids spawned here from server")]
    public List<GameObject> bloidList1;
    [Tooltip("list of the boids spawned here from server that client can control")]
    public List<GameObject> bloidList2; // my objects to update locally 

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
        connectToServer(ipAddress); // connect to specified server
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
            for (int i = 0; i < bloidList2.Count; ++i)
            {
                currentObj = bloidList2[i].GetComponent<BoidBehavior>();
                if (timer <= 0)
                    currentObj.SendMessage("SetDirection");
                currentObj.SendMessage("simulatePos");
            }
            BloidData newData = receiveData();
            //update all objects
            if (newData.objectId >= 0)
            {
                for (int i = 0; i < bloidList1.Count && !found; i++)
                {
                    // find the object and update it
                    if (bloidList1[i].GetComponent<BoidBehavior>().objId == newData.objectId)
                    {
                        bloidList1[i].GetComponent<BoidBehavior>().SendMessage("setPos", new Vector3(newData.x, newData.y, newData.z));
                        bloidList1[i].GetComponent<BoidBehavior>().direction = newData.direction;
                        found = true;
                    }
                } 
                // this was done for spawning events that never happened
                if (!found && newData.objectId <= 10000)
                {
                    GameObject dorkus = Instantiate(boid, new Vector3(newData.x, newData.y, newData.z), Quaternion.identity);
                    dorkus.GetComponent<BoidBehavior>().objId = newData.objectId;
                    bloidList1.Add(dorkus);
                }
                found = false;
            }
            if (timer <= 0)
                timer = MAX_TIME;
        }
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
            bloidList1.Add(dorkus);
            Debug.Log(x++);
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
            bloidList1.Add(dorkus);
            bloidList2.Add(dorkus);
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
        for (int i = 0; i < bloidList2.Count; ++i)
        {
            currentObj = bloidList2[i].GetComponent<BoidBehavior>();
            currObjTrans = bloidList2[i].transform.position;
            sendData(currentObj.objId, currObjTrans.x, currObjTrans.y, currObjTrans.z, currentObj.direction);
        }
        StartCoroutine("SendData"); // this is here so send data is called

    }

}

