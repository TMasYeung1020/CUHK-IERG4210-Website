using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Boss : MonoBehaviour {

    public int health;
    public int damage;
    private float timeBtwDamage = 1.5f;


    public Slider healthBar;
    private Animator anim;
    public bool isDead;

    public GameObject[] Star;
    private bool canFall = true;
    public float StarSpeed;
    public int i = 0;
    public GameObject door;

    private void Start()
    {
        anim = GetComponent<Animator>();
    }

    private void Update()
    {

        if (health <= 80) 
        {
            anim.SetTrigger("stageTwo");
            if (i < 10 && canFall)
            {
                Star[i].SetActive(true);
                Star[i].GetComponent<Rigidbody2D>().velocity = transform.forward * StarSpeed;
                canFall = false;
                StartCoroutine(StopFalling());
                i++;
            }
        }
        if (health <= 60)
        {
            if (i < 20 && canFall)
            {
                Star[i].SetActive(true);
                Star[i].GetComponent<Rigidbody2D>().velocity = transform.forward * StarSpeed;
                canFall = false;
                StartCoroutine(StopFalling());
                i++;
            }
        }
        if (health <= 40)
        {
            if (i < 30 && canFall)
            {
                Star[i].SetActive(true);
                Star[i].GetComponent<Rigidbody2D>().velocity = transform.forward * StarSpeed;
                canFall = false;
                StartCoroutine(StopFalling());
                i++;
            }
            
        }
        if (health <= 20)
        {
        if (i < 40 && canFall)
            {
                Star[i].SetActive(true);
                Star[i].GetComponent<Rigidbody2D>().velocity = transform.forward * StarSpeed;
                canFall = false;
                StartCoroutine(StopFalling());
                i++;
            }
            
        }

        if (health <= 0) {
            anim.SetTrigger("death");
            door.SetActive(true);
        }

        // give the player some time to recover before taking more damage !
        if (timeBtwDamage > 0) {
            timeBtwDamage -= Time.deltaTime;
        }

        healthBar.value = health;
    }


    private IEnumerator StopFalling()
    {
        yield return new WaitForSeconds(3);
        canFall = true;

    }

    private void OnTriggerEnter2D(Collider2D other)
    {
        // deal the player damage ! 
        if (other.CompareTag("Player") && other.GetType().ToString() == "UnityEngine.PolygonCollider2D" && isDead == false) {
            if (timeBtwDamage <= 0) {
                other.GetComponent<PlayerHealth>().DamgePlayer(damage);
            }
        } 
    }
}
