using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class Win : MonoBehaviour
{
    public Text starNum;
    public Text semgpa;
    public Text cgpa;
    public StarUI CurrentStar;

    public float a;
    public float b;

    private float sem1gpa;
    private float sem2gpa;
    private float sem3gpa;
    private float sem4gpa;

    private int RestartStarNumj;

    void Start()
    {
        RestartStarNumj = PlayerPrefs.GetInt("TotalStarNum");
        if (SceneManager.GetActiveScene().buildIndex == 1)
        {
            starNum.text = CurrentStar.WinStarNum.ToString();
            a = CurrentStar.WinStarNum/10f;
            semgpa.text = a.ToString("0.00");
            cgpa.text = a.ToString("0.00");
            PlayerPrefs.SetInt("TotalStarNum", CurrentStar.WinStarNum);
        }
        if (SceneManager.GetActiveScene().buildIndex == 2 || SceneManager.GetActiveScene().buildIndex == 3 || SceneManager.GetActiveScene().buildIndex == 4)
        {
            starNum.text = CurrentStar.WinStarNum.ToString();
            a = CurrentStar.WinStarNum/10f;
            semgpa.text = a.ToString("0.00");
            b = (CurrentStar.WinStarNum + PlayerPrefs.GetInt("TotalStarNum"))/(SceneManager.GetActiveScene().buildIndex*10f);
            cgpa.text = b.ToString("0.00");
            PlayerPrefs.SetInt("TotalStarNum", CurrentStar.WinStarNum + PlayerPrefs.GetInt("TotalStarNum"));
        }


    }

    public void Restart()
    {
        SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex);
        PlayerPrefs.SetInt("TotalStarNum", RestartStarNumj);
        Time.timeScale = 1f;
    }

    public void nextStage()
    {
        if (SceneManager.GetActiveScene().buildIndex != 4)
        {
            SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex +1);
            Time.timeScale = 1f;
        }
        else
        {
            SceneManager.LoadScene(6);
        }
    }
}
