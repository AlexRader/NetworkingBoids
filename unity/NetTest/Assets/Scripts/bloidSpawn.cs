using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
//using System;

//this struct is basically the same as the pugin so info can be transfered
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
struct BloidMessage
{
    byte typeID;

    public int objectId;
    public float x, y, z;
    public int direction;

};
//script is used for handling a data push server
public class bloidSpawn : myDataStructs
{
    [Tooltip("public reference to spawned boid or bloid in this case")]
    public GameObject boid;
    [Tooltip("modifiable ip address so you can use this on diff computers")]
    public string ipAddress;

    [Tooltip("list of the boids spawned here from server")]
    public List<GameObject> bloidList1;

    [Tooltip("set true when boid was found")]
    public bool found;
    [Tooltip("used to signal initial game state is loaded")]
    public bool allowUpdates;

	// Use this for initialization
	void Start ()
    {
        found = false;
        allowUpdates = false;

        raknetPeer(); // initialize the connection
        connectToServer(ipAddress); // connect to specified server
        StartCoroutine("InitialLoad"); // this is where the server info will be loaded
        found = false;
    }

    // Update is called once per frame
    void Update ()
    {
        //just gets the update from the server. and updates boids
        if (allowUpdates)
        {
            BloidData newData = receiveData();
            if (newData.objectId >= 0)
            {
                for (int i = 0; i < bloidList1.Count && !found; i++)
                {
                    // find the object and update it
                    if (bloidList1[i].GetComponent<BoidBehavior>().objId == newData.objectId)
                    {
                        bloidList1[i].GetComponent<BoidBehavior>().SendMessage("setPos", new Vector3(newData.x, newData.y, newData.z));
                        found = true;
                    }
                }
                found = false;
            }
        }
    }
    //loads sent server data.
    IEnumerator InitialLoad()
    {
        yield return new WaitForSeconds(.1f); // this is so we dont have data overload
        BloidData newData = InitialData();
        if (newData.objectId >= 0)
        {
            GameObject dorkus = Instantiate(boid, new Vector3(newData.x, newData.y, newData.z), Quaternion.identity);
            dorkus.GetComponent<BoidBehavior>().objId = newData.objectId;
            bloidList1.Add(dorkus);
            StartCoroutine("InitialLoad");
        }
        else if (newData.objectId == -1)
            allowUpdates = true;
        else
            StartCoroutine("InitialLoad");
    }
    
}

