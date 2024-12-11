using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Script_InstantiateGraves : MonoBehaviour
{
    //Script_Endscene_get statgetter;
    [SerializeField] GameObject grave;
    public int graveamount;
    private Vector3 pos;
    private Quaternion rotation;
    // Start is called before the first frame update
    void Start()
    {
        pos.y = -6f;
        rotation.x = 0; rotation.y = 0; rotation.z = 0;
        graveamount = PlayerPrefs.GetInt("playamount");
        for(int i = 1; i < graveamount; i++)
        {
            pos.x = 1.2f * (i % 4);
            //z = i / 4 + 1;
            pos.z = 3 * (i / 4);
            Instantiate(grave, pos, rotation, gameObject.transform);
            
        }

    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
