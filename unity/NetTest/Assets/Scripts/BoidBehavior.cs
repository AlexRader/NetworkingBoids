using System.Collections;
using System.Collections.Generic;
using UnityEngine;
/*
the following file was modified by
Alex Rader 1023256 Rosser Martinez 1020967
EGP-405-01/02 Project2 | due 10/29/2018
current date 10/3/2018

“We certify that this work is entirely our own. The assessor of this project
may reproduce this project and provide copies to other academic staff, and/or
communicate a copy of this project to a plagiarism-checking service, which may
retain a copy of the project on its database.”
*/
public class BoidBehavior : myDataStructs {
    //disclaimer, not really boids
    private const float INC = 0.001f;
    //public int directionInt; 

    public int direction;//0 up, 1 down, 2 left, 3 right, 4 forward, 5 back

    public int objId;

    public float moveInc;

    public Vector3 velocity;
    public Vector3 acceleration;

    // Use this for initialization
    void Start () {
        direction = 0;
        moveInc = 5;

        velocity = Vector3.zero;
        acceleration = new Vector3(5, 5, 5);

    }
	

    void setPos(Vector3 sentData)
    {
        transform.position = new Vector3(sentData.x, sentData.y, sentData.z);
    }
    //updates bloids current position
    void simulatePos()
    {
        // y direction change
       if(direction < 2 && direction >= 0)
       {
            if (0 == direction % 2)
                transform.position = new Vector3(transform.position.x, transform.position.y + (acceleration.y * Time.deltaTime), transform.position.z);
            else
                transform.position = new Vector3(transform.position.x, transform.position.y - (acceleration.y * Time.deltaTime), transform.position.z);
        }
        // x direction change
       else if (direction < 4)
        {
            if (0 == direction % 2)
                transform.position = new Vector3(transform.position.x + (acceleration.x * Time.deltaTime), transform.position.y, transform.position.z);
            else
                transform.position = new Vector3(transform.position.x - (acceleration.x * Time.deltaTime), transform.position.y, transform.position.z);
        }
        // z direction change
        else
        {
            if (0 == direction % 2)
                transform.position = new Vector3(transform.position.x, transform.position.y, transform.position.z + (acceleration.z * Time.deltaTime));
            else
                transform.position = new Vector3(transform.position.x, transform.position.y, transform.position.z - (acceleration.z * Time.deltaTime));
        }
    }
    //sets direction of the current bloid
    void SetDirection()
    {
        direction = Random.Range(0,6);
    }

}
