using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Hole : MonoBehaviour {

    // Fields
    private bool hitting;								// is player hitting this hole
    [SerializeField]
    private GameObject mole;							// mole for hole
    [SerializeField]
    private GameObject bomb;							// bomb for hole

    private bool moleUp;								// is mole up?
    private bool bombUp;								// is bomb up?

    private int odds;									// current odds of pop up
    private const int MINODDS = 5;						// minimum odds of pop up

    private Vector3 up;									// up position
    private Vector3 down;								// down position

    private float hitCooldown;							// cooldown for hit
    private float objectCooldown;						// cooldown for object pop up
    private float objectCountdown;						// count down until object goes down
    private float maxObjectCountdown;					// max count down for object pop up
    private const float MINMAXOBJECTCOUNTDOWN = 1.5f;	// lowest count down for pop up
    private float maxObjectCooldown;					// max cooldown for object pop up
    private const float MINMAXOBJECTCOOLDOWN = 1.5f;	// lowest cooldown for object pop up
    private const float HITCOOLDOWN = 1.0f;				// time between player hits

    [SerializeField]
    private GameObject GameManager;						// set in inspector
    private GameManager manager;						// manager script on gameManager object

    // Properties
    public bool Hitting								
    {
        get { return hitting; }
        set { hitting = value; }
    }

	// Use this for initialization
	void Start () {
		// set inital states
        Reset();
		// get the manager
        manager = GameManager.GetComponent<GameManager>();
		
		// set up and down position
        up = new Vector3(transform.position.x, 0, transform.position.z);
        down = new Vector3(transform.position.x, -1.5f, transform.position.z);
		
		// face the camera
        Vector2 direction = new Vector2(manager.transform.position.x - transform.position.x, manager.transform.position.z - transform.position.z);
        float angle = Mathf.Atan2(direction.y, direction.x);
        transform.rotation *= Quaternion.Euler(0, angle, 0);
	}
	
	// Update is called once per frame
	void Update () {
		// code by gamestate
        switch (manager.GS)
        {
			// idle mode
            case global::GameManager.gameState.idle:
                // start the game on any button press
				if (hitting)
                {
					// start game and reset score
                    manager.GS = global::GameManager.gameState.game;
                    manager.Score = 0;
                }
                break;

			// game mode
            case global::GameManager.gameState.game:
                // Update Timers
                if (hitCooldown > 0)
                {
                    hitCooldown -= Time.deltaTime;
                }
                if (objectCooldown > 0)
                {
                    objectCooldown -= Time.deltaTime;
                }
                if (objectCountdown > 0)
                {
                    objectCountdown -= Time.deltaTime;
                }

                // Check hammer
                if (hitting && hitCooldown <= 0)
                {
					// check if mole hit
                    if (moleUp)
                    {
						// put the mole back
                        moleUp = false;
                        mole.transform.position = bomb.transform.position;
						
						// reset the countdown till new object
                        objectCountdown = 0;

						// reduce object cooldown
                        if (MINMAXOBJECTCOOLDOWN < maxObjectCooldown)
                        {
                            maxObjectCooldown *= .95f;
                        }
						// prevent from going past floor (MINMAXOBJECTCOOLDOWN)
                        else if (MINMAXOBJECTCOOLDOWN > maxObjectCooldown)
                        {
                            maxObjectCooldown = MINMAXOBJECTCOOLDOWN;
                        }

						// set object cooldown
                        objectCooldown = maxObjectCooldown;
						
						// increase odds of spawning an object
                        if (odds > MINODDS)
                        {
                            odds--;
                        }
						
						// increase score
                        manager.Score++;
                    }
					// check if bomb hit
                    else if (bombUp)
                    {
						// put the bomb back
                        bombUp = false;
						
						// reset the cooldown and countdown
                        objectCountdown = 0;
                        objectCooldown = maxObjectCooldown;
						
						// switch to reseting state (game over)
                        manager.GS = global::GameManager.gameState.reseting;
                        break;
                    }
					
					// start hit cooldown
                    hitCooldown = HITCOOLDOWN;
                }

                // Check 'up' objects timer
                if (moleUp && objectCountdown <= 0)
                {
					// put the mole back and set countdown
                    moleUp = false;
                    objectCountdown = 0;

					// reduce object cooldown
                    if (MINMAXOBJECTCOUNTDOWN < maxObjectCountdown)
                    {
                        maxObjectCountdown *= .95f;
                    }
					// prevent from going past floor (MINMAXOBJECTCOOLDOWN)
                    else if (MINMAXOBJECTCOUNTDOWN > maxObjectCountdown)
                    {
                        maxObjectCountdown = MINMAXOBJECTCOUNTDOWN;
                    }
					
					// set object cooldown
                    objectCooldown = maxObjectCooldown;
					
					// put the mole back
                    mole.transform.position = down;
                }
                else if (bombUp && objectCountdown <= 0)
                {
					// put the bomb back and set countdown
                    bombUp = false;
                    objectCountdown = 0;
                    bomb.transform.position = down;
                }

                // bring em up
                if (!bombUp && !moleUp && objectCooldown <= 0)
                {
					// determine what comes up by rng
                    int next = Random.Range(0, odds);
                    //Debug.Log(next);

                    switch (next)
                    {
                        case 0: // Mole
							// mark and move the mole up
                            moleUp = true;
                            mole.transform.position = up;
							
							// set the count down until mole goes away
                            objectCountdown = maxObjectCountdown;
							
							// speed up count down
                            maxObjectCooldown *= .95f;
							
							// increse odds of spawning
                            if (odds > MINODDS)
                            {
                                odds--;
                            }
                            break;
							
                        case 1: // Bomb
							// mark and move bomb up
                            bombUp = true;
                            bomb.transform.position = up;
							
							// set the count down until bomb goes away
                            objectCountdown = maxObjectCountdown;
							
							// speed up count down
                            maxObjectCooldown *= .95f;
							
							// increse odds of spawning
                            if (odds > MINODDS)
                            {
                                odds--;
                            }
                            break;
							
                        default: // Nothing this time
							// reset object cooldown
                            objectCooldown = maxObjectCooldown;
                            break;
                    }
                }
                break;

			// reset
            case global::GameManager.gameState.reseting:
				// check if not reset
				if (!manager.Ender[int.Parse(gameObject.name)])
				{
					// move all pop ups down
					mole.transform.position = down;
					bomb.transform.position = down;
				
					// reset for new game
					Reset();
				
					// tell the manager done reseting
					manager.Ender[int.Parse(gameObject.name)] = true;
				}
                break;
        }
	}

	// setup for new game
    private void Reset()
    {
		// set positions and pop ups to false
        hitting = false;
        moleUp = false;
        bombUp = false;

		// set up inital odds
        odds = 15;

		// set cooldown and count downs to inital states
        hitCooldown = 0;
        maxObjectCooldown = 5.0f;
        objectCooldown = maxObjectCooldown;
        maxObjectCountdown = 5.0f;
        objectCountdown = 0;
    }
}
