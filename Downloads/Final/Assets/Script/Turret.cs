using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Turret : MonoBehaviour
{

    public GameObject bullet;
    public Transform bulletPos;
    private float timer;
    public Transform Target;
    public GameObject AlarmLight;
    private GameObject player;

    // Start is called before the first frame update
    void Start()
    {
        player = GameObject.FindGameObjectWithTag("Player");
    }

    // Update is called once per frame
    void Update()
    {

        float distance = Vector2.Distance(transform.position , player.transform.position);


        if (distance< 26)
        {
            timer += Time.deltaTime;
            AlarmLight.GetComponent<SpriteRenderer>().color = Color.red;

            if(timer > 2)
            {
                timer = 0;
                shoot();
            }
        }
        else
        {
            AlarmLight.GetComponent<SpriteRenderer>().color = Color.black;
        }

       
    }

    void shoot()
    {
        Instantiate(bullet, bulletPos.position, Quaternion.identity);
    }

    
}
