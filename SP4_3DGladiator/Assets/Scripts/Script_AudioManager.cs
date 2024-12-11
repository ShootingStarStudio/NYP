using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Audio;

public class Script_AudioManager : MonoBehaviour
{

    [SerializeField] private AudioMixer BGMgroup;
    [SerializeField] private AudioMixer SFXgroup;
    

    // Start is called before the first frame update
    void Start()
    {
        float bgmvolume = (float)PlayerPrefs.GetInt("BGMVOLUME");
        float sfxvolume = (float)PlayerPrefs.GetInt("SFXVOLUME");
        Debug.LogWarning(sfxvolume);
        BGMgroup.SetFloat("BGMvol", bgmvolume);
        SFXgroup.SetFloat("SFXvol", sfxvolume);
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void UpdateMixerVolume()
    {

    }
}
