using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EventHandler : MonoBehaviour
{
    public GameObject ParentObject;
    public int EventCountdown = 30;
    public int EventDuration = 15;
    public bool GameStart = false;

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        // if player starts fighting and map is default
        if (ParentObject.name == "Default Arena")
        {
            //event here
        }

        // if player starts fighting and map is desert
        else if (ParentObject.name == "Desert Arena")
        {
            //event here
        }

        // if player starts fighting and map is forest
        else if (ParentObject.name == "Forest Arena")
        {
            //event here
        }

        // if player starts fighting and map is lava
        else if (ParentObject.name == "Volcanic Arena")
        {
            //event here
        }
    }
}
