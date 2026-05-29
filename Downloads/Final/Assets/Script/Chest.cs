using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Chest : MonoBehaviour
{
    public float delaytime;
    private bool canOpen;
    private bool isOpened;
    private Animator anim;
    public Transform weapon;
    [SerializeField] private AudioSource open;
    // Start is called before the first frame update
    void Start()
    {
        anim = GetComponent<Animator>();
        isOpened = false;
    }

    // Update is called once per frame
    void Update()
    {
        if(Input.GetKeyDown(KeyCode.R))
        {
            if (canOpen && !isOpened)
            {
                open.Play();
                anim.SetTrigger("opening");
                isOpened = true;
                Invoke("GetWeopon", delaytime);
            }
        }
    }

    void OnTriggerEnter2D(Collider2D other)
    {
        if (other.gameObject.CompareTag("Player")
        && other.GetType().ToString() == "UnityEngine.PolygonCollider2D")
        {
            canOpen = true;
        }
    }

    void OnTriggerExit2D (Collider2D other)
    {
        if (other.gameObject.CompareTag("Player")
        && other.GetType().ToString() == "UnityEngine.PolygonCollider2D")
        {
            canOpen = false;
        }
    }

    void GetWeopon()
    {
        weapon.position = gameObject.transform.position;
    }
}
