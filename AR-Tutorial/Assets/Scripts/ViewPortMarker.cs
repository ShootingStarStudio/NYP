using System.Collections.Generic;
using UnityEngine;
using UnityEngine.XR.ARFoundation;
using UnityEngine.XR.ARSubsystems;

public class ViewPortMarker : MonoBehaviour
{
    private ARRaycastManager rayManager;
    private ARPlaneManager planeManager;
    private GameObject plane;
    List<ARRaycastHit> hits = new List<ARRaycastHit>();

    // Start is called before the first frame update
    void Start()
    {
        rayManager = FindObjectOfType<ARRaycastManager>();
        planeManager = FindObjectOfType<ARPlaneManager>();
        plane = transform.GetChild(0).gameObject;
    }

    // Update is called once per frame
    void Update()
    {
        if (rayManager.Raycast(new Vector2(Screen.width / 2, Screen.height / 2),
            hits, TrackableType.Planes))
        {
            //move the plane to this location
            transform.position = hits[0].pose.position;
            transform.rotation = hits[0].pose.rotation;
            plane.SetActive(true);
        }
        else plane.SetActive(false);

    }
}
