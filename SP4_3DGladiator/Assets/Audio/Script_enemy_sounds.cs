using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Script_enemy_sounds : MonoBehaviour
{
    public AudioClip hit;
    public AudioClip death;
    private AudioSource source;
    // Start is called before the first frame update
    void Start()
    {
        source = GetComponent<AudioSource>();
    }
    private void Awake()
    {
        source = GetComponent<AudioSource>();
    }

    // Update is called once per frame
    void Update()
    {
       
    }
    public void hitsound()
    {
        source.PlayOneShot(hit);
    }
    public void deadsound()
    {
        source.PlayOneShot(death);
    }
}
