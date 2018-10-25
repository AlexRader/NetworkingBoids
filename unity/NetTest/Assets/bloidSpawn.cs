using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using System;

//struct BloidMessage
//{
//    char typeID;

//    int objectId;
//    float x, y, z;
//    int direction;

//};

[StructLayout(LayoutKind.Sequential, Pack = 1)]

struct BloidMessage
{
    byte typeID;

    public int objectId;
    public float x, y, z;
    public int direction;

};


struct BloidData
{
    public int objectId;
    public float x, y, z;
    public int direction;

};

public class bloidSpawn : MonoBehaviour {



    [DllImport("BoidEvents")]
    static extern void connectToServer([MarshalAs(UnmanagedType.LPStr)]string ip);

    [DllImport("BoidEvents")]
    static extern unsafe BloidData receiveData();

    [DllImport("BoidEvents")]
    static extern void sendData(int id, float x, float y, float z, int dir);


    public GameObject boid;

    public float timer;
    public float maxTime = 1.5f;

    public string ipAddress = "216.93.149.128";
    public char[] ipAddressChar;
   
    public GameObject[] bloidList;
    public bool found;

	// Use this for initialization
	void Start () {
        timer = maxTime;

        connectToServer(ipAddress);


    }

    // Update is called once per frame
    void Update ()
    {

        timer -= Time.deltaTime;



        if (timer <= 0)
        {

            BloidData newData = receiveData();

                    Debug.Log("id " + newData.direction +
                        "direction " + newData.direction);
             

            //sorry dan but yes im calling this in update, I know its expensive, invoice me later
            bloidList = GameObject.FindGameObjectsWithTag("BLOID");

            found = false;

            for (int i = 0; i < bloidList.Length; i++)
            {
                if (bloidList[i].GetComponent<BoidBehavior>().objId == newData.objectId)
                {
                    bloidList[i].GetComponent<BoidBehavior>().changeDirection(newData.direction);
                    found = true;
                }
            }

            if (!found)
            {
                GameObject dorkus = Instantiate(boid, Vector3.zero, Quaternion.identity);
                dorkus.GetComponent<BoidBehavior>().objId = newData.objectId;
                dorkus.GetComponent<BoidBehavior>().changeDirection(newData.direction);

            }

            timer = maxTime;

        }

    }
}
