using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Move : MonoBehaviour
{
    public Rigidbody2D rb;
    public Animator ani;
    public float speed;
    public float jumpforce;
    public LayerMask ground;
    public Collider2D coll;


    public float KBForce;
    public float KBCounter;
    public float KBTotalTime;
    public bool knockFromRight;
    public float horizontal;

    private bool canDash = true;
    private bool isDashing;
    private float dashingPower = 30f;
    private float dashingTime = 0.5f;
    private Vector2 dashingDir;
    public TrailRenderer tr;
    private bool Dash_on = false;

    [SerializeField] private AudioSource Jump;

    // Start is called before the first frame update
    void Start()
    {
        rb = GetComponent<Rigidbody2D>();
        ani = GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {
        Movement();
        SwitchAni();
        KB();
        horizontal =  Input.GetAxisRaw("Horizontal");

        if (Input.GetKeyDown(KeyCode.LeftShift) && canDash && Dash_on)
        {
            isDashing = true;
            canDash = false;
            tr.emitting = true;
            dashingDir = new Vector2(Input.GetAxisRaw("Horizontal") , Input.GetAxisRaw("Vertical"));
            if (dashingDir == Vector2.zero)
            {
                dashingDir = new Vector2(transform.localScale.x, 0);

            }
            StartCoroutine(StopDashing());
        }
        if (coll.IsTouchingLayers(ground))
        {
            canDash = true;
        }

        if (isDashing)
        {
            rb.velocity = dashingDir.normalized * dashingPower;
            return;
        }

        
    }

    private IEnumerator StopDashing()
    {
        yield return new WaitForSeconds(dashingTime);
        tr.emitting = false;
        isDashing = false;
    }
    

    void Movement(){
        float HorizontalMove;
        HorizontalMove = Input.GetAxis("Horizontal");
        float direction = Input.GetAxisRaw("Horizontal");

        //Movement
        if (HorizontalMove !=0){
            rb.velocity = new Vector2(HorizontalMove * speed, rb.velocity.y);
            ani.SetFloat("running", Mathf.Abs(direction));
        }
        //facedirection
        if (direction != 0) {
            transform.localScale = new Vector3(-direction,1,1);
        }

        if (Input.GetButtonDown("Jump") && coll.IsTouchingLayers(ground)) {
            Jump.Play();
            ani.SetBool("jumping", true);
            rb.velocity = new Vector2(rb.velocity.x, jumpforce);
        } 
    }


    void SwitchAni(){
        ani.SetBool("idle", false);
        if (ani.GetBool("jumping")) {
            if (rb.velocity.y < 0) {
                ani.SetBool("jumping", false);
                ani.SetBool("falling", true);
            }
        } 
        else if (coll.IsTouchingLayers(ground)) {
            ani.SetBool("falling", false);
            ani.SetBool("idle", true);
        }
    }



    void KB () 
    {
        float input = Input.GetAxis("Horizontal");
        if(KBCounter <= 0)
        {
            rb.velocity = new Vector2(input * speed, rb.velocity.y);
        }
        else
        {
            if(knockFromRight == true)
            {
                rb.velocity = new Vector2(-KBForce, KBForce);
            }
            if (knockFromRight == false)
            {
                rb.velocity = new Vector2(KBForce, KBForce);
            }

            KBCounter -= Time.deltaTime;
        }
    }

    void OnTriggerEnter2D(Collider2D other)
    {
        if (other.gameObject.CompareTag("Potion"))
        {
            Dash_on = true;
            Destroy(other.gameObject);
        }
    }


}
