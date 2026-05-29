using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Star : MonoBehaviour
{

    [SerializeField] private AudioSource CollectionSoundEffect;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnTriggerEnter2D(Collider2D other) {
    if (other.gameObject.CompareTag("Player")
    && other.GetType().ToString() == "UnityEngine.PolygonCollider2D") {
        CollectionSoundEffect.Play();
        StarUI.CurrentStarNum += 1;
        Destroy(gameObject);
    }
    }

}
