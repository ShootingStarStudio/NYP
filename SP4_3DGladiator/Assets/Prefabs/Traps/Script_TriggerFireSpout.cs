using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class Script_TriggerFireSpout : MonoBehaviour
{
    public int iDamage = 2;

    public float minCooldown, maxCooldown;
    private float fCooldown;
    private float fCurrentCooldown = 0f;

    private bool isActive = false;
    public float minActiveTime, maxActiveTime;
    private float fActiveTime;
    private float fTime = 0f;

    public float fDamageTick = 0.2f;
    private float fDamageCooldown = 0f;
    private AudioSource source;

    ParticleSystem fireParticle;

    // Start is called before the first frame update
    void Start()
    {
        fireParticle = GetComponent<ParticleSystem>();

        fCooldown = Random.Range(minCooldown, maxCooldown);
        fCurrentCooldown = fCooldown;
        fActiveTime = Random.Range(minCooldown, maxCooldown);
        fTime = fActiveTime;
    }

    // Update is called once per frame
    void Update()
    {
        if (isActive)
        {
            fireParticle.Play();
           // fDamageCooldown -= Time.deltaTime;
            fDamageCooldown = Mathf.Clamp(fDamageCooldown - 1 * Time.deltaTime, 0, fDamageTick);
            

            fTime -= Time.deltaTime;
            fTime = Mathf.Clamp(fTime - 1 * Time.deltaTime, 0, fActiveTime);
            if (fTime <= 0f)
            {
                fTime = 0f;
                fCurrentCooldown = fCooldown;
                fDamageCooldown = 0f;
                isActive = false;
            }

        }
        else
        {
            fireParticle.Stop();
            fCurrentCooldown -= Time.deltaTime;
            fCurrentCooldown = Mathf.Clamp(fCurrentCooldown - 1 * Time.deltaTime, 0, fCooldown);
            if (fCurrentCooldown <= 0f)
            {
                fCurrentCooldown = 0f;
                fTime = fActiveTime;
                fDamageCooldown = fDamageTick;
                isActive = true;
            }
        }
    }

    private void OnTriggerEnter(Collider other)
    {
        Debug.Log("fire:)");
    }
    private void OnTriggerExit(Collider other)
    {
        Debug.Log("nofire:(");
    }
    private void OnTriggerStay(Collider other)
    {
        Script_baseHealth Entity = other.GetComponent<Script_baseHealth>();
        if (Entity != null && isActive && fDamageCooldown == 0)
        {
            Debug.Log(Entity.getHealth());
            Entity.TakeDamage(iDamage,gameObject);
            fDamageCooldown = fDamageTick;
            source.Play();
        }
    }
}
