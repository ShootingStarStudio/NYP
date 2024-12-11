using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Script_Teleport : MonoBehaviour
{
    public FirstPersonController fpc;
    public Vector3 TeleportPos = new Vector3(0,0,0);

    private float timer = 2f;
    private float cooldownTimer = 2f;

    public bool teleported = false;         //if recently teleported player
    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.tag == fpc.tag)
        {
            timer = 2f;
            fpc.tping = true;
        }
    }
    private void OnTriggerStay(Collider other)
    {
        if (other.gameObject.tag == fpc.tag && teleported == false)
        {
            timer = Mathf.Clamp(timer -= Time.deltaTime, 0, 2);
            if (timer <= 0)
            {
                fpc.transform.position = TeleportPos;
                teleported = true;
                cooldownTimer = 2f;
                fpc.tpcount++;
            }
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (teleported == true)
        {
            cooldownTimer -= Time.deltaTime;
            if (cooldownTimer <= 0)
            {
                cooldownTimer = 0;
                teleported = false;
            }
        }
        
    }
}
