using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Script_GIbs : MonoBehaviour
{
    public float lifetime = 2;
    public bool vanishing = false;

    // Start is called before the first frame update
    void Start()
    { 

    }

    // Update is called once per frame
    void Update()
    {
        if (vanishing == true)
        {
            lifetime = Mathf.Clamp(lifetime -= Time.deltaTime, 0, 5);
        }
        if (lifetime <= 0)
        {
            gameObject.transform.localScale -= new Vector3 (0.01f,0.01f,0.01f);
            if (gameObject.transform.localScale.x <= 0)
            {
                Destroy(gameObject);
            }
        }
    }
}
