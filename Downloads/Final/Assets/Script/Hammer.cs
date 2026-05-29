using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Hammer : MonoBehaviour
{
    public int damage;
    public float time;
    public float AttackCD;
    private float isCD;
    public float Starttime;
    private Animator ani;
    private PolygonCollider2D coll2D;
    public SpriteRenderer myRenderer;
    public GameObject sword;
    [SerializeField] private AudioSource HammerSound;


    // Start is called before the first frame update
    void Start()
    {
        ani = GameObject.FindGameObjectWithTag("Player").GetComponent<Animator>();
        coll2D = GetComponent<PolygonCollider2D>();
    }

    // Update is called once per frame
    void Update()
    {
        SwordAttack();
    }

    void SwordAttack()
    {
        if (Input.GetButtonDown("Attack")&&isCD==0)
        {
            HammerSound.Play();
            sword.SetActive(true);
            myRenderer.sortingLayerName = "background";
            StartCoroutine(SwordStartAttack());
            isCD = 1;
            StartCoroutine(Swordcd());
        }
    }

    IEnumerator Swordcd()
    {
        yield return new WaitForSeconds(AttackCD);
        sword.SetActive(false);
        myRenderer.sortingLayerName = "player";
        isCD = 0;
    }

    IEnumerator SwordStartAttack()
    {
        yield return new WaitForSeconds(Starttime);
        coll2D.enabled = true;
        StartCoroutine(SworddisableHitBox());
    }
    IEnumerator SworddisableHitBox()
    {
        yield return new WaitForSeconds(time);
        coll2D.enabled = false;
    }

    void OnTriggerEnter2D(Collider2D other)
    {
        if(other.gameObject.CompareTag("Enemy"))
        {
            other.GetComponent<Enemy>().TakeDamage(damage);
        }
        if (other.CompareTag("Boss"))
        {
            other.GetComponent<Boss>().health -= damage;
        }
    }

}
