using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class ClickBTN : MonoBehaviour, IPointerDownHandler, IPointerUpHandler
{
    [SerializeField] private Image img;
    [SerializeField] private Sprite _default, pressed;
    [SerializeField] private AudioClip compressClip, uncompressClip;
    [SerializeField] private AudioSource source;
    [SerializeField] GameObject NewGamebtn;
    [SerializeField] GameObject Continuebtn;

    public void OnPointerDown(PointerEventData eventData)
    {
        img.sprite = pressed;
        source.PlayOneShot(compressClip);
    }
    public void OnPointerUp(PointerEventData eventData)
    {
        img.sprite = _default;
        source.PlayOneShot(uncompressClip);
        gameObject.SetActive(false);
        NewGamebtn.SetActive(true);
        Continuebtn.SetActive(true);
    }

}
