using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameOver : MonoBehaviour
{
    public PlayerHealth playerHealth;

    private int currentSceneIndex;
    
    public void gameOver()
    {
        gameObject.SetActive(true);
    }

    public void Retry()
    {
        if (SceneManager.GetActiveScene().buildIndex != 4)
        {
            playerHealth.RespawnNow();
        }
        else
        {
            SceneManager.LoadScene(4);
        }
        gameObject.SetActive(false);
    }

    public void Exit(int sceneID)
    {
        currentSceneIndex = SceneManager.GetActiveScene().buildIndex;
        PlayerPrefs.SetInt("SavedScene", currentSceneIndex);
        SceneManager.LoadScene(sceneID);
    }
}
