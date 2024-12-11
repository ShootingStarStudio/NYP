using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class Script_baseAI : MonoBehaviour
{
    public NavMeshAgent agent;

    public Transform player;
    public Transform garbage;
    public LayerMask whatIsGround, whatIsPlayer;

    //Patroling
    public Vector3 walkPoint;
    bool walkPointSet;
    public float walkPointRange;

    public Vector3 flankpointA;
    bool flankpointASet;
    public Vector3 flankpointB;
    bool flankpointBSet;
    public Vector3 flankpointC;
    bool flankpointCSet;
    public int iteration = 0;

    //Attacking
    public float timeBetweenAttacks;
    bool alreadyAttacked;
    public GameObject projectile;

    //States
    public float sightRange, attackRange;
    public bool playerInSightRange, playerInAttackRange;
    public bool isStrafing;

    Script_baseFSM FSMScript;

    private void Awake()
    {
        player = GameObject.Find("FirstPersonController").transform;
        garbage = GameObject.Find("Garbage Container").transform;
        agent = GetComponent<NavMeshAgent>();
        FSMScript = GetComponent<Script_baseFSM>();
    }

    private void Update()
    {
        if (FSMScript.currentFSM != Script_baseFSM.FSM.IDLE)
        {
            //Check for sight and attack range
            playerInSightRange = Physics.CheckSphere(transform.position, sightRange, whatIsPlayer);
            playerInAttackRange = Physics.CheckSphere(transform.position, attackRange, whatIsPlayer);

            if (!playerInSightRange && !playerInAttackRange && FSMScript.OnVantage == false) Patroling();
            if (playerInSightRange && !playerInAttackRange && FSMScript.OnVantage == false && !FSMScript.IsFlanking) ChasePlayer();
            if (playerInSightRange && !playerInAttackRange && FSMScript.OnVantage == false && FSMScript.IsFlanking) EncirclePlayer();

            if (playerInAttackRange && playerInSightRange)
            {
                int random = Random.Range(0, 2);
                if (random == 1 && FSMScript.OnVantage == false)
                {
                    Retreat();
                }
                else
                {
                    AttackPlayer();
                }
            }
        }
    }

    private void Patroling()
    {
        FSMScript.currentFSM = Script_baseFSM.FSM.PATROL;
        if (!walkPointSet) SearchWalkPoint();

        if (walkPointSet)
            agent.SetDestination(walkPoint);

        Vector3 distanceToWalkPoint = transform.position - walkPoint;

        //Walkpoint reached
        if (distanceToWalkPoint.magnitude < 1f)
            walkPointSet = false;
    }
    private void SearchWalkPoint()
    {
        //Calculate random point in range
        float randomZ = Random.Range(-walkPointRange, walkPointRange);
        float randomX = Random.Range(-walkPointRange, walkPointRange);

        walkPoint = new Vector3(transform.position.x + randomX, transform.position.y, transform.position.z + randomZ);

        if (Physics.Raycast(walkPoint, -transform.up, 2f, whatIsGround))
            walkPointSet = true;
    }

    private void ChasePlayer()
    {
        FSMScript.currentFSM = Script_baseFSM.FSM.MOVE;
        agent.SetDestination(player.position);
    }

    // AI finds a alternate route to flank the player
    private void EncirclePlayer()
    {
        FSMScript.currentFSM = Script_baseFSM.FSM.ENCIRCLE;
        // iteration int
        iteration = 0;
        float randomZ = Random.Range(-walkPointRange, walkPointRange);
        float randomX = Random.Range(-walkPointRange, walkPointRange);

        // point nearest to enemy
        flankpointA = new Vector3(transform.position.x + randomX, transform.position.y, transform.position.z + randomZ);
        if (Physics.Raycast(flankpointA, flankpointA - transform.position, 100.0f, whatIsGround) &&
            !Physics.Raycast(flankpointA, player.transform.position - flankpointA, 100.0f, whatIsGround))
            flankpointASet = true;
        // point nearest to player
        flankpointB = new Vector3(player.transform.position.x + randomX, player.transform.position.y, player.transform.position.z + randomZ);
        if (!Physics.Raycast(flankpointB, flankpointB - transform.position, 100.0f, whatIsGround) &&
            Physics.Raycast(flankpointB, player.transform.position - flankpointB, 100.0f, whatIsGround))
            flankpointBSet = true;
        // point in between the two points above
        flankpointC = new Vector3(flankpointA.x - flankpointB.x, flankpointA.y - flankpointB.y, flankpointA.z - flankpointB.z);
        if (Physics.Raycast(flankpointC, flankpointC - flankpointA, 100.0f, whatIsGround) &&
            Physics.Raycast(flankpointC, flankpointC - flankpointB, 100.0f, whatIsGround) &&
            !Physics.Raycast(flankpointC, flankpointC - transform.position, 100.0f, whatIsGround) &&
            !Physics.Raycast(flankpointC, flankpointC - player.position, 100.0f, whatIsGround))
            flankpointCSet = true;

        // goes towards each waypoint to flank player

        Vector3[] FlankArray = new[] { flankpointA, flankpointC, flankpointB, player.position };
        var dist = Vector3.Distance(FlankArray[iteration], transform.position);

        if (dist < 5)
        {
            for (int i = 0; i < FlankArray.Length; i++)
            {
                if (iteration < FlankArray.Length - 1)
                {
                    iteration++;
                    agent.SetDestination(FlankArray[iteration]);
                }
            }
        }


    }

    // AI retreats/backs up and not engage the player instead of attacking using the same attack cooldown
    private void Retreat()
    {
        if (!alreadyAttacked)
        {
            FSMScript.currentFSM = Script_baseFSM.FSM.RETREAT;
            agent.SetDestination(transform.position - transform.forward);
            alreadyAttacked = true;
            Invoke(nameof(ResetAttack), timeBetweenAttacks);
        }
    }

    // For Archers if they are on vantage point, their isStrafing variable is off and they stand on the vantage point and shoot, else when they take damage they move off the tower
    // and strafe assault the player

    // For melee enemies they will have different states including rushing towards the player, encircling the player, flanking, tactical retreat

    private void AttackPlayer()
    {
        if (FSMScript.IsMelee && !alreadyAttacked)
        {
            FSMScript.currentFSM = Script_baseFSM.FSM.ATTACK;
            transform.LookAt(player);
            gameObject.transform.GetChild(0).gameObject.GetComponent<Script_baseWeapon>().Attack();

            if (gameObject.transform.GetChild(1).gameObject.GetComponent<Script_baseWeapon>() != null)
            {
                gameObject.transform.GetChild(1).gameObject.GetComponent<Script_baseWeapon>().Attack();
            }
            alreadyAttacked = true;
            Invoke(nameof(ResetAttack), timeBetweenAttacks);
        }

        else if (FSMScript.IsRanged)
        {
            FSMScript.currentFSM = Script_baseFSM.FSM.ATTACK;
            transform.LookAt(player);

            if (!alreadyAttacked)
            {
                // Ranged Code Here
                // Archer is able to strafe and is not on an archer tower
                if (isStrafing && FSMScript.OnVantage == false)
                {
                    if (!walkPointSet) SearchWalkPoint();

                    if (walkPointSet)
                        agent.SetDestination(walkPoint);

                    Vector3 distanceToWalkPoint = transform.position - walkPoint;

                    //Walkpoint reached
                    if (distanceToWalkPoint.magnitude < 5f)
                        walkPointSet = false;
                }
                else
                {
                    // Archer stands in place
                    agent.SetDestination(transform.position);
                    int random = Random.Range(0, 5);
                    if (random == 1)
                    {
                        FSMScript.OnVantage = false;
                    }
                }
                Quaternion rotation = Quaternion.Euler(transform.rotation.x, transform.rotation.y, transform.rotation.z);
                ///Attack code here
                Rigidbody rb = Instantiate(projectile, transform.position + (transform.forward * 2) + (transform.up * 0.3f), Quaternion.Euler(transform.forward),garbage).GetComponent<Rigidbody>();
                //Debug.Log(transform.position);
                rb.rotation = (Quaternion.Euler(transform.forward));
                rb.AddForce(transform.forward * 32f, ForceMode.Impulse); // forward force of projectile
                rb.AddForce(transform.up * 8f, ForceMode.Impulse); // upward force of projectile
    
                ///End of attack code

                alreadyAttacked = true;
                Invoke(nameof(ResetAttack), timeBetweenAttacks);
            }
        }
    }
    private void ResetAttack()
    {
        alreadyAttacked = false;
    }
    private void OnDrawGizmosSelected()
    {
        Gizmos.color = Color.red;
        Gizmos.DrawWireSphere(transform.position, attackRange);
        Gizmos.color = Color.yellow;
        Gizmos.DrawWireSphere(transform.position, sightRange);
    }
}