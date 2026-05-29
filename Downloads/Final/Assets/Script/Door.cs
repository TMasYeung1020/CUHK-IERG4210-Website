using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Door : MonoBehaviour
{
    public GameObject gameObject;
    public AudioSource Win;

    private void OnTriggerEnter2D(Collider2D other) {
    if (other.gameObject.CompareTag("Player")
    && other.GetType().ToString() == "UnityEngine.PolygonCollider2D") 
    {
        Win.Play();
        Time.timeScale = 0f;
        gameObject.SetActive(true);
    }
    }
}
