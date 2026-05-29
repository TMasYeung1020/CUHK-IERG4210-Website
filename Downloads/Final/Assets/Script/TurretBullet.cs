using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TurretBullet : MonoBehaviour
{
    private GameObject player;
    private Rigidbody2D rb;
    public float force;

    // Start is called before the first frame update
    void Start()
    {
        rb = GetComponent<Rigidbody2D>();
        player = GameObject.FindGameObjectWithTag("Player");

        Vector3 direction = player.transform.position - transform.position;
        rb.velocity = new Vector2(direction.x, direction.y).normalized * force;
    }

    // Update is called once per frame
    void Update()
    {

    }

    void OnTriggerEnter2D(Collider2D other)
    {
        if (other.gameObject.CompareTag("Player")
        && other.GetType().ToString() == "UnityEngine.PolygonCollider2D")
        {
            other.gameObject.GetComponent<PlayerHealth>().DamgePlayer(2);
            Destroy(gameObject);
        }
        
    }
}

