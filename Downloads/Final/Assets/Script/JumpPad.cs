using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class JumpPad : MonoBehaviour
{
    public float bounce;
    private Animator anim;
    private bool JumpCD = true;

    void Start()
    {
        anim = GetComponent<Animator>();
    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        if (collision.gameObject.CompareTag("Player") && JumpCD)
        {
            collision.gameObject.GetComponent<Rigidbody2D>().AddForce(Vector2.up*bounce, ForceMode2D.Impulse);
            anim.SetTrigger("jump");
            JumpCD = false;
            Invoke("CD",1);
        }
    }

    private void CD()
    {
        JumpCD = true;
    }
}
