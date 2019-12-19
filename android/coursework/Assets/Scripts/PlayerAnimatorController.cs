using UnityEngine;

public class PlayerAnimatorController : MonoBehaviour 
{
	public Transform Transform;
	public GameObject SpriteSheet;
	
	private Animator animator;
	private Vector3 scale;

	void Start () 
	{
		animator = SpriteSheet.GetComponent<Animator>();
		scale = Transform.localScale;
	}

	private void Flip(float motion)
	{
		if (motion * scale.x < 0)
		{
			scale.x *= -1;
			Transform.localScale = scale;
		}
	}

	public void UpdateMotion(float motion)
	{
		animator.SetFloat("Motion", motion);
		Flip(motion);
	}

	public void UpdateIsGrounded(bool isGrounded)
	{
		animator.SetBool("isGrounded", isGrounded);
	}
}
