using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Dead : MonoBehaviour
{
    private PlayerHealth playerHealth;
    private int damge = 100;
    // Start is called before the first frame update
    void Start()
    {
        playerHealth = GameObject.FindGameObjectWithTag("Player").GetComponent<PlayerHealth>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    void OnTriggerEnter2D(Collider2D other)
    {
        if(other.CompareTag("Player") && other.GetType().ToString() == "UnityEngine.PolygonCollider2D")
        {
            playerHealth.DamgePlayer(damge);
        }
    }
}
