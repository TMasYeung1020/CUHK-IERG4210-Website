using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WeaponController : MonoBehaviour
{
    public GameObject sword;
    public GameObject Chest_sword;
    private bool GetSword = false;
    public GameObject hammer;
    public GameObject Chest_hammer;
    private bool GetHammer = false;
    private GameObject[] weapon;
    public int CurrentWeaponIndex = -1;
     public GameObject gun;
    public GameObject Chest_gun;
    private bool Getgun = false;

    // Start is called before the first frame update
    void Start()
    {
        weapon = new GameObject[4];
    }

    // Update is called once per frame
    void Update()
    {
        if (GetSword == true)
        {
            weapon[0] = sword;
        }
        if (GetHammer == true)
        {
            weapon[1] = hammer;
        }
        if (Getgun == true)
        {
            weapon[2] = gun;
        }
        if (Input.GetKeyDown(KeyCode.Q))
        {
            SwitchWeapon();
        }
    }

    private void SwitchWeapon()
    {
        if (CurrentWeaponIndex >= 0)
        {
            if (weapon[CurrentWeaponIndex] is not null)
            {
                weapon[CurrentWeaponIndex].SetActive(false);
            }
        }

        if (CurrentWeaponIndex != 3)
        {
            CurrentWeaponIndex++;
        } else
        {
            CurrentWeaponIndex = 0;
        }

        if (weapon[CurrentWeaponIndex] is not null)
        {
            weapon[CurrentWeaponIndex].SetActive(true);
        }
        else
        {
            SwitchWeapon();
        }
    }

    void OnTriggerEnter2D (Collider2D other)
    {
        if (other.gameObject.CompareTag("Sword"))
        {
            Destroy(Chest_sword);
            GetSword = true;
        }
        if (other.gameObject.CompareTag("Hammer"))
        {
            Destroy(Chest_hammer);
            GetHammer = true;
        }
        if (other.gameObject.CompareTag("Gun"))
        {
            Destroy(Chest_gun);
            Getgun = true;
        }
    }

}
