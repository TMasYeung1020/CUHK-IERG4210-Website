using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HackMode : MonoBehaviour
{
    public PlayerHealth playerhealth;
    private bool IsExecute = false;



    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        
    }
    
    public void Hack()
    {
        if (IsExecute == false)
        {
            playerhealth.health = 1000000;
            IsExecute = true;
        }
        else
        {
            playerhealth.health = 20;
            IsExecute = false;
        }
    }
}
