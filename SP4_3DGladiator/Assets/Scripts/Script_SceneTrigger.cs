using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
public class Script_SceneTrigger : MonoBehaviour
{
    // Start is called before the first frame update
    public Image black;
    public Animator anim;
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnTriggerEnter(Collider other)
    {

        StartCoroutine(change());
    }
    IEnumerator change()
    {
        anim.SetBool("fade", true);
        yield return new WaitUntil(() => black.color.a == 1);
        Cursor.lockState = CursorLockMode.None;
        SceneManager.LoadScene("Scene_Menu");
    }
}
