using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class Item : MonoBehaviour
{
    public enum ITEMID
    {
        //all the items in the list
        Dirt, Grass,
        Stone, Cobblestone,
        Log, Plank, Leaves,
        TotalID
    }

    public ITEMID ID = ITEMID.Dirt;
    public int count = 0;
    //ignore max item count, cause its an inventory list

    private void FixedUpdate()
    {
        transform.GetChild(1).GetComponent<TextMeshProUGUI>().text = count.ToString();
    }

    public void generate()
    {
        //assign texture
        transform.GetChild(0).GetComponent<RawImage>().texture = Resources.Load<Texture>(ID.ToString());
        //assign button listener
        gameObject.GetComponent<Button>().onClick.AddListener(() => {
            transform.parent.GetComponent<ToolBarManager>().BuildRef.setItemID(ID);
        });
    }
}
