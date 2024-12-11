using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyProjectileCollision : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.name.Contains("FirstPerson"))
        {
            other.GetComponent<Script_baseHealth>().TakeDamage(5,gameObject);
            Debug.LogWarning("hit: " + other.GetComponent<Script_baseHealth>().selfHealth);
            Destroy(transform.parent.gameObject);
        }
        Destroy(transform.parent.gameObject);
    }
}
