using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerHealth : MonoBehaviour
{
    public int health;
    public int Blinks;
    public float time;
    private Renderer myRender;
    private Animator ani;
    public float dieTime;

    public healthbar healthbar;

    private PolygonCollider2D polygonCollider2D;
    public float hitBoxCdTime;

    public Vector3 checkPoint;
    public bool isExcused = false;
    private bool CD = true;

    public GameOver gameOver;

    [SerializeField] private AudioSource Dead;

    // Start is called before the first frame update
    void Start()
    {
        myRender = GetComponent<Renderer>();
        ani = GetComponent<Animator>();
        healthbar.SetMaxHealth(health);
        polygonCollider2D = GetComponent<PolygonCollider2D>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void DamgePlayer(int damage)
    {
        health -= damage;
        healthbar.SetHealth(health);
        if(health <= 0 && CD == true)
        {
            CD = false;
            ani.SetBool("Respawn", false);
            if (isExcused == false)
            {
                ani.SetTrigger("Die");
                Dead.Play();
                isExcused = true;
            }
            Invoke("KillPlayer", dieTime);
        }
        BlinkPlayer(Blinks, time);
        polygonCollider2D.enabled = false;
        StartCoroutine(ShowPlayerHitbox());
    }

    IEnumerator ShowPlayerHitbox()
    {
        yield return new WaitForSeconds(hitBoxCdTime);
        polygonCollider2D.enabled = true;
    }

    void KillPlayer()
    {
        GameOverScreen();
    }

    void BlinkPlayer(int numBlinks, float seconds)
    {
        StartCoroutine(DoBlinks(numBlinks, seconds));
    }

    IEnumerator DoBlinks(int numBlinks, float seconds)
    {
        for (int i=0; i < numBlinks*2;i++)
        {
            myRender.enabled =! myRender.enabled;
            yield return new WaitForSeconds(seconds);
        }
        myRender.enabled = true;
    }

    public void RespawnNow()
    {
        ani.SetBool("Respawn", true);
        transform.position = checkPoint;
        health = 20;
        healthbar.SetMaxHealth(health);
        isExcused = false;
        Invoke("RespawnOnce", 2);
    }
    
    public void RespawnOnce()
    {
        CD = true;
    }

    public void GameOverScreen()
    {
        gameOver.gameOver();
    }
}
