using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.AI;

public class Script_ArenaHandler : MonoBehaviour
{
    Script_Time TimeInstance;

    public GameObject Teleport_TriggerStart, Teleport_TriggerEnd;

    private Script_Teleport TeleportStart, TeleportEnd;

    //if start is triggered, handler is active and timer is set
    //when player clears the level, activate the end
    //the end is then referenced by the next arena to start the arena

    public float waveTimer = 60f;
    private float arenaTime;
    //boss instance
    private bool spawnedBoss = false;           //ensure 1 boss is spawned

    public GameObject prefab_NPC;
    public GameObject prefab_NPC_SHIELD;
    public GameObject prefab_NPC_SPEAR;
    public GameObject prefab_NPC_BOW;

    public int[] numEnemyPerWave;

    public bool toSpawnBoss = false;
    public GameObject prefab_Boss;
    private GameObject bossPtr = null;          //if dead, then level ends

    private float time_Offset = 0f;

    public bool hasArcherTower = false;
    public GameObject[] archerTowerPos;

  
    //private int countNPC;                       //if 0, then all NPC dead, level ends
    // Start is called before the first frame update
    private void Awake()
    {
        TimeInstance = GetComponent<Script_Time>();
        TeleportStart = Teleport_TriggerStart.GetComponent<Script_Teleport>();
        TeleportEnd = Teleport_TriggerEnd.GetComponent<Script_Teleport>();
        arenaTime = numEnemyPerWave.Length * waveTimer + 2;
    }

    // Update is called once per frame
    void Update()
    {
        //teleporting in and out of arena and boss checking
        {
            if (TeleportStart.teleported)               //entered arena
            {
                TimeInstance.setTime(arenaTime);
                spawnedBoss = false;
                TimeInstance.ActivateTime(true);
            }
            if (TeleportEnd.teleported)                 //left arena
            {
                TimeInstance.setTime(arenaTime);        //reset time
                spawnedBoss = false;
            }

            GameObject[] enemies = GameObject.FindGameObjectsWithTag("Enemy");

            //if timer is done and all enemies are dead
            if (!TimeInstance.getState() && enemies.Length == 0)
            {
                if (!toSpawnBoss)                                    // for no boss
                {
                    TeleportEnd.gameObject.SetActive(true);
                }
                else if (toSpawnBoss && spawnedBoss && bossPtr != null)        // for boss check
                {
                    TeleportEnd.gameObject.SetActive(true);
                }
                else
                {
                    //null
                }
            }
            else
            {
                TeleportEnd.gameObject.SetActive(false);
            }
        }

        //npc spawning
        if (time_Offset > 0) time_Offset -= Time.deltaTime;
        else time_Offset = 0;

        if ((int)TimeInstance.timeRemaining % waveTimer == 1 && time_Offset <= 0)
        {
            time_Offset = 5f;
            int waveCount = numEnemyPerWave.Length - (int)(TimeInstance.timeRemaining/waveTimer);          //inverting time into wave count
            //Debug.Log("Num Ene: " + numEnemyPerWave[waveCount]);

            //last wave and boss is not spawned
            if (toSpawnBoss)  //check if the boss is meant to be spawned
            {
                if (numEnemyPerWave.Length == waveCount && bossPtr == null && !spawnedBoss)
                {
                    spawnedBoss = true;

                    bossPtr = Instantiate(prefab_Boss, gameObject.transform.position, Quaternion.identity);
                    bossPtr.gameObject.GetComponent<Script_baseAI>().enabled = true;
                    bossPtr.gameObject.GetComponent<Script_baseFSM>().enabled = true;
                    bossPtr.GetComponent<NavMeshAgent>().Warp(new Vector3(gameObject.transform.position.x, 17f, gameObject.transform.position.z - 55));     //spawn boss at gate

                    if (hasArcherTower)
                    {
                        SpawnArcherOnTower();
                    }
                }
            }
            
            Debug.Log("length of Number Per Wave: " + numEnemyPerWave.Length + " / Wave Count: " + waveCount + " / IsSpawnedBoss: " + spawnedBoss + " / bossPtr: " + bossPtr);
            if (!spawnedBoss)
            {
                for (int i = 0; i < numEnemyPerWave[waveCount]; i++)
                {
                    GameObject NPCToClone = prefab_NPC;
                    switch (Random.Range(0,4))
                    {
                        case 0: NPCToClone = prefab_NPC; break;
                        case 1: NPCToClone = prefab_NPC_SHIELD; break;
                        case 2: NPCToClone = prefab_NPC_SPEAR; break;
                        case 3: NPCToClone = prefab_NPC_BOW; break;
                        default: Debug.Log("AHHHHHHHHHHHHHHHHHHHHHHHH"); break;
                    }
                    GameObject NPCclone = Instantiate(NPCToClone, gameObject.transform.position, Quaternion.identity);
                    NPCclone.gameObject.GetComponent<Script_baseAI>().enabled = true;
                    NPCclone.gameObject.GetComponent<Script_baseFSM>().enabled = true;
                    int gateDirection = Random.Range(0, 4);
                    int randXOffset = 0, randZOffset = 0;
                    if (gateDirection == 0) randXOffset = 55;
                    else if (gateDirection == 1) randXOffset = -55;
                    else if (gateDirection == 2) randZOffset = 55;
                    else if (gateDirection == 3) randZOffset = -55;
                    NPCclone.GetComponent<NavMeshAgent>().Warp(new Vector3(gameObject.transform.position.x + randXOffset, 16f, gameObject.transform.position.z + randZOffset));
                    int random = Random.Range(0, 2);
                    if (random == 1) NPCclone.gameObject.GetComponent<Script_baseFSM>().IsFlanking = true;
                }

                if (hasArcherTower)
                {
                    SpawnArcherOnTower();
                }
            }
        }
    }

    public void SpawnArcherOnTower()
    {
        //instantiate a archer on each tower

        for (int i = 0; i < archerTowerPos.Length; i++)
        {
            GameObject NPCclone = Instantiate(prefab_NPC_BOW, gameObject.transform.position, Quaternion.identity);
            NPCclone.gameObject.GetComponent<Script_baseAI>().enabled = true;
            NPCclone.gameObject.GetComponent<Script_baseFSM>().enabled = true;
            NPCclone.gameObject.GetComponent<Script_baseFSM>().OnVantage = true;
            NPCclone.gameObject.GetComponent<Script_baseFSM>().ParentArcherTower = archerTowerPos[i];
            NPCclone.GetComponent<NavMeshAgent>().Warp(new Vector3(archerTowerPos[i].transform.position.x, 
                                                                    archerTowerPos[i].transform.position.y + 6, 
                                                                    archerTowerPos[i].transform.position.z));
        }
    }
}
