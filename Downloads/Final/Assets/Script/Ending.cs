using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using UnityEngine.SceneManagement;

public class Ending : MonoBehaviour
{
    public GameObject HKU;
    public GameObject Poly;
    public GameObject LIngu;
    public GameObject Mu;
    public GameObject NO;

    void Start()
    {
        if (PlayerPrefs.GetInt("TotalStarNum")/40f >= 3.8)
        {
            Debug.Log("a");
            HKU.SetActive(true);
        }
        if (3.8 > PlayerPrefs.GetInt("TotalStarNum")/40f && PlayerPrefs.GetInt("TotalStarNum")/40f >= 3.3)
        {
            Debug.Log("b");
            Poly.SetActive(true);
        }
        if (3.3 > PlayerPrefs.GetInt("TotalStarNum")/40f && PlayerPrefs.GetInt("TotalStarNum")/40f >= 2.7)
        {
            Debug.Log("c");
            LIngu.SetActive(true);
        }
        if (2.7 > PlayerPrefs.GetInt("TotalStarNum")/40f && PlayerPrefs.GetInt("TotalStarNum")/40f >= 2)
        {
            Debug.Log("d");
            Mu.SetActive(true);
        }     
        if (PlayerPrefs.GetInt("TotalStarNum")/40f < 2)
        {
            Debug.Log("e");
            NO.SetActive(true);
        }

    }

}
