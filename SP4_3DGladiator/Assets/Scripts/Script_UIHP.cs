using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Script_UIHP : MonoBehaviour
{

    public float content = 100;
    private float max = 100;
    private Image bar;
    [SerializeField]
    FirstPersonController fpc;
    Script_baseHealth health;


    // Start is called before the first frame update
    void Start()
    {
        bar = GetComponent<Image>();
        health = fpc.GetComponent<Script_baseHealth>();
        //Script_baseHealth basehp = fpc.GetComponent<Script_baseHealth>();
        //max = fpc.max_hp;

    }

    // Update is called once per frame
    void Update()
    {
        content = (float)health.getHealth();
        //Debug.Log("conten: " + content + "/max: " + max + "/conten/max: " + content/max);
        bar.transform.localScale = new Vector3(content / max, 1f, 1f);
    }
}
