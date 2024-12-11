using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class VRInteraction : MonoBehaviour              //base class, duplicate here
{
    private Renderer _myRenderer;
    public Material InactiveMaterial;
    public Material GazedAtMaterial;

    private void Awake()
    {
        _myRenderer = GetComponent<Renderer>();
    }

    void Update()
    {
        
    }

    /// <summary>
    /// This method is called by the Main Camera when it starts gazing at this GameObject.
    /// </summary>
    public void OnPointerEnter()
    {
        SetMaterial(true);
    }

    /// <summary>
    /// This method is called by the Main Camera when it stops gazing at this GameObject.
    /// </summary>
    public void OnPointerExit()
    {
        SetMaterial(false);
    }

    /// <summary>
    /// This method is called by the Main Camera when it is gazing at this GameObject and the screen
    /// is touched.
    /// </summary>
    public void OnPointerClick()
    {

    }

    private void SetMaterial(bool gazedAt)
    {
        if (InactiveMaterial != null && GazedAtMaterial != null)
        {
            _myRenderer.material = gazedAt ? GazedAtMaterial : InactiveMaterial;
        }
    }
}
