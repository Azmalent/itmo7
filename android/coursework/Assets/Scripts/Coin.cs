using UnityEngine;
    
public class Coin : MonoBehaviour 
{
    private AudioSource pickupSound;

    private void Start() 
    {
        pickupSound = GetComponent<AudioSource>();
    }

    private void OnTriggerEnter2D(Collider2D other) 
    {
        if (other.tag == "Player")
        {
            GameContext.Instance.AddScore(1);
            pickupSound.Play(0);
            DestroyObject(gameObject, pickupSound.clip.length);
        }
    }
}