using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BoidBehavior : MonoBehaviour {
    //disclaimer, not really boids

    public int directionInt; //0 north, 1 east, 2 south, 3 west

    public Vector3 direction;
    public Vector3 directionEnd; //direction * magnitude for distance to lerp to

    public int objId;

    public float lerpTimer;
    public float maxLerpTimer;
    public float lerpValue;

    struct BloidData
    {
        int objectId;
        float x, y, z;
        int direction;

    };

  //    BloidData lastData;

	// Use this for initialization
	void Start () {
        direction = Vector3.zero;

        maxLerpTimer = 5;
	}
	
	// Update is called once per frame
	void Update () {

        lerpTimer += Time.deltaTime;

        if (lerpTimer > maxLerpTimer)
        {
            lerpTimer = maxLerpTimer;
        }

        lerpValue = lerpTimer / maxLerpTimer;

        transform.position = Vector3.Lerp(transform.position, directionEnd, lerpValue);

    }


    public void changeDirection(int newDirection)
    {

        //get direction int from peer

        //directionInt = Random.Range(0, 6);
        directionInt = newDirection;


        switch (directionInt)
        {
            case 0:
                direction = Vector3.forward;
                break;
            case 1:
                direction = Vector3.right;
                break;
            case 2:
                direction = Vector3.back;
                break;
            case 3:
                direction = Vector3.left;
                break;
            case 4:
                direction = Vector3.up;
                break;
            case 5:
                direction = Vector3.down;
                break;

            default:
                direction = Vector3.zero;
                break;
        }

        directionEnd = transform.position + (direction * 5);

        lerpTimer = 0;
    }

}
