using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
public class Script_SettingsSliders : MonoBehaviour
{
    public string search;
    public Slider slider;
    // Start is called before the first frame update
    void Start()
    {
        int content = PlayerPrefs.GetInt(search);
        slider.value = content;
    }

    // Update is called once per frame
    void Update()
    {
        PlayerPrefs.SetInt(search, (int)slider.value);
    }
}
