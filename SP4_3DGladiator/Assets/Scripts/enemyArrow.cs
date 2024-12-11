using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class enemyArrow : MonoBehaviour
{
    // Start is called before the first frame update
    Rigidbody rb;
    void Awake()
    {
         rb = gameObject.GetComponent<Rigidbody>();
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        rb.MoveRotation(Quaternion.LookRotation(rb.velocity));
    }
}
