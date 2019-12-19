using UnityEngine;
using UnityEngine.SceneManagement;

public class Player : MonoBehaviour
{
 	public float MaxSpeed = 5f;
	public float JumpVelocity = 8f;
	
	private bool isGrounded = false;
	private bool doubleJumped = false;
	private Transform groundTag;

	public LayerMask GroundMask;

	public float Motion;

	private BoxCollider2D boxCollider;
	private Rigidbody2D rigidBody;
	private PlayerAnimatorController animatorController;
	private AudioSource jumpSound;

	public static Player Instance;
	
	void Start () 
	{        
		if (Instance != null)
        {
            DestroyObject(gameObject);
            return;
        }

        Instance = this;

		boxCollider = GetComponent<BoxCollider2D>();
		rigidBody = GetComponent<Rigidbody2D>();
		animatorController = GetComponent<PlayerAnimatorController>();
		jumpSound = GetComponent<AudioSource>();

		groundTag = GameObject.Find(this.name + "/groundTag").transform;
	}
	
	void FixedUpdate () 
	{
		isGrounded = Physics2D.Linecast(transform.position, groundTag.position, GroundMask);
		if (isGrounded) doubleJumped = false;

		animatorController.UpdateIsGrounded(isGrounded);
		animatorController.UpdateMotion(Motion);

		if (transform.position.y < -5) Die();
	}

	void Update()
	{
		foreach(Touch touch in Input.touches)
        {
			var currentTouchPos = Camera.main.ScreenToWorldPoint(touch.position);
			if (VirtualJoystick.Instance.ContainsPoint(currentTouchPos)) continue;

            if (touch.phase == TouchPhase.Began) Jump();
		}
		
		rigidBody.velocity = new Vector2(Motion * MaxSpeed, rigidBody.velocity.y);
	}
	
	private void Jump()
	{
		float velocity = JumpVelocity;
		if (isGrounded || !doubleJumped)
		{
			doubleJumped = !isGrounded;
			if (doubleJumped) velocity *= 0.75f;
			rigidBody.velocity = new Vector2(rigidBody.velocity.x, velocity);
			jumpSound.Play(0);
		}
	} 

	public void Die() 
	{
		SceneManager.LoadScene("scene");
	}
}
