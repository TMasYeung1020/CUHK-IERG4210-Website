using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Rat : Enemy
{
    public Transform [] patrolPoints;
    public float speed;
    public int patrolDestination;

    public float dieTime;
    private Animator ani;
    private bool wasExecuted = false;



    public void Start()
    {
        base.Start();
        ani = GetComponent<Animator>();
    }

    // Update is called once per frame
    public void Update()
    {
        if (patrolDestination == 0 && health > 0)
        {
            transform.position = Vector2.MoveTowards(transform.position, patrolPoints[0].position, speed * Time.deltaTime);
            if(Vector2.Distance(transform.position, patrolPoints[0].position) < .5f)
            {
                patrolDestination = 1;
                transform.localScale = new Vector3(-1, 1, 1);
            }
        }

        if (patrolDestination == 1 && health > 0)
        {
            transform.position = Vector2.MoveTowards(transform.position, patrolPoints[1].position, speed * Time.deltaTime);
            if(Vector2.Distance(transform.position, patrolPoints[1].position) < .5f)
            {
                patrolDestination = 0;
                transform.localScale = new Vector3(1, 1, 1);
            }
        }
        DealthRat();
    }

    public void DealthRat()
    {
        if (health <= 0 && wasExecuted == false)
        {
            ani.SetTrigger("Die");
            Invoke("KillRat", dieTime);
            wasExecuted = true;
        }
    }

    void KillRat()
    {
        Destroy(gameObject);
    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.tag == "Player")
        {
            if (playerMovement.ani.GetBool("falling"))
            {
                health = 0;
                playerMovement.ani.SetBool("jumping", true);
                playerMovement.rb.velocity = new Vector2(playerMovement.rb.velocity.x, playerMovement.jumpforce);
            } else 
            {
                playerMovement.KBCounter = playerMovement.KBTotalTime;
                if (playerHealth != null) {
                    playerHealth.DamgePlayer(damage);
                }
                if (collision.transform.position.x <= transform.position.x)
                {
                    playerMovement.knockFromRight = true;
                }
                if (collision.transform.position.x > transform.position.x)
                {
                    playerMovement.knockFromRight = false;
                }
            }
        }
    }
}
