using UnityEngine;

public class GameContext : MonoBehaviour 
{
    private int score;

    public static GameContext Instance;

    private void Start() 
    {
        if (Instance != null)
        {
            DestroyObject(gameObject);
            return;
        }    

        Instance = this;
    }

    public void AddScore(int score)
    {
        this.score += score;
    }

    private void OnGUI() 
    {
        GUI.skin.label.fontSize = 25;

        GUI.Label(new Rect (25, 25, 500, 50), "Score: " + score);
    }
}