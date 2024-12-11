using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class Script_TriggerSpikes : MonoBehaviour
{
    public int iDamage = 10;
    public float fCooldown = 3f;
    private float fCurrentCooldown = 0f;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {

        fCurrentCooldown = Mathf.Clamp(fCurrentCooldown - 1 * Time.deltaTime, 0, fCooldown);

    }

    private void OnTriggerEnter(Collider other)
    {
        
    }
    private void OnTriggerExit(Collider other)
    {

    }
    private void OnTriggerStay(Collider other)
    {
        Script_baseHealth Entity = other.GetComponent<Script_baseHealth>();
        if (Entity != null && fCurrentCooldown <= 0)
        {
            Entity.TakeDamage(iDamage,gameObject);
            Debug.Log("Spiked");
            fCurrentCooldown = fCooldown;
            Debug.Log(Entity.getHealth());
        }

    }
}
