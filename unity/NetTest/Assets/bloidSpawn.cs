using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class bloidSpawn : MonoBehaviour {

    public GameObject boid;

    public float timer;
    public float maxTime;

	// Use this for initialization
	void Start () {
        timer = maxTime;

    }

    // Update is called once per frame
    void Update () {

        timer -= Time.deltaTime;

        if (timer <= 0)
        {
            GameObject dorkus = Instantiate(boid, Vector3.zero, Quaternion.identity);

            //dorkus.direction = //getdirectionfromrakpeer;

            //reset timer
            timer = maxTime;
        }
	}
}
