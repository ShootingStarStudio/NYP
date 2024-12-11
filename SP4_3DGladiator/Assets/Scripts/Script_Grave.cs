using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class Script_Grave : MonoBehaviour
{
    GameObject gravestone;
    public int ID;
    [SerializeField]
    TextMeshPro Gravetext;
   // [SerializeField]
    //Script_Endscene_get db;
    // Start is called before the first frame update
    void Start()
    {
        //db = gameObject.transform.parent.GetComponent<Script_Endscene_get>();
        //Gravetext = GetComponent<TextMeshPro>();
        //ID = gameObject.transform.parent.GetComponent<Script_Endscene_get>().graveamoun
        string final = PlayerPrefs.GetString("deathArray" + ID);
        Gravetext.text = final;
        Debug.Log(ID);
        Debug.Log(final);
    }

    // Update is called once per frame
    void Update()
    {

    }
}
