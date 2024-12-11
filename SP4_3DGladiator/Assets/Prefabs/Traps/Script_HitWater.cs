using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Script_HitWater : MonoBehaviour
{
    //private float playerspeed = 0;
    private bool soundIsPlaying = false;
    private void OnTriggerStay(Collider other)
    {
        
        if (other.gameObject.name == "FirstPersonController")
        {
            //playerspeed = Vector3.Magnitude( other.gameObject.GetComponent<Rigidbody>().velocity);
            Debug.Log("Player Touch Water " + other.gameObject.GetComponent<FirstPersonController>().walkSpeed);
            other.gameObject.GetComponent<FirstPersonController>().isInWater = true;
            other.gameObject.GetComponent<FirstPersonController>().walkSpeed = 2.5f;
            if (other.gameObject.GetComponent<FirstPersonController>().isWalking && soundIsPlaying == false)
            {
                other.GetComponent<AudioSource>().Play();
                soundIsPlaying = true;
            }
            if (other.gameObject.GetComponent<FirstPersonController>().isWalking == false)
            {
                other.gameObject.GetComponent<AudioSource>().Stop();
                soundIsPlaying = false;
            }
        }
        else if (other.gameObject.name.Contains("NPC_Enemy"))
        {
            Debug.Log("Enemy Touch Water");
        }
    }

    private void OnTriggerExit(Collider other)
    {
        if (other.gameObject.name == "FirstPersonController")
        {
            Debug.Log("Player Left Water " + other.gameObject.GetComponent<FirstPersonController>().walkSpeed);
            other.gameObject.GetComponent<FirstPersonController>().isInWater = false;
            other.gameObject.GetComponent<FirstPersonController>().walkSpeed = 5.0f;
            other.gameObject.GetComponent<AudioSource>().Stop();
            soundIsPlaying = false;
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
