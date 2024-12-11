using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.SceneManagement;

public class CubeController : MonoBehaviour
{
    public float dragThreshold = 0.2f; // Minimum drag distance for a valid move
    public float dotThreshold = 0.8f; // Minimum dot product value for a valid axis

    public GameObject[] rayCastParent;

    public Transform pieceParent;

    public float castDist = 5f;

    public int initialShuffle = 0;

    RaycastHit hit;

    private void Awake()
    {
        Shuffle(initialShuffle);
    }

    public void RotateFace(int faceIndex, bool clockwise)
    {
        Debug.Log(faceIndex);
        Debug.Log(clockwise ? "clockwise" : "anticlockwise");
        // check the rotation axis of the cube
        Vector3 rotationAxis = Vector3.zero;
        switch (faceIndex)
        {
            default:
            case 0:
                rotationAxis = new Vector3(0, 0, -90);
                break;
            case 1:
                rotationAxis = new Vector3(0, 0, 90);
                break;
            case 2:
                rotationAxis = new Vector3(90, 0, 0);
                break;
            case 3:
                rotationAxis = new Vector3(-90, 0, 0);
                break;
            case 4:
                rotationAxis = new Vector3(0, 90, 0);
                break;
            case 5:
                rotationAxis = new Vector3(0, -90, 0);
                break;
        }
        if (!clockwise) rotationAxis *= -1;

        // get reference to center piece in face
        Transform centerRay = rayCastParent[faceIndex].transform;
        GameObject center = pieceParent.GetChild(faceIndex).gameObject;
        // add all the children in the raycast list
        foreach (Transform childRay in centerRay)
        {
            if (Physics.Raycast(childRay.position, childRay.forward, out hit, castDist)) hit.transform.parent = center.transform;
        }
        // get current rotation
        Quaternion currRotation = center.transform.rotation;
        // Rotate the selected face
        center.transform.rotation = Quaternion.Euler((int)currRotation.x + rotationAxis.x,
                                                          (int)currRotation.y + rotationAxis.y,
                                                          (int)currRotation.z + rotationAxis.z);
        // return the children back to pieces
        foreach (Transform childRay in centerRay)
        {
            if (Physics.Raycast(childRay.position, childRay.forward, out hit, castDist)) hit.transform.parent = pieceParent.transform;
        }
    }

    public bool IsCubeSolved()
    {
        for (int i = 0; i < 6; i++)
        {
            Transform centerRay = rayCastParent[i].transform;
            string centerFace = pieceParent.transform.GetChild(i).GetComponent<PieceData>().originFace;
            foreach (Transform childRay in centerRay)
            {
                if (childRay.name == "Face")
                {
                    if (Physics.Raycast(childRay.position, childRay.forward, out hit, castDist))
                    {
                        if (hit.transform.GetComponent<PieceData>().originFace != centerFace) return false;
                    }

                }
            }
        }
        return true;
    }

    private void Update()
    {
        foreach (GameObject child in rayCastParent)
        { 
            foreach (Transform ray in child.transform)
            {
                Debug.DrawRay(ray.position, ray.forward * castDist, Color.white);
            }
        }
        if (IsCubeSolved()) FindAnyObjectByType<TimeManager>().timeState = false;           //stop timer

        //if (Random.Range(0, 1000) > 900) Shuffle(1);
    }

    public void Shuffle(int turns)
    {
        for (int i = 0; i < turns; i++)
        {
            int randomFace = Random.Range(0, 6);
            bool randomDirection = Random.Range(0, 2) == 0 ? true : false;
            RotateFace(randomFace, randomDirection);
        }
    }

    public void Solve()
    {
        //solve code here
    }
}
