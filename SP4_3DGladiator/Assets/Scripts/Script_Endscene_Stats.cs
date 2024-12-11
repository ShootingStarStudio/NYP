using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Script_Endscene_Stats : MonoBehaviour
{
    private bool generated = false;
    public string[] statsarray;
    public string finalstr;
    private int arenanumber;
    private string arenaname;
    private string causeofdeath;
    [SerializeField]
    FirstPersonController fpc;
    // Start is called before the first frame update
    void Start()
    {
        PlayerPrefs.SetInt("dmgtaken", 0);
    }

    // Update is called once per frame
    void Update()
    {
        if(fpc.health.selfHealth <= 0 && generated == false)
        {
            arenanumber = fpc.tpcount;
            arenanumber = arenanumber % 5;
            switch (arenanumber)
            {
                case 1: arenaname = "Forest";
                    break;
                case 2: arenaname = "Desert";
                    break;
                case 3: arenaname = "Volcano";
                    break;
                case 4: arenaname = "Castle";
                    break;
                case 0: arenaname = "how are you dying in the hub";
                    break;     
            }
            causeofdeath = fpc.lasthitfrom;
            if (causeofdeath.Contains("Trap_Spikes")) causeofdeath = "Spike Trap";
            if (causeofdeath.Contains("Lava")) causeofdeath = "Lava Pit";
            if (causeofdeath.Contains("Fire")) causeofdeath = "Fire Geyser";
            if (causeofdeath.Contains("Arrow")) causeofdeath = "Arrow";
            generateStats();
            generated = true;
        }
    }
    void generateStats()
    {
        statsarray = new string[4];
        PlayerPrefs.SetInt("playamount", PlayerPrefs.GetInt("playamount") + 1);
        int playcount = PlayerPrefs.GetInt("playamount");
        string name = PlayerPrefs.GetString("username");
        string location = arenaname;
        string causeOfDeath = causeofdeath;
        statsarray[0] = playcount.ToString(); statsarray[1] = name; statsarray[2] = location; statsarray[3] = causeOfDeath;
        finalstr = '#' + statsarray[0] + "\n\n" + statsarray[1] + "\n\nPerished in:\n" + statsarray[2] + "\n\nCause of death:\n" + statsarray[3];

        PlayerPrefs.SetString("deathArray" + playcount, finalstr);

        PlayerPrefs.SetInt("dmgtaken", fpc.health.totaldmg);

        PlayerPrefs.Save();
    }

}
