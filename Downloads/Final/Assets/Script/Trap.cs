using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Trap : MonoBehaviour
{
    public Transform trap;
    public float speed;
    public Transform pos;
    Vector3 nextPos;
    private bool start = false;

    void Start()
    {

    }
    void Update()
    {
        if (start == true)
        {
            if (trap.position != pos.position)
            {
                nextPos = pos.position;
                trap.position = Vector3.MoveTowards(trap.position, nextPos,speed *Time.deltaTime);
            }
        }
    }

    void OnTriggerEnter2D(Collider2D other)
    {
        if(other.CompareTag("Player") && other.GetType().ToString() == "UnityEngine.PolygonCollider2D")
        {
            start = true;
        }
    }


}
