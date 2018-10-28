﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;


public class bloidShareSpawn : myDataStructs
{
    public GameObject boid;

    public string ipAddress;
    public char[] ipAddressChar;
   
    public GameObject[] bloidList;
    public List<GameObject> bloidList1; // all objects
    public List<GameObject> bloidList2; // my objects to update locally 
    public bool found;
    public bool allowUpdates;
    public Material localObject;

	// Use this for initialization
	void Start ()
    {
        found = false;
        allowUpdates = false;
        //BloidData var = Test();
        //
        //Debug.Log(var.objectId);
        //Debug.Log(var.direction);
        //Debug.Log(var.x);
        //Debug.Log(var.y);
        //Debug.Log(var.z);

        raknetPeer();
        connectToServer(ipAddress);
        StartCoroutine("InitialLoad");
        found = false;
    }

    // Update is called once per frame
    void Update ()
    {
        if (allowUpdates)
        {
            for (int i = 0; i < bloidList2.Count; ++i)
            {
                bloidList2[i].GetComponent<BoidBehavior>().SendMessage("simulatePos");
            }
            BloidData newData = receiveData();
            if (newData.objectId >= 0)
            {
                for (int i = 0; i < bloidList1.Count && !found; i++)
                {
                    if (bloidList1[i].GetComponent<BoidBehavior>().objId == newData.objectId)
                    {
                        bloidList1[i].GetComponent<BoidBehavior>().SendMessage("setPos", new Vector3(newData.x, newData.y, newData.z));
                        bloidList1[i].GetComponent<BoidBehavior>().direction = newData.direction;
                        found = true;
                    }
                }
                if (!found)
                {
                    GameObject dorkus = Instantiate(boid, new Vector3(newData.x, newData.y, newData.z), Quaternion.identity);
                    dorkus.GetComponent<BoidBehavior>().objId = newData.objectId;
                    bloidList1.Add(dorkus);
                }
                found = false;
            }
        }
    }

    IEnumerator InitialLoad()
    {
        yield return new WaitForSeconds(.1f);
        BloidData newData = InitialData();
        if (newData.objectId >= 0)
        {
            GameObject dorkus = Instantiate(boid, new Vector3(newData.x, newData.y, newData.z), Quaternion.identity);
            dorkus.GetComponent<BoidBehavior>().objId = newData.objectId;
            bloidList1.Add(dorkus);
            StartCoroutine("InitialLoad");
        }
        else if (newData.objectId == -1)
            StartCoroutine("MyObjects");
        else
            StartCoroutine("InitialLoad");
    }

    IEnumerator MyObjects()
    {
        yield return new WaitForSeconds(.1f);
        BloidData newData = InitialData();
        if (newData.objectId >= 0)
        {
            GameObject dorkus = Instantiate(boid, new Vector3(newData.x, newData.y, newData.z), Quaternion.identity);
            dorkus.GetComponent<BoidBehavior>().objId = newData.objectId;
            dorkus.GetComponent<Renderer>().material.color = Color.red;
            bloidList1.Add(dorkus);
            bloidList2.Add(dorkus);
            StartCoroutine("MyObjects");
        }
        else if (newData.objectId == -1)
            allowUpdates = true;
        else
            StartCoroutine("MyObjects");
    }
    
}
