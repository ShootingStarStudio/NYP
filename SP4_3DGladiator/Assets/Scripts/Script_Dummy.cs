using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Audio;

public class Script_Dummy : MonoBehaviour
{
    public float timer;
    public float deadtime;
    public float deadneutraltime;
    public float revivetime;

    public bool dead = false;
    public bool deadneutral = false;
    public bool revive = false;
    public Animator anim;

    public Script_baseHealth health;
    private int currentHP = 100;
    private int lastHP = 100;

    public AudioClip hit;
    public AudioClip death;
    private AudioSource source;
    // Start is called before the first frame update
    void Start()
    {
        health = GetComponent<Script_baseHealth>();
        source = GetComponent<AudioSource>();
    }

    // Update is called once per frame
    void Update()
    {
        timer = Mathf.Clamp(timer - Time.deltaTime, 0, 1);
        if (deadneutral)
        {
            if (deadneutraltime > 0)
            {
                deadneutraltime = Mathf.Clamp(deadneutraltime - Time.deltaTime, 0, 5);
            }
            else
            {
                deadneutral = false;
                anim.SetBool("dead", false);
                anim.SetBool("deadneutral", true);
                deadtime = 2;
                dead = true;
            }
        }
        if (dead)
        {          
            if (deadtime > 0)
            {
                deadtime = Mathf.Clamp(deadtime - Time.deltaTime, 0, 5);
            }
            else
            {
                dead = false;
                anim.SetBool("deadneutral", false);
                anim.SetBool("revive", true);
                revivetime = 0.9f;
                revive = true;
            }
        }
        if (revive)
        {
            if (revivetime > 0)
            {
                revivetime = Mathf.Clamp(revivetime - Time.deltaTime, 0, 5);
            }
            else
            {
                revive = false;
                anim.SetBool("revive", false);
                anim.SetBool("backtoneutral", true);
                health.Healing(100);
                lastHP = currentHP;
            }
        }

        currentHP = health.getHealth();
        if (currentHP == 100)
        {
            lastHP = currentHP;
            anim.SetBool("hit", false);

        }

        if (currentHP <= 0)
        {
            health.selfHealth = 1;
            lastHP = currentHP;
            anim.SetBool("backtoneutral", false);
            anim.SetBool("dead", true);
            lastHP = currentHP;
            source.PlayOneShot(death);
            deadneutraltime = 1;
            deadneutral = true;
        }
        else if (currentHP < lastHP)
        {
            lastHP = currentHP;
            anim.SetBool("hit", true);
            timer = 1f;
            source.PlayOneShot(hit);
            //Debug.Log(anim.GetBool("dummy hit"));

        }    
        if (timer <= 0)
        { 
            anim.SetBool("hit", false);
        }
    }
}
