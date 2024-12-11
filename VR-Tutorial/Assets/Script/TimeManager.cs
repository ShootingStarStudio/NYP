using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class TimeManager : MonoBehaviour
{
    float currTime;
    TextMeshProUGUI textRenderer;
    int minutes, seconds;

    public bool timeState = true;

    private void Awake()
    {
        textRenderer = gameObject.GetComponent<TextMeshProUGUI>();
    }

    // Update is called once per frame
    void Update()
    {
        if (timeState)
        {
            currTime += Time.deltaTime;
            minutes = Mathf.FloorToInt(currTime / 60F);
            seconds = Mathf.FloorToInt(currTime - minutes * 60);
            textRenderer.text = string.Format("{0:0}:{1:00}", minutes, seconds);
        }
        else textRenderer.text = "Congratulations! Time Taken: " + string.Format("{0:0}:{1:00}", minutes, seconds);
    }
}
