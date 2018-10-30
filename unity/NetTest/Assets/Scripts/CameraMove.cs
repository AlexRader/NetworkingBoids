using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//just here to make the camera have the ability to move
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
