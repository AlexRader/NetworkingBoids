using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;

public class BoidScript : MonoBehaviour {

    [DllImport("BoidEvents")]
    static extern int function();

    [DllImport("BoidEvents")]
    static extern int Serialize();

    [DllImport("BoidEvents")]
    static extern int Deserialize();


    // Use this for initialization
    void Start () {
        Debug.Log ("random value at " + function());
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
