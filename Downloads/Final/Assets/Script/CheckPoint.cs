using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CheckPoint : MonoBehaviour
{
    private PlayerHealth playerHealth;
    [SerializeField] private AudioSource check;

    void Start()
    {
        playerHealth = GameObject.Find("Player").GetComponent<PlayerHealth>();
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.name == "Player")
        {
            check.Play();
            playerHealth.checkPoint = transform.position;
        }
    }
    
}
