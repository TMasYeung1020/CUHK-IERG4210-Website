using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class Enemy : MonoBehaviour
{
    public int health;
    public int damage;
    public float flashtime;

    private SpriteRenderer sr;
    private Color originalColor;
    
    public PlayerHealth playerHealth;

    public  Move playerMovement;

    // Start is called before the first frame update
    public void Start()
    {
        playerHealth = GameObject.FindGameObjectWithTag("Player").GetComponent<PlayerHealth>();
        sr = GetComponent<SpriteRenderer>();
        originalColor = sr.color;
    }

    // Update is called once per frame
    public void Update()
    {
    }

    public void TakeDamage(int damage)
    {
        health -= damage;
        FlashColor(flashtime);
    }
    
        void FlashColor(float time)
    {
        sr.color = Color.red;
        Invoke("ResetColor", time);
    }

    void ResetColor()
    {
        sr.color = originalColor;
    }

    

}
