using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Script_Throwing : MonoBehaviour
{
    private LineRenderer lineRenderer;
    private const int LinePoints = 20;
    private const float TimeBetweenPoints = 0.1f;
    private const float projectileMass = 100f;

    private LayerMask SolidCollisionLayer;

    public GameObject SphereTargetPos;

    // Start is called before the first frame update
    void Start()
    {
        lineRenderer = gameObject.GetComponent<LineRenderer>();
    }

    private void Awake()
    {
        int selflayer = gameObject.layer;
        for (int i = 0; i < 32; i++)
        {
            if (!Physics.GetIgnoreLayerCollision(selflayer, i))
            {
                SolidCollisionLayer |= 1 << i;
            }
        }
    }

    // Update is called once per frame
    void Update()
    {

    }

    public void DrawTrajectoryProjection(Vector3 location, Vector3 direction, float _throwStrength)
    {
        lineRenderer.enabled = true;
        lineRenderer.positionCount = Mathf.CeilToInt(LinePoints / TimeBetweenPoints) + 1;
        Vector3 startPosition = location;
        Vector3 startVelocity = _throwStrength * direction / projectileMass;
        int i = 0;
        lineRenderer.SetPosition(i, startPosition);
        for (float time = 0; time < LinePoints; time += TimeBetweenPoints)
        {
            i++;
            Vector3 point = startPosition + time * startVelocity;
            point.y = startPosition.y + startVelocity.y * time + (Physics.gravity.y / 2f * time * time);
            lineRenderer.SetPosition(i, point);
            Vector3 lastPos = lineRenderer.GetPosition(i - 1);
            if (Physics.Raycast(lastPos, (point - lastPos).normalized, 
                out RaycastHit hit, (point - lastPos).magnitude, SolidCollisionLayer))
            {
                lineRenderer.SetPosition(i, hit.point);
                lineRenderer.positionCount = i + 1;

                SphereTargetPos.SetActive(true);
                SphereTargetPos.transform.position = hit.point;
                return;
            }
        }
    }
    public void StopTrajectoryDraw()
    {
        lineRenderer.enabled = false;
        SphereTargetPos.SetActive(false);
    }
}
