using System.Diagnostics;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Enemy : MonoBehaviour 
{
	public float Speed = 1.5f;
	public LayerMask GroundMask;

	private Vector3 scale;
	private Rigidbody2D rigidBody;

	private float direction = 1;
	private float width;

	private void Start () 
	{
		scale = transform.localScale;
		rigidBody = GetComponent<Rigidbody2D>();

		width = GetComponent<SpriteRenderer>().bounds.extents.x;

		rigidBody.velocity = new Vector2(direction * Speed, rigidBody.velocity.y);
	}
	
	private void FixedUpdate() 
	{
		Vector2 linecastStart = transform.position;
		Vector2 linecastEnd = linecastStart + new Vector2(0.75f * direction, 0);

		bool hasObstacle = Physics2D.Linecast(linecastStart, linecastEnd, GroundMask);
		if (hasObstacle)
		{
			direction *= -1;
			rigidBody.velocity = new Vector2(direction * Speed, rigidBody.velocity.y);

			scale.x *= -1;
			transform.localScale = scale;
		}

		UnityEngine.Debug.DrawLine(linecastStart, linecastEnd);
	}

	private void OnCollisionEnter2D(Collision2D collision) 
    {
        if (collision.gameObject.tag == "Player")
        {
			Player.Instance.Die();
        }
    }
}
