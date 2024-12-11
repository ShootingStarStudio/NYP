using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Script_SpecialDash : MonoBehaviour
{
    public GameObject player;
    // Start is called before the first frame update
    private void OnTriggerEnter(Collider other)
    {
        Debug.Log("dashtriggerenter");
        if (other.GetComponent<Script_baseHealth>() != null && other != player)
        {
            other.GetComponent<Script_baseHealth>().TakeDamage(50);

        }
        //if (other.gameObject.tag == "Player")
        //{
        //    Vector3 moveDir = other.transform.position - player.transform.position;
        //    other.GetComponent<Rigidbody>().AddForce(moveDir.normalized * 9000f);
        //}
    }

    private void OnTriggerStay(Collider other)
    {

    }

    // Update is called once per frame
    void Update()
    {

    }
}