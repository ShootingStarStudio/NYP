using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using TMPro;
public class Script_Mainmenu : MonoBehaviour
{
    public Animator anim;
    public Image black;
    public TMP_InputField nameinput;
    public string name = "nameless";
    // Start is called before the first frame update
    void Start()
    {

        Cursor.lockState = CursorLockMode.None;
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    public void GuestLogin()
    {
        name = nameinput.text;
        if (name == "") name = "nameless";
        PlayerPrefs.SetString("username",name);
        StartCoroutine(Fading("Scene_Battle"));
       // Debug.LogError("Loggin");
    }

    public void DoQuitGame()
    {
        Debug.Log("Clicked On quit game button");
        Application.Quit();
    }

    IEnumerator Fading(string sceneName)
    {
        anim.SetBool("fade", true);
        yield return new WaitUntil(() => black.color.a == 1);
        SceneManager.LoadScene(sceneName);
    }
}
