using UnityEngine;

public class CameraMovement : MonoBehaviour 
{
	public float dampTime = 0.15f;
	private Vector3 velocity = Vector3.zero;
	public Transform target;
	
	private new Camera camera;

	private void Start() 
	{
		camera = GetComponent<Camera>();	
	}

	void Update () 
	{
		if (target)
		{
			Vector3 point = camera.WorldToViewportPoint(target.position + Vector3.up * 0.75f);
			Vector3 delta = target.position + Vector3.up * 0.75f - camera.ViewportToWorldPoint(new Vector3(0.5f, 0.5f, point.z));
			Vector3 destination = transform.position + delta;

			if (destination.y < 0) destination.y = 0;

			transform.position = Vector3.SmoothDamp(transform.position, destination, ref velocity, dampTime);
		}
		
	}
}