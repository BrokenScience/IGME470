using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GameManager : MonoBehaviour {

    // Fields
    private bool[] ender;								// state of holes (game end/reset, in-game)
    private float coolOff;								// timer until new game can start
    private const float MAXCOOLOFF = 1f;				// max time until new game can start
    [SerializeField]
    private GameObject textHolder;						// to be set in inspector
    private Text text;									// text of textHolder gameObject
	
    private int score;									// player score

    public enum gameState { game, reseting, idle };		// gamestate
    [SerializeField]
    private gameState gs;								// current gamestate

    // Properties
    public bool[] Ender
    {
        get { return ender; }
        set { ender = value; }
    }

    public int Score
    {
        get { return score; }
        set { score = value; }
    }

    public gameState GS
    {
        get { return gs; }
        set { gs = value; }
    }

	// Use this for initialization
	void Start () {
		// set ender array
        ender = new bool[6];
        for (int i = 0; i < 6; i++)
        {
            ender[i] = false;
        }
		
		// get text for screen display
        text = textHolder.GetComponent<Text>();
        text.text = "Press any button to play";

		// set score, cooloff and gamestate to initial values
        score = 0;
        coolOff = MAXCOOLOFF;
        gs = gameState.idle;
	}
	
	// Update is called once per frame
	void Update () {
        switch (gs)
        {
            case gameState.idle:
                // no op
                break;
				
            case gameState.game:
				// display score
                text.text = "Score: " + score.ToString();
                break;
				
            case gameState.reseting:
				// check if all holes have finished reseting and cool off time has elapsed
                if (DoneReseting() && coolOff <= 0)
                {
					// set all enders back to false
                    for (int i = 0; i < ender.Length; i++)
                    {
                        ender[i] = false;
                    }
					// display score and final message
                    text.text = "Score: " + score + "!\nPress any button to play again!";
					
					// set cooloff
                    coolOff = MAXCOOLOFF;
					
					// set to idle state
					gs = gameState.idle;
                }
                else if (coolOff > 0)
                {
					// count down cooloff
                    coolOff -= Time.deltaTime;
                }
                break;
        }
	}

	// check if all holes are done reseting
    bool DoneReseting()
    {
		// if any are false, return false
        for (int i = 0; i < ender.Length; i++)
        {
            if (!ender[i])
            {
                return false;
            }
        }
        return true;
    }
}
