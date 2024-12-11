using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Script_Quiver : MonoBehaviour
{
    
    [SerializeField]
    FirstPersonController fpc;
    [SerializeField]
    RawImage quiver;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if (fpc.arrows <= 0)
        {
            quiver.gameObject.SetActive(false);
          //  Debug.Log("have no arrow");
        }
        else
        {
            quiver.gameObject.SetActive(true);
          //  Debug.Log("Have arrow");
        }
            
    }
}
