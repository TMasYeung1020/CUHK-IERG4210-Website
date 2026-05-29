using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class OpenSetting : MonoBehaviour
{
    public Animator ani;
    public bool isActive = false;
    public GameObject slider;

    public void Open()
    {
        if (true)
        {
            ani.SetTrigger("Show");
            isActive = !isActive;
        }
        if (isActive == false)
        {
            slider.SetActive(false);
        }
    }
}
