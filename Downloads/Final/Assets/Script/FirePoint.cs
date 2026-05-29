using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FirePoint : MonoBehaviour
{
    public Move player;
    public bool isFacingRight = true;
    
    void Update()
    {
        Flip();
    }
    
    
    private void Flip()
    {
        if (isFacingRight && player.horizontal < 0f || !isFacingRight && player.horizontal > 0f)
        {
            isFacingRight = !isFacingRight;
            transform.Rotate(0f, 180f, 0f);
        }
    }

}
