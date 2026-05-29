using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FlyMonster : Enemy

{
    public float speed;
    private GameObject player;

    public float dieTime;
    private Animator ani;
    private bool wasExecuted = false;
    public bool chase = false;
    public Transform startingPoint;



    public void Start()
    {
        base.Start();
        ani = GetComponent<Animator>();
        player = GameObject.FindGameObjectWithTag("Player");
    }

    // Update is called once per frame
    public void Update()
    {
        if (chase == true)
        {
            Chase();
        } else
        {
            ReturnStartPoint();
        }

        Dealth();
        Flip();
    }

    private void Chase()
    {
        transform.position = Vector2.MoveTowards(transform.position, player.transform.position, speed * Time.deltaTime);
    }

    private void ReturnStartPoint()
    {
        transform.position = Vector2.MoveTowards(transform.position, startingPoint.position, speed * Time.deltaTime);
    }

    private void Flip()
    {
        if (transform.position.x > player.transform.position.x)
        {
            transform.rotation = Quaternion.Euler(0, 0, 0);
        } else
        {
            transform.rotation = Quaternion.Euler(0, 180, 0);
        }
    }


    public void Dealth()
    {
        if (health <= 0 && wasExecuted == false)
        {
            ani.SetTrigger("Die");
            Invoke("Kill", dieTime);
            wasExecuted = true;
        }
    }

    void Kill()
    {
        Destroy(gameObject);
    }
    private void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.tag == "Player")
        {
            
                if (playerHealth != null) {
                    playerHealth.DamgePlayer(damage);
                }
        }
    }
}

