using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UI;

public class Script_UIDirectiionalIndicator : MonoBehaviour
{
    private GameObject playerInstance;      //is set by parent

    public GameObject target;
    private Color color = Color.white;

    private float timeTotal = 2f;
    private float fadeTime = 1f;
    private float timeRemaining;

    private bool isObjective = false;
    private float distFade = 5f;

    public void setTarget(GameObject _target, bool _isObjective)
    {
        target = _target;
        isObjective = _isObjective;
        if (isObjective)
        {
            color = Color.white;
        }
        else
        {
            color = Color.red;
        }
    }

    private void Awake()
    {
        timeRemaining = timeTotal;
        playerInstance = gameObject.transform.parent.parent.parent.gameObject;
    }

    // Update is called once per frame
    private void Update()
    {
        Vector3 playerPos = playerInstance.transform.position;
        Vector3 playerForward = new Vector3(playerInstance.transform.forward.x, 0, playerInstance.transform.forward.z).normalized;
        Vector3 playerRight = new Vector3(playerInstance.transform.right.x, 0, playerInstance.transform.right.z).normalized;

        if (target != null)         //check if the target is active, if died then dont need to check
        {
            Vector3 targetPos = target.transform.position;
            Vector3 targetDir = playerPos - targetPos;
            Vector3 targetRot = new Vector3(targetDir.x, 0, targetDir.z).normalized;

            float dist = targetDir.magnitude;

            float angleRot = Vector3.Angle(playerForward, targetRot) - 180;
            if (Vector3.Angle(playerRight, targetRot) < 90f)       //on right side
            {
                gameObject.transform.rotation = Quaternion.Euler(0, 0, -angleRot);
            }
            else
            {
                gameObject.transform.rotation = Quaternion.Euler(0, 0, angleRot);
            }

            if (!isObjective)
            {
                timeRemaining -= Time.deltaTime;
                if (timeRemaining <= 0)
                {
                    Destroy(gameObject);
                }
                //only fade if the target is not an objective and within fade time
                if (timeRemaining <= fadeTime)
                {
                    gameObject.GetComponent<RawImage>().color = new Color(color.r, color.g, color.b, timeRemaining / fadeTime);
                }
            }
            else
            {
                //give objective a highlight glow

                if (dist <= distFade)           //objective fade when close
                {
                    gameObject.GetComponent<RawImage>().color = new Color(color.r, color.g, color.b, dist / distFade);
                }
            }
        }
        else
        {
            Destroy(gameObject);
        }
    }
}
