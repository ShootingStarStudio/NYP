using System;
using UnityEngine;
using UnityEngine.AI;

public class Script_baseFSM : MonoBehaviour
{
    Script_baseHealth baseHealth;

    public FSM currentFSM = 0;
    int iFSMCounter = 0;
    public const float iMaxFSMCounter = 60f;       //in seconds
    public int iStunCounter = 3; // time to get stunned
    //after 1 min ends, stop chase and wait for other enemies to spawn and reset self FSM
    Script_baseAI ScriptAI;

    private NavMeshAgent navMeshAgent;
    public Transform TargetPos;
    public bool IsMelee; // boolean to check whether the enemy is a melee type
    public bool IsRanged; // boolean to check whether the enemy is a ranged type
    public bool OnVantage; // boolean to check whether the enemy is on the vantage point
    public bool IsStunned; // booleN to check whether the enemy is stunned or not
    public bool IsFlanking;
    public GameObject ParentArcherTower;
    public Transform garbage;

    private int lastHP;
    //public AudioClip hit;
    //public AudioClip death;
  //  private AudioSource source;

    public int dropRate = 25;

    public enum FSM
    {
        INACTIVE,
        IDLE = 0,           //check for player
        PATROL,             //"melee": roam spawn point
        MOVE,               //found target, moving to target
        ATTACK,             //target in range, attack
        RETREAT,            //fall back distance, regroup and reorganise
        ENCIRCLE,           //player found, going to surround player
        VANTAGE,            //"ranged": keep target in range, go to higher point, 
        DEATH
    }

    // Start is called before the first frame update
    private void Awake()
    {
        baseHealth = GetComponent<Script_baseHealth>();
        garbage = GameObject.Find("Garbage Container").transform;
        navMeshAgent = GetComponent<NavMeshAgent>();
        lastHP = baseHealth.maxHealth;
        //source = GetComponent<AudioSource>();
    }

    // Update is called once per frame
    void Update()
    {
        //navMeshAgent.destination = TargetPos.position;
        //Debug.Log(FSMState);
      //  Debug.LogWarning("hp " + baseHealth.getHealth() + "   lasthp " + lastHP);
        if (baseHealth.getHealth() <= 0)
        {
            //AudioSource.PlayClipAtPoint(death, new Vector3(gameObject.transform.position.x, gameObject.transform.position.y, gameObject.transform.position.z));
            currentFSM = FSM.DEATH;
        }
        else if (baseHealth.getHealth() < lastHP)
        {
            lastHP = baseHealth.getHealth();
            transform.GetChild(2).gameObject.GetComponent<Script_enemy_sounds>().hitsound();
            Debug.LogWarning("enemy hit");
        }
        else
        {
            currentFSM = FSM.ATTACK;
        }

        switch (currentFSM){
            case FSM.IDLE:
                {
                    break;
                }
            case FSM.PATROL:
                {
                    if (IsMelee)
                    {

                    }
                    break;
                }
            case FSM.MOVE:
                {
                    break;
                }
            case FSM.ATTACK:
                {
                    if (IsMelee)
                    {

                    }

                    else if (IsRanged)
                    {
                        //ScriptAI.isStrafing = true;
                    }
                    break;
                }
            case FSM.RETREAT:
                {
                    break;
                }
            case FSM.ENCIRCLE:
                {
                    break;
                }
            case FSM.VANTAGE:
                {
                    if (baseHealth.getHealth() != 100 && IsRanged) // if enemy is an archer and isn't max health 
                    {
                        // get out of vantage point to flank/attack player on ground
                        //after getting out of vantage point (dropping to ground), enemy goes to attack
                        OnVantage = false;
                    }
                    else if (baseHealth.getHealth() == 100)
                    {
                        // stay at vantage point and attack from range
                        OnVantage = true;
                    }
                    break;
                }
            case FSM.DEATH:
                {
                    //drop weapon
                    GameObject Clone;

                    for (int i = 0; i < gameObject.transform.childCount; i++)
                    {
                        if (gameObject.transform.GetChild(i).gameObject.tag == "Weapon")
                        {
                            Clone = Instantiate(transform.GetChild(i).gameObject, transform.position, Quaternion.identity, garbage);
                            Clone.name = transform.GetChild(i).gameObject.name;
                            Clone.GetComponent<Rigidbody>().constraints = RigidbodyConstraints.None;
                            Clone.GetComponent<MeshCollider>().enabled = true;
                            Clone.GetComponent<Animator>().enabled = false;
                        }
                        if (gameObject.transform.GetChild(i).gameObject.tag == "Healing")
                        {
                            if (UnityEngine.Random.Range(0, 100) <= dropRate)
                            {
                                //drop other
                                GameObject Other = Instantiate(transform.GetChild(i).gameObject, transform.position, Quaternion.identity, garbage);
                                Other.name = transform.GetChild(i).gameObject.name;
                                Other.GetComponent<Rigidbody>().constraints = RigidbodyConstraints.None;
                            }
                        }
                        if (gameObject.transform.GetChild(i).gameObject.tag == "soundgib")
                        {
                            Clone = Instantiate(transform.GetChild(i).gameObject, transform.position, Quaternion.identity);
                            Clone.GetComponent<Script_GIbs>().vanishing = true;
                            Clone.name = transform.GetChild(i).gameObject.name;
                            Clone.GetComponent<Rigidbody>().constraints = RigidbodyConstraints.None;
                            Clone.GetComponent<Script_enemy_sounds>().deadsound();
                        }
                        if (gameObject.transform.GetChild(i).gameObject.tag == "Untagged")
                        {
                            Clone = Instantiate(transform.GetChild(i).gameObject, transform.position, Quaternion.identity);
                            Clone.GetComponent<Script_GIbs>().vanishing = true;
                            Clone.name = transform.GetChild(i).gameObject.name;
                            Clone.GetComponent<Rigidbody>().constraints = RigidbodyConstraints.None;

                        }

                    }

                    //destroy self
                    Destroy(gameObject);
                    break;
                }
            default:
                {
                    break;
                }
        };


    }

    public void resetFSM()
    {
        currentFSM = FSM.INACTIVE;
        iFSMCounter = 0;
    }
}
