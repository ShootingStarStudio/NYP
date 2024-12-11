using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Script_HitLava : MonoBehaviour
{
    public float LavaDamageTime = 0.3f;
    private float LavaDamageTimer;
    private bool InLava = false;
    private bool canDamage = false;

    // Dont bother trying to udnertsand the code. I dont know how it works but it does. ~Luke

    // Start is called before the first frame update
    void Awake()
    {
        LavaDamageTimer = LavaDamageTime;
    }

    // Update is called once per frame
    void Update()
    {
        if (InLava)
        {
            LavaDamageTimer -= Time.deltaTime;
        }
        else
        {
            LavaDamageTimer = LavaDamageTime;
        }


        if (LavaDamageTimer <= 0f)
        {
            LavaDamageTimer = LavaDamageTime;
            canDamage = true;
        }
        else
        {
            canDamage = false;
        }
    }
    private void OnTriggerStay(Collider other)
    {
        InLava = true;
        if (other.gameObject.name == "FirstPersonController")
        {
            Debug.Log("Player Touch Lava " + LavaDamageTimer);
            if (canDamage)
            {
                other.gameObject.GetComponent<Script_baseHealth>().TakeDamage(10, gameObject);
                canDamage = false;
            }
        }
        else if (other.gameObject.name.Contains("NPC_Enemy"))
        {
            Debug.Log("Enemy Touch Lava");
        }
        else if (other.gameObject.tag == "Weapon")
        {
            Destroy(other.gameObject);
        }
        else { }
        //if (canDamage)
        //{
        //    other.gameObject.GetComponent<Script_baseHealth>().TakeDamage(10,gameObject);
        //    canDamage = false;
        //}
    }

    private void OnTriggerExit(Collider other)
    {
        InLava = false;
        if (other.gameObject.name == "FirstPersonController")
        {
            Debug.Log("Player Leave Lava " + other.gameObject.GetComponent<Script_baseHealth>().getHealth());
        }
        else if (other.gameObject.name.Contains("NPC_Enemy"))
        {
            Debug.Log("Enemy Left Water");
        }
    }
}
