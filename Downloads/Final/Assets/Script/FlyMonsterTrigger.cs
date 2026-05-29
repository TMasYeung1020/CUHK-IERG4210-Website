using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FlyMonsterTrigger : MonoBehaviour
{
    public FlyMonster flymonster;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    private void OnTriggerEnter2D(Collider2D other)
    {
        if (other.CompareTag("Player"))
        {
            flymonster.chase = true;
        }
    }
    private void OnTriggerExit2D(Collider2D other)
    {
        if (other.CompareTag("Player"))
        {
            flymonster.chase = false;
        }
    }
}
