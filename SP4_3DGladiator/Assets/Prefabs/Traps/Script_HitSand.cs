using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Script_HitSand : MonoBehaviour
{
    public GameObject center;
    private void OnTriggerStay(Collider other)
    {
        if (other.gameObject.name == "FirstPersonController")
        {
            Debug.Log("Player Touch Sand");
        }
        else if (other.gameObject.name.Contains("NPC_Enemy"))
        {
            Debug.Log("Enemy Touch Water");
        }

        other.transform.position = Vector3.MoveTowards(other.transform.position, center.transform.position, 0.01f);
    }

    private void OnTriggerExit(Collider other)
    {
        if (other.gameObject.name == "FirstPersonController")
        {
            Debug.Log("Player Left Sand");
        }
        else if (other.gameObject.name.Contains("NPC_Enemy"))
        {
            Debug.Log("Enemy Left Water");
        }
    }
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
