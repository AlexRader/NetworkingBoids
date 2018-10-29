using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BoidBehavior : myDataStructs {
    //disclaimer, not really boids
    private const float INC = 0.001f;
    //public int directionInt; 

    public int direction;//0 up, 1 down, 2 left, 3 right, 4 forward, 5 back
    //public Vector3 directionEnd; //direction * magnitude for distance to lerp to

    public int objId;

    //public float lerpTimer;
    //public float maxLerpTimer;
    //public float lerpValue;
    public float moveInc;

    //Rigidbody rb;

	// Use this for initialization
	void Start () {
        direction = 0;
        //rb = GetComponent<Rigidbody>();
        //maxLerpTimer = 5;
        moveInc = 5;
	}
	

    void setPos(Vector3 sentData)
    {
        transform.position = new Vector3(sentData.x, sentData.y, sentData.z);
        //rb.velocity = new Vector3(sentData.x, sentData.y, sentData.z).normalized * 2;
    }

    void simulatePos()
    {
       if (direction < 2 && direction >= 0)
       {
           if (0 == direction % 2)
               transform.position = new Vector3(transform.position.x, transform.position.y + (moveInc * Time.deltaTime), transform.position.z);
           else
               transform.position = new Vector3(transform.position.x, transform.position.y - (moveInc * Time.deltaTime), transform.position.z);
       }
       else if (direction < 4)
       {
           if (0 == direction % 2)
               transform.position = new Vector3(transform.position.x + (moveInc * Time.deltaTime), transform.position.y, transform.position.z);
           else
               transform.position = new Vector3(transform.position.x - (moveInc * Time.deltaTime), transform.position.y, transform.position.z);
       }
       else
       {
           if (0 == direction % 2)
               transform.position = new Vector3(transform.position.x, transform.position.y, transform.position.z + (moveInc * Time.deltaTime));
           else
               transform.position = new Vector3(transform.position.x, transform.position.y, transform.position.z - (moveInc * Time.deltaTime));
       }
    }

    void SetDirection()
    {
        direction = Random.Range(0,6);
    }

}
