using TMPro;
using UnityEngine;

public class Script_Time : MonoBehaviour
{
    public float timeRemaining = 0f;        //in seconds
    private bool timeActive = false;

    private AudioSource audioSource;
    private bool hasRung = false;

    public TextMeshPro[] timeText;
    public int minutes, seconds;

    private void Awake()
    {
        audioSource = GetComponent<AudioSource>();
    }

    // Update is called once per frame
    void Update()
    {
        if (timeActive)
        {
            if (timeRemaining > 0)
            {
                timeRemaining -= Time.deltaTime;
            }
            else
            {
                Debug.Log("Time has run out!");
                timeRemaining = 0;
                timeActive = false;
                if (!hasRung)
                {
                    audioSource.Play();
                    hasRung = true;
                }
            }
        }
        DisplayTime(timeRemaining);
    }
    void DisplayTime(float timeToDisplay)
    {
        minutes = Mathf.FloorToInt(timeToDisplay / 60);
        seconds = Mathf.FloorToInt(timeToDisplay % 60);
        for (int i = 0; i < timeText.Length; i++) 
        {
            timeText[i].text = string.Format("{0:00}:{1:00}", minutes, seconds);
        }
    }
    public void setTime(float _time)
    {
        timeRemaining = _time;
    }

    public void ActivateTime(bool _state)
    {
        timeActive = _state;
    }

    public bool getState()
    {
        return timeActive;
    }
}
