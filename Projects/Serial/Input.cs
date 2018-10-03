using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;

public class Input : MonoBehaviour
{

    public string aPortName;  // Set in the Inspector after checking
    public int baud = 9600;
    SerialPort stream;

    [SerializeField]
    private Hole[] holes; // Set in inspector

    // Use this for initialization
    void Start()
    {
        stream = new SerialPort(aPortName, baud);
        stream.ReadTimeout = 500;
        stream.Open(); // Open serial stream
        // Write '1' to confirm connection
        stream.Write("1");
    }

    // Update is called once per frame
    void Update()
    {
        try
        {
			// get the input from the Arduino
            string value = stream.ReadLine();
            //Debug.Log(value);
			
			// check if data sent working correctly
            if (value == "0")
            {
            }
            else
            {
				// convert input to char array
                char[] buttons = value.ToCharArray();
				
				// set the hitting of each of the holes by the input
                for (int i = 0; i < 6; i++)
                {
                    if (buttons[i] == '1')
                    {
                        holes[i].Hitting = true;
                    }
                    else
                    {
                        holes[i].Hitting = false;
                    }
                }
            }
			// tell arduino to keep going
            stream.Write("1"); 
        }
        catch (Exception e)
        {
            // no op
        }

    }
}