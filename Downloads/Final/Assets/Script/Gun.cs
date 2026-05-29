using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Gun : MonoBehaviour
{
    public GameObject gun;
    public GameObject bulletPrefab;
    public Transform FirePoint;
    public SpriteRenderer myRenderer;
    [SerializeField] private AudioSource shoot;


    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetButtonDown("Attack"))
        {
            Attack();

            myRenderer.sortingLayerName = "background";
        }
        if (Input.GetButtonUp("Attack"))
        {
            gun.SetActive(false);
            myRenderer.sortingLayerName = "player";
        }
    }

    private void Attack()
    {
        shoot.Play();
        gun.SetActive(true);
        Instantiate(bulletPrefab, FirePoint.position, FirePoint.rotation);
    }
}
