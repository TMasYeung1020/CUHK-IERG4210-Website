using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Saw : MonoBehaviour
{
    public float speed;
    private int damage = 30;
    public Transform startingPoint;
    public Transform pos1, pos2;

    Vector3 nextPos;

    void Start()
    {
        nextPos = startingPoint.position;
    }

    void Update()
    {
        if (transform.position == pos1.position)
        {
            nextPos = pos2.position;
        }
        if (transform.position == pos2.position)
        {
            nextPos = pos1.position;
        }

        transform.position = Vector3.MoveTowards(transform.position, nextPos, speed *Time.deltaTime);
    }

    private void OnTriggerEnter2D(Collider2D other)
    {
        if(other.tag == "Player")
        {
            other.GetComponent<PlayerHealth>().DamgePlayer(damage);
        }
    }
}
