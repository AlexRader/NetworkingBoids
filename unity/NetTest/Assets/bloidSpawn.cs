﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
//using System;

//struct BloidMessage
//{
//    char typeID;

//    int objectId;
//    float x, y, z;
//    int direction;

//};

//[StructLayout(LayoutKind.Sequential, Pack = 1)]

struct BloidMessage
{
    byte typeID;

    public int objectId;
    public float x, y, z;
    public int direction;

};


//struct BloidData
//{
//    public int objectId;
//    public float x, y, z;
//    public int direction;
//
//};

public class bloidSpawn : MonoBehaviour
{
    [StructLayout(LayoutKind.Sequential, Size = 20), System.Serializable]
    struct BloidData
    {
        [MarshalAsAttribute(UnmanagedType.I4, SizeConst = 4)]
        public int objectId;
        [MarshalAsAttribute(UnmanagedType.R4, SizeConst = 4)]
        public float x;
        [MarshalAsAttribute(UnmanagedType.R4, SizeConst = 4)]
        public float y;
        [MarshalAsAttribute(UnmanagedType.R4, SizeConst = 4)]
        public float z;
        [MarshalAsAttribute(UnmanagedType.I4, SizeConst = 4)]
        public int direction;

    };
    [DllImport("BoidEvents")]
    static extern void raknetPeer();

    [DllImport("BoidEvents")]
    static extern void connectToServer([MarshalAs(UnmanagedType.LPStr)]string ip);

    [DllImport("BoidEvents")]
    static extern BloidData receiveData();

    [DllImport("BoidEvents")]
    static extern void sendData(int id, float x, float y, float z, int dir);

    [DllImport("BoidEvents")]
    static extern BloidData Test();


    public GameObject boid;

    public float timer;
    public float maxTime = 1.5f;

    public string ipAddress = "216.93.149.120";
    public char[] ipAddressChar;
   
    public GameObject[] bloidList;
    public bool found;

	// Use this for initialization
	void Start ()
    {
        found = false;
        timer = maxTime;
        BloidData var = Test();

        Debug.Log(var.objectId);
        Debug.Log(var.direction);
        Debug.Log(var.x);
        Debug.Log(var.y);
        Debug.Log(var.z);

        raknetPeer();
        connectToServer(ipAddress);
    }

    // Update is called once per frame
    void Update ()
    {

        timer -= Time.deltaTime;
        //sorry dan but yes im calling this in update, I know its expensive, invoice me later
        bloidList = GameObject.FindGameObjectsWithTag("BLOID");

        BloidData newData = receiveData();
        Debug.Log(newData.objectId);


        if (newData.objectId >= 0)
        {
            
            for (int i = 0; i < bloidList.Length; i++)
            {
                if (bloidList[i].GetComponent<BoidBehavior>().objId == newData.objectId)
                {
                    found = true;
                }
            }
            if (!found)
            {
                //create new BLOID            
                GameObject dorkus = Instantiate(boid, Vector3.zero, Quaternion.identity);
                dorkus.GetComponent<BoidBehavior>().objId = newData.objectId;
                dorkus.GetComponent<BoidBehavior>().changeDirection(newData.direction);
                found = false;
            }
            else
            {

                //get
                for (int i = 0; i < bloidList.Length; i++)
                {
                    if (bloidList[i].GetComponent<BoidBehavior>().objId == newData.objectId)
                    {
                        bloidList[i].GetComponent<BoidBehavior>().changeDirection(newData.direction);
                    }
                }
            }
            
        }
    }
}
