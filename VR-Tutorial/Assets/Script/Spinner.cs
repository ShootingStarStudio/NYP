using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Spinner : MonoBehaviour
{
    //when clicked, rotate the parent object by its clockwise or anticlockwise
    public int face;
    public bool clockwise;

    public Material InactiveMaterial, GazedAtMaterial;

    public void OnPointerEnter()
    {
        SetMaterial(true);
    }

    public void OnPointerExit()
    {
        SetMaterial(false);
    }

    public void OnPointerClick()
    {
        //get its parent's parent to rotate
        transform.parent.parent.GetComponent<CubeController>().RotateFace(face, clockwise);
    }

    private void SetMaterial(bool gazedAt)          //change color of all children
    {
        if (InactiveMaterial != null && GazedAtMaterial != null)
        {
            foreach (Transform child in transform)
            {
                child.GetComponent<Renderer>().material = gazedAt ? GazedAtMaterial : InactiveMaterial;
            }
        }
    }
}
