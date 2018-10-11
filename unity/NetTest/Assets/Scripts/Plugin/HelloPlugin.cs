using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//need
using System.Runtime.InteropServices;
public class HelloPlugin : MonoBehaviour
{
    [DllImport("egp-net-plugin-Unity x64")]
    static extern int startup();

    [DllImport("egp-net-plugin-Unity x64")]
    static extern int shutdown();

    [DllImport("egp-net-plugin-Unity x64")]
    static extern int foo(int bar);
	// Use this for initialization
	void Start ()
    {
		
	}
    int counter = 0;
	// Update is called once per frame
	void Update ()
    {
        Debug.Log("counter = " + counter.ToString() + ", foo = " + foo(counter++).ToString());
	}
}
