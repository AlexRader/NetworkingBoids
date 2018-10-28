using System.Collections;
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

public class bloidSpawn : myDataStructs
{
    //[StructLayout(LayoutKind.Sequential, Size = 20), System.Serializable]
    //struct BloidData
    //{
    //    [MarshalAsAttribute(UnmanagedType.I4, SizeConst = 4)]
    //    public int objectId;
    //    [MarshalAsAttribute(UnmanagedType.R4, SizeConst = 4)]
    //    public float x;
    //    [MarshalAsAttribute(UnmanagedType.R4, SizeConst = 4)]
    //    public float y;
    //    [MarshalAsAttribute(UnmanagedType.R4, SizeConst = 4)]
    //    public float z;
    //    [MarshalAsAttribute(UnmanagedType.I4, SizeConst = 4)]
    //    public int direction;
    //
    //};
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

    [DllImport("BoidEvents")]
    static extern BloidData InitialData();

    public GameObject boid;

    public float timer;
    public float maxTime = 1.5f;

    public string ipAddress = "216.93.149.120";
    public char[] ipAddressChar;
   
    public GameObject[] bloidList;
    public List<GameObject> bloidList1;
    public bool found;
    public bool allowUpdates;

	// Use this for initialization
	void Start ()
    {
        found = false;
        timer = maxTime;
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
            BloidData newData = receiveData();
            if (newData.objectId >= 0)
            {
                for (int i = 0; i < bloidList1.Count && !found; i++)
                {
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
            allowUpdates = true;
        else
            StartCoroutine("InitialLoad");
    }
    
}

