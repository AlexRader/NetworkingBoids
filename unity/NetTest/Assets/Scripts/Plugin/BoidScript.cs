using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using System;

public class BoidScript : MonoBehaviour {

    struct BloidMessage
    {
        char typeID;

        int objectId;
        float x, y, z;
        int direction;

    };

    struct BloidData
    {
        int objectId;
        float x, y, z;
        int direction;

    };

    [DllImport("BoidEvents")]
    static extern int connect(IntPtr ip);

    [DllImport("BoidEvents")]
    static extern BloidData receiveData();

    [DllImport("BoidEvents")]
    static extern int sendData(int id, float x, float y, float z, int dir);



    // Use this for initialization
    void Start () {
      //  Debug.Log ("random value at " + function());
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
