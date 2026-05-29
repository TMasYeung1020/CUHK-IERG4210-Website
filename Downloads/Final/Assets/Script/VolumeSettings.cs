using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class VolumeSettings : MonoBehaviour
{
    public GameObject slider;
    
    public void Show()
    {
        if (slider != null)
        {
            bool isActive = slider.activeSelf;
            slider.SetActive(!isActive);
        }
    }
    
}