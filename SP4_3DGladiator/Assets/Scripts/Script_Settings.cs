using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Audio;

public class Script_Settings : MonoBehaviour
{
    // Start is called before the first frame update
    public int bgmvol;
    public int sfxvol;
    public Text bgmdisplay;
    public Text sfxdisplay;
    public Toggle healthToggle;
    public Toggle staminaToggle;
    public Toggle arrowToggle;
    
    public Image DefaultTab;
    
    public Image SettingsScreen;


    [SerializeField] private AudioMixer BGMgroup;
    [SerializeField] private AudioMixer SFXgroup;
    private AudioSource source;
    public AudioClip testsound;

    void Start()
    {
        bgmvol = PlayerPrefs.GetInt("BGMVOLUME");
        sfxvol = PlayerPrefs.GetInt("SFXVOLUME");
        bgmdisplay.text = (bgmvol + 80).ToString();
        sfxdisplay.text = (sfxvol + 80).ToString();

        for (int i = 0; i < DefaultTab.transform.childCount; i++)
        {
            DefaultTab.transform.GetChild(i).gameObject.SetActive(false);
        }

        #region Toggles

        #region Health Toggle
        bool HealthIsOn;
        if (PlayerPrefs.GetInt("HEALTHBARON") == 1)
        {
            HealthIsOn = true;
        }
        else
        {
            HealthIsOn = false;
        }
        healthToggle.isOn = HealthIsOn;
        #endregion

        #region Stamina Toggle
        bool StaminaIsOn;
        if (PlayerPrefs.GetInt("STAMBARON") == 1)
        {
            StaminaIsOn = true;
        }
        else
        {
            StaminaIsOn = false;
        }
        staminaToggle.isOn = StaminaIsOn;
        #endregion

        #region Arrow Counter Toggle
        bool ArrowIsOn;
        if (PlayerPrefs.GetInt("ARROWCOUNTON") == 1)
        {
            ArrowIsOn = true;
        }
        else
        {
            ArrowIsOn = false;
        }
        arrowToggle.isOn = ArrowIsOn;
        #endregion

        #endregion
    }

    private void Awake()
    {
        source = GetComponent<AudioSource>();
    }
    // Update is called once per frame
    void Update()
    {
        setHealthBarToggle();
        setStaminaBarToggle();
        setArrowCountToggle();
    }
    public void setBGMvol(float num)
    {
        bgmvol = (int)num;
        bgmdisplay.text = (bgmvol+80).ToString();
        BGMgroup.SetFloat("BGMvol", num);
    }

    public void setSFXvol(float num)
    {
        sfxvol = (int)num;
        sfxdisplay.text = (sfxvol+80).ToString();
        SFXgroup.SetFloat("SFXvol", num);
    }

    public void setHealthBarToggle()
    {
        if (healthToggle.isOn)
        {
            PlayerPrefs.SetInt("HEALTHBARON", 1);
        }
        else
        {
            PlayerPrefs.SetInt("HEALTHBARON", 0);
        }
    }

    public void setStaminaBarToggle()
    {
        if (staminaToggle.isOn)
        {
            PlayerPrefs.SetInt("STAMBARON", 1);
        }
        else
        {
            PlayerPrefs.SetInt("STAMBARON", 0);
        }
    }

    public void setArrowCountToggle()
    {
        if (arrowToggle.isOn)
        {
            PlayerPrefs.SetInt("ARROWCOUNTON", 1);
        }
        else
        {
            PlayerPrefs.SetInt("ARROWCOUNTON", 0);
        }
    }

    public void cleardata()
    {
        for (int i = 0; i < PlayerPrefs.GetInt("playamount");i++)
        {
            PlayerPrefs.DeleteKey("deathArray" + i);
        }
        PlayerPrefs.DeleteKey("playamount");
        Debug.Log(PlayerPrefs.GetInt("playamount"));
    }

    public void Save()
    {
        PlayerPrefs.SetInt("BGMVOLUME", bgmvol);
        PlayerPrefs.SetInt("SFXVOLUME", sfxvol);
        SettingsScreen.gameObject.SetActive(false);
    }

    public void ToggleTab(int tab)
    {
        for (int i=0;i<DefaultTab.transform.childCount;i++)
        {
            DefaultTab.transform.GetChild(i).gameObject.SetActive(false);
        }
        DefaultTab.transform.GetChild(tab).gameObject.SetActive(true);
    }

    public void OpenSettings()
    {
        SettingsScreen.gameObject.SetActive(true);
    }

    public void playsound()
    {
        source.PlayOneShot(testsound);
    }
}
