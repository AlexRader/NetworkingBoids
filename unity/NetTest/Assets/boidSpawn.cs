using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class boidSpawn : MonoBehaviour {

    public GameObject boid;

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        if (Input.GetKeyDown(KeyCode.Space))
        {
            Instantiate(boid, Vector3.zero, Quaternion.identity);
        }
	}
}
