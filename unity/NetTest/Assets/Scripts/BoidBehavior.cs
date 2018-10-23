using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BoidBehavior : MonoBehaviour {

    public Vector2 position;
    public Vector2 velocity;
    public Vector2 acceleration;

    public float moveSpeed;

    public float rotation; //x rotation specifically

	// Use this for initialization
	void Start () {
        this.transform.position = position;
	}
	
	// Update is called once per frame
	void Update () {

        //get stats from server
        ///????

        //transform based off velocity
        position += velocity;

        //apply transformation to transform
        this.transform.position = position;



        //reset
        ///position = Vector2.zero;
        velocity = Vector2.zero;

	}

}
