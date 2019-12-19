using System.Collections.Generic;
using UnityEngine;

public class VirtualJoystick : MonoBehaviour {
    public float speed = 5.0f;

    private bool isTouching = false;
    private Vector2 touchPos;

    public Transform circle;
    public Transform outerCircle;

    private float outerCircleRadius;

    private HashSet<int> trackedTouches = new HashSet<int>();

    public static VirtualJoystick Instance;

    void Start() 
    {
        if (Instance != null)
        {
            DestroyObject(gameObject);
            return;
        }

        Instance = this;

        var outerCircleSprite = outerCircle.GetComponent<SpriteRenderer>();    
        outerCircleRadius = outerCircleSprite.bounds.extents.x;
    }

	void Update () 
    {   
        foreach(Touch touch in Input.touches)
        {
            var currentTouchPos = Camera.main.ScreenToWorldPoint(touch.position);

            if (touch.phase == TouchPhase.Began && ContainsPoint(currentTouchPos))
            {
                trackedTouches.Add(touch.fingerId);
            }

            if (!trackedTouches.Contains(touch.fingerId)) continue; 

            isTouching = touch.phase != TouchPhase.Ended;
            if (isTouching) 
            {
                touchPos = currentTouchPos;
            }
            else 
            {
                trackedTouches.Remove(touch.fingerId);
            }
		}        
	}
	
    public bool ContainsPoint(Vector2 pos)
    {
        return Vector2.Distance(outerCircle.transform.position, pos) <= outerCircleRadius;
    }

    private void FixedUpdate()
    {
        if(isTouching)
        {
            Vector2 offset = touchPos - (Vector2) outerCircle.position;
            Vector2 direction = Vector2.ClampMagnitude(offset, 1.0f);
            Player.Instance.Motion = direction.x;

            circle.transform.position = outerCircle.transform.position + (Vector3) direction;
        }
        else
        {
            Player.Instance.Motion = 0;
            circle.transform.position = outerCircle.transform.position;
        }
	}
}