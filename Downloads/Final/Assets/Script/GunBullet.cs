using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GunBullet : MonoBehaviour
{
    public float speed;
    public Rigidbody2D rb;
    public int damage;

    void Start()
    {
        rb.velocity = transform.right * speed;
    }

    void Update()
    {

    }

    void OnTriggerEnter2D(Collider2D other)
    {
        if (other.gameObject.CompareTag("Enemy"))
        {
            other.GetComponent<Enemy>().TakeDamage(damage);
            gameObject.SetActive(false);
        }
        if (other.CompareTag("Boss"))
        {
            other.GetComponent<Boss>().health -= damage;
            gameObject.SetActive(false);
        }
    }
}
