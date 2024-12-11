using UnityEngine;
using UnityEngine.UI;

public class ToolBarManager : MonoBehaviour
{
    public GameObject itemPrefab;
    public BuildMode BuildRef;

    private void Awake()
    {
        BuildRef = FindObjectOfType<BuildMode>();
    }

    public int checkItem(Item.ITEMID ID)
    {
        foreach (Transform item in transform)
        {
            if (item.GetComponent<Item>().ID == ID) return item.GetComponent<Item>().count;
        }
        return 0;
    }

    //functions as inventory manager
    public bool addItem(Item.ITEMID ID, int count)
    {
        //check if item exist
        foreach (Transform item in transform)
        {
            if (item.GetComponent<Item>().ID == ID)
            {
                item.GetComponent<Item>().count += count;
                return true;
            }
        }
        //item does not exist, add new child
        GameObject newItem = Instantiate(itemPrefab, transform);
        newItem.GetComponent<Item>().ID = ID;
        newItem.GetComponent<Item>().count = count;
        newItem.GetComponent<Item>().generate();
        return true;
    }
    private void removeItem(GameObject item)
    {
        //use only when item is empty, or forcibly remove items
        Destroy(item);
    }

    private void FixedUpdate()
    {
        foreach (Transform item in transform)
        {
            //remove if empty
            if (item.GetComponent<Item>().count <= 0) removeItem(item.gameObject);
        }
    }

}
