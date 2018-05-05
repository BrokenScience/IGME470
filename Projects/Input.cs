using System; //So we catch exceptions & ignore them
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports; //So we can do serial

public class Input : MonoBehaviour
{

    public string aPortName;  //Set in the Inspector after checking
    public int baud = 9600;
    SerialPort stream;

    [SerializeField]
    private Hole[] holes; // Set in inspector

    // Use this for initialization
    void Start()
    {
        stream = new SerialPort(aPortName, baud);
        stream.ReadTimeout = 500;
        stream.Open(); //Open serial stream
        //Behavior expected from Arduino's SerialCallResponseASCII sketch
        stream.Write("1");
    }

    // Update is called once per frame
    void Update()
    {
        try
        {
            string value = stream.ReadLine();
            Debug.Log(value);
            if (value == "0")
            {
            }
            else
            {
                char[] buttons = value.ToCharArray();
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
            stream.Write("1"); //tell arduino to keep 
        }
        catch (Exception e)
        {
            //no op
        }

    }
}