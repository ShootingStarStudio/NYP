using System;
using UnityEngine;

public class BuildMode : MonoBehaviour
{
    public GameObject buildPlane;      //grid plate
    private GameObject buildLot;

    public Item.ITEMID itemID = Item.ITEMID.Dirt;
    string[] ItemName = System.Enum.GetNames(typeof(Item.ITEMID));

    private ToolBarManager toolBar;

    private void Awake()
    {
        toolBar = FindAnyObjectByType<ToolBarManager>();
    }

    void Update()
    {
        if (Input.touchCount > 0 && Input.touches[0].phase == TouchPhase.Began && buildLot == null)
        {
            //create a new build plane 
            buildLot = Instantiate(buildPlane, transform.position, Quaternion.identity);
            buildLot.transform.localScale = gameObject.transform.localScale;
        }
    }

    public void setItemID(Item.ITEMID ID)
    {
        itemID = ID;
    }

    public void buildBlock()
    {
        //cast ray
        Ray ray = Camera.main.ScreenPointToRay(new Vector2(Screen.width / 2, Screen.height / 2));
        RaycastHit hit;
        if (Physics.Raycast(ray, out hit) && toolBar.checkItem(itemID) != 0)      //check if hit, then if buildlot, then if theres stuff left to build
        {
            if (hit.transform.name != "ResourceBoulder" && hit.transform.name != "ResourceMossyDirt" && hit.transform.name != "ResourceTree")
            {
                Vector3 offsetVector = Vector3.zero;        //0,0,0
                                                            //check direction of placement
                float minX = -0.5f * transform.localScale.x, minY = -0.5f * transform.localScale.y, minZ = -0.5f * transform.localScale.z;
                float maxX = 0.5f * transform.localScale.x, maxY = 0.5f * transform.localScale.y, maxZ = 0.5f * transform.localScale.z;
                float[] collisionBoundMin = { minX, minY, minZ };
                float[] collisionBoundMax = { maxX, maxY, maxZ };

                //higher chance to interact with sides than top and bottom
                if (hit.point.z <= hit.transform.position.z + collisionBoundMin[2]) offsetVector = new Vector3(0, 0, minZ * 2);
                else if (hit.point.z >= hit.transform.position.z + collisionBoundMax[2]) offsetVector = new Vector3(0, 0, maxZ * 2);

                else if (hit.point.x <= hit.transform.position.x + collisionBoundMin[0]) offsetVector = new Vector3(minX * 2, 0, 0);
                else if (hit.point.x >= hit.transform.position.x + collisionBoundMax[0]) offsetVector = new Vector3(maxX * 2, 0, 0);

                else if (hit.point.y <= hit.transform.position.y + collisionBoundMin[1]) offsetVector = new Vector3(0, minY * 2, 0);
                else if (hit.point.y >= hit.transform.position.y + collisionBoundMax[1]) offsetVector = new Vector3(0, maxY * 2, 0);

                //spawn object with offset
                toolBar.addItem(itemID, -1);
                GameObject obj = Instantiate(Resources.Load<GameObject>(itemID.ToString()), buildLot.transform);        //parent object, should allow for saving
                obj.transform.position = hit.transform.position + offsetVector;
            }
        }
    }

    public void breakBlock()
    {
        //cast ray
        Ray ray = Camera.main.ScreenPointToRay(new Vector2(Screen.width / 2, Screen.height / 2));
        RaycastHit hit;
        if (Physics.Raycast(ray, out hit))
        {
            if (hit.transform.name == "ResourceBoulder")
            {
                toolBar.addItem(Item.ITEMID.Cobblestone, 8);
                toolBar.addItem(Item.ITEMID.Stone, 2);
            }
            else if (hit.transform.name == "ResourceMossyDirt")
            {
                toolBar.addItem(Item.ITEMID.Dirt, 4);
                toolBar.addItem(Item.ITEMID.Grass, 2);
            }
            else if (hit.transform.name == "ResourceTree")
            {
                toolBar.addItem(Item.ITEMID.Plank, 8);
                toolBar.addItem(Item.ITEMID.Log, 2);
                toolBar.addItem(Item.ITEMID.Leaves, 8);
            }
            else
            {
                //individual items
                foreach (Item.ITEMID ID in Enum.GetValues(typeof(Item.ITEMID)))
                {
                    if (hit.transform.GetComponent<Block>().ID == ID)
                    {
                        //add item into inventory
                        toolBar.addItem(ID, 1);
                        //destroy
                        Destroy(hit.transform.gameObject);
                        break;
                    }
                }
            }
        }
    }

}
