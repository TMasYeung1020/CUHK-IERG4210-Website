using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class StarUI : MonoBehaviour
{

    public int star;
    public Text StarNum;

    public static int CurrentStarNum;
    public int WinStarNum;
    // Start is called before the first frame update
    void Start()
    {
        CurrentStarNum = star;
    }

    // Update is called once per frame
    void Update()
    {
        StarNum.text = CurrentStarNum.ToString();
        WinStarNum = CurrentStarNum;
    }
}
