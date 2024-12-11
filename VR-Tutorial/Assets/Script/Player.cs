using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Player : MonoBehaviour
{
    private GameObject _gazedAtObject = null;
    private float hoveringTime = 0f;
    private float hoveringTheshold = 3f;
    public float raycastDist = 10f;

    public CubeController currCube;

    public float dragThreshold = 0.2f; // Minimum drag distance for a valid move

    private Vector3 initialDragPosition, endDragPosition;

    private Transform cameraView;

    private void Awake()
    {
        cameraView = Camera.main.transform;
    }

    // Update is called once per frame
    void Update()
    {
        RaycastHit hit;
        if (Physics.Raycast(transform.position, cameraView.forward, out hit, raycastDist))
        {
            // Checks for screen touches.
            if (Google.XR.Cardboard.Api.IsTriggerPressed)       //1st frame
            {
                _gazedAtObject?.SendMessage("OnPointerClick");
                //teleport point code, obselete
                if (_gazedAtObject.CompareTag("TeleportPoint")) gameObject.transform.position = _gazedAtObject.transform.position + new Vector3(0, 1, 0);

                initialDragPosition = new Vector3(cameraView.rotation.x, cameraView.rotation.y, cameraView.rotation.z);

                //check face
                //if (Physics.Raycast(transform.position, cameraView.forward, out hit, raycastDist))
                //{
                //    int faceIndex = -1;
                //    float[] angles = new float[6];
                //    //check the direction of the player via the camera view forwards
                //    angles[0] = Vector3.Angle(cameraView.forward, gameObject.transform.forward);
                //    angles[1] = Vector3.Angle(cameraView.forward, -gameObject.transform.forward);
                //    angles[2] = Vector3.Angle(cameraView.forward, -gameObject.transform.right);
                //    angles[3] = Vector3.Angle(cameraView.forward, gameObject.transform.right);
                //    angles[4] = Vector3.Angle(cameraView.forward, -gameObject.transform.up);
                //    angles[5] = Vector3.Angle(cameraView.forward, gameObject.transform.up);
                //    float smallestAngle = angles.Min();
                //    for (faceIndex = 0; faceIndex < 6; faceIndex++)
                //    {
                //        if (angles[faceIndex] == smallestAngle) break;
                //    }
                     
                //    Debug.Log(faceIndex);
                //}
            }
            else if (Google.XR.Cardboard.Api.IsTriggerHeldPressed)      //constant down
            {
                endDragPosition = new Vector3(cameraView.rotation.x, cameraView.rotation.y, cameraView.rotation.z);
            }
            else
            {
                //end the dragging
                Vector3 dirVector = endDragPosition - initialDragPosition;

                float distance = dirVector.magnitude;
                if (distance > dragThreshold)
                {
                    // check valid drag
                }

                initialDragPosition = Vector3.zero;
                endDragPosition = Vector3.zero;
            }

            hoveringTime += Time.deltaTime;
            //hover code
            //select object using rycast for a period of time
            //then press click to move to it
            if (hoveringTime > hoveringTheshold)            //hovered
            {
                //hovered on the face for sometime
                //render valid movement choices as arrows
                //ignore for now
            }

            // GameObject detected in front of the camera.
            if (_gazedAtObject != hit.transform.gameObject)
            {
                // replace gazing object, reset variables
                _gazedAtObject?.SendMessage("OnPointerExit");
                _gazedAtObject = hit.transform.gameObject;
                _gazedAtObject.SendMessage("OnPointerEnter");
                hoveringTime = 0f;
            }

        }
        else
        {
            // No GameObject detected in front of the camera.
            _gazedAtObject?.SendMessage("OnPointerExit");
            _gazedAtObject = null;
        }

    }
    public void ResetScene()
    {
        SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex);
        Time.timeScale = 1f;
    }
}
