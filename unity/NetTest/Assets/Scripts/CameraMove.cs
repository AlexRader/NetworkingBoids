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

//just here to make the camera have the ability to move yw Dan, also enjoy the backgroundf color
public class CameraMove : MonoBehaviour
{
    public const float MOVE_SPEED = 20.0f;
    public const float ROTATE_SPEED = 100.0f;
    [Header("R resets rotation")]

    [Tooltip("w/s forward,back")]
    public float forward;
    [Tooltip("a/d left,right")]
    public float yRot;
    [Tooltip("h/j up,down")]
    public float zRot;
	
	// Update is called once per frame
	void Update ()
    {
        Inputs();
        transform.position += transform.forward * forward * Time.deltaTime * MOVE_SPEED;
        transform.Rotate(0, yRot * Time.deltaTime * ROTATE_SPEED, 0);
        transform.Rotate(zRot * Time.deltaTime * ROTATE_SPEED, 0, 0);

    }

    void Inputs()
    {
        forward = Input.GetAxis("Vertical");
        yRot = Input.GetAxis("Horizontal");
        zRot = Input.GetAxis("Strafe");
        if (Input.GetKeyDown(KeyCode.R))
            transform.rotation = Quaternion.identity;
    }

}
