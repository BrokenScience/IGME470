// first is 10
// last is 2 (near and far determined by distance to arduino nano in breadboard)
#include <avr/wdt.h>
const int MINZOOM = 500;              // Longest delay between movement
const int MAXZOOM = 25;               // Shortest delay between movement
const int MAXPIN = 10;                // Largest LED pin number
const int MINPIN = 2;                 // Smallest LED pin number
const float ZOOMRECTIFIER = 1.05;     // Rate at which to adjust speed
const long BUTTONCOOLDOWN = 5;        // Milisecond Max timer for button cool down
const int DELAYBASE = 150;            // Default delay time between LED phases
const int BLINK = 10;                 // Number of times to blink
int pin = MAXPIN + 1;                 // Current pin number
int zoom = MINZOOM;                   // Current speed (millisecond delay)
bool down = true;                     // Direction (down is true, up is false)
bool moarZoom = true;                 // Accelerate?
bool near = false;                    // State of near button this loop
bool prevNear = false;                // State of near button last loop
bool far = false;                     // State of far button this loop
bool prevFar = false;                 // State of far button last loop
long lastMil = 0;                     // Number of miliseconds past since start as of last loop
long mil = 0;                         // Number of miliseconds past since last loop
long timer = 0;                       // Timer in miliseconds for light movement
long nearTimer = 0;                   // Timer in miliseconds since near button pressed
long farTimer = 0;                    // Timer in miliseconds since far button pressed
bool idle = true;                     // Idle state? (Am I in the Idle state)
int scoreNear = 0;                    // Near side's score
int scoreFar = 0;                     // Far side's score
bool nearStart = true;                // Does the near side start this round (loser of previous round)

// This works
void (*reset)(void) = 0;              // reset the board by calling null function pointer (solution created with friend)

// setup code
void setup() {
  // Set output pins
  for (int i = MAXPIN; i >= MINPIN; i--)
  {
    pinMode(i, OUTPUT);
  }
  // begin debug serial
  //Serial.begin(9600);
}

// main loop
void loop() {
  // Time since last loop
  mil = millis() - lastMil;
  lastMil = millis();

  // Running timers update
  timer += mil;
  nearTimer += mil;
  farTimer += mil;

  // Convert analog input to a bool for button pressed
  // Check near button
  if (analogRead(A0) > 512)
  {
    // Check if both the cooldown timer and the button has not been held
    if (nearTimer >= BUTTONCOOLDOWN && !prevNear)
    {
      // Activate the button this loop
      near = true;
      nearTimer = 0;
    }
    else
    {
      // Do not activate the button this loop
      near = false;
    }
    // Button was pressed this loop
    prevNear = true;
  }
  else
  {
    // Button was not pressed this loop
    prevNear = false;
  }

  //Serial.print("prevFar: ");
  //Serial.println(prevFar);
  
  // Check far button
  if (analogRead(A1) > 512)
  {
    // Check if both the cooldown timer and the button has not been held
    if (farTimer >= BUTTONCOOLDOWN && !prevFar)
    {
      // Activate the button this loop
      far = true;
      farTimer = 0;
    }
    else
    {
      // Do not activate the button this loop
      far = false;
    }
    // Button was pressed this loop
    prevFar = true;
  }
  else
  {
    // Button was not pressed this loop
    prevFar = false;
  }
  //Serial.print("far: ");
  //Serial.println(far);
  
  //Serial.println(timer);

  // idle state code
  if (idle)
  {
    // check if idle state broken by button press
    if (near || far)
    {
      // begin game state
      idle = false;
      digitalWrite(pin,LOW);
      GameInit();
    }
    else
    {
      // Idle State
      // Determine if light needs to move and move it
      if (down && timer >= zoom)
      {
        //Serial.println("up");
        // turn off current pin
        digitalWrite(pin, LOW);
        
        // move light
        pin--;
        
        // check if bounced
        BounceCheckIdle();
        
        // light new pin
        digitalWrite(pin, HIGH);
        
        // reset light timer
        timer = 0;
      }
      else if (timer >= zoom)
      {
        //Serial.println("down");
        // turn off current pin
        digitalWrite(pin, LOW);
        
        // move light
        pin++;
        
        // check if bounced
        BounceCheckIdle();
        
        // light new pin
        digitalWrite(pin, HIGH);
        
        // reset light timer
        timer = 0;
      }
    }
  }
  else
  {
    // Game Phase
    // check if light should move
    if (down && timer >= zoom)
    {
      // turn off current light
      digitalWrite(pin, LOW);
      
      // move light
      pin--;
      
      // light new light
      digitalWrite(pin, HIGH);
      
      // reset light timer
      timer = 0;
    }
    else if (timer >= zoom)
    {
      // turn off current light
      digitalWrite(pin, LOW);
      
      // move light
      pin++;
      
      // light new light
      digitalWrite(pin, HIGH);
      
      // reset light timer
      timer = 0;
    }

    // check for bounces (and old debug statements)
    if (near)
    {
      //Serial.print("near ");
      if (down)
      {
        //Serial.print("down ");
      }
      else
      {
        //Serial.print("up   ");
      }
      //Serial.print(pin);
      // check if near side has bounced the light and bounce
      if (!down && pin >= MAXPIN - 1)
      {
        //Serial.println(" BOUNCE");
        BounceGame();
      }
      else
      {
        //Serial.println();
      }
    }
    if (far)
    {
      //Serial.print("far  ");
      if (down)
      {
        //Serial.print("down ");
      }
      else
      {
        //Serial.print("up   ");
      }
      //Serial.print(pin);
      // check if far side has bounced the light and bounce
      if (down && pin <= MINPIN + 1)
      {
        //Serial.println(" BOUNCE");
        BounceGame();
      }
      else
      {
        //Serial.println();
      }
    }

    //Serial.print("Pin: ");
    //Serial.println(pin);
    
    // Check for point end (loser starts)
    if (pin < MINPIN)
    {
      // Near gets a point and reset
      nearStart = false;
      ResetPoint();
      
      //digitalWrite(4, HIGH);
      //delay(100);
      //digitalWrite(4,LOW);
    }
    if (pin > MAXPIN)
    {
      // Far gets a point and reset
      nearStart = true;
      ResetPoint();
    }
  }
}

// Bounce the light in idle state
void SpeedIdle() {
  // if accelerating
  if (moarZoom)
  {
    // reduce time between light movement
    zoom /= ZOOMRECTIFIER;
    
    // limit speed at max
    if (zoom < MAXZOOM)
    {
      zoom = MAXZOOM;
      // slow down
      moarZoom = !moarZoom;
    }
  }
  else
  {
    // increase time bewteen light movement
    zoom = (zoom * ZOOMRECTIFIER) + 1;      // +1 to prevent stalling at fast speeds
    
    // limit speed at min
    if (zoom > MINZOOM)
    {
      zoom = MINZOOM;
      // speed up
      moarZoom = !moarZoom;
    }
  }
  //Serial.println(zoom);
}

// Check if light needs to be bounced
void BounceCheckIdle() {
  // if light is past the min, bounce
  if (pin < MINPIN)
  {
    // change direction
    down = false;
    // move light to bounced position
    pin = MINPIN + 1;
    // adjust speed
    SpeedIdle();
  }
  // if light is past the max, bounce
  else if (pin > MAXPIN)
  {
    // change direction
    down = true;
    // move light to bounced position
    pin = MAXPIN - 1;
    // adjust speed
    SpeedIdle();
  }
}

// Bounce the light IN THE GAME
void BounceGame() {
  // change direction
  down = !down;
  // turn current light off
  digitalWrite(pin, LOW);
  
  // move light depending on direction
  if (down)
  {
    pin--;
  }
  else
  {
    pin++;
  }
  
  // light up new light
  digitalWrite(pin, HIGH);
  
  // speed up
  zoom /= ZOOMRECTIFIER;
  
  // reset light movement timer
  timer = 0;
}

// Reset the point
void ResetPoint() {
  // Show previous score as constant lights from the ends
  for (int i = MAXPIN; i > MAXPIN - scoreNear; i--)
  {
    digitalWrite(i, HIGH);
  }
  for (int i = MINPIN; i < MINPIN + scoreFar; i++)
  {
    digitalWrite(i,HIGH);
  }

  // Determine who won and find light coresponding to that point
  int pointPin = 0;
  
  // if near lost, far gets point and gets flashing pin (& vice versa)
  if (nearStart)
  {
    scoreFar++;
    pointPin = MINPIN + scoreFar - 1;
  }
  else
  {
    scoreNear++;
    pointPin = MAXPIN - scoreNear + 1;
  }

  // Show Most Recent Point as flashing pin
  for (int i = 0; i < BLINK; i++)
  {
    digitalWrite(pointPin, LOW);
    delay(DELAYBASE);
    digitalWrite(pointPin, HIGH);
    delay(DELAYBASE);
  }
  
  // delay before reset
  delay(4 * DELAYBASE);

  // Determine Game end
  // near wins
  if (scoreNear >= 3)
  {
    //Serial.println("BEGIN NEAR WIN");
    // clear pins of far side
    for (int i = MINPIN; i < MINPIN + scoreFar; i++)
    {
      digitalWrite(i,LOW);
      delay(DELAYBASE);
    }
    //Serial.println("DOOTALY DOOTALY BIT");
    // winner celebration dance
    for (int i = 0; i < BLINK; i++)
    {
      digitalWrite(MAXPIN, HIGH);
      digitalWrite(MAXPIN - 1, LOW);
      digitalWrite(MAXPIN - 2, HIGH);
      delay(DELAYBASE);
      digitalWrite(MAXPIN, LOW);
      digitalWrite(MAXPIN - 1, HIGH);
      digitalWrite(MAXPIN - 2, LOW);
      delay(DELAYBASE);
    }
    //Serial.println("SHUT PINS OFF");
    // turn off winner pins
    for (int i = 0; i < 3; i++)
    {
      digitalWrite(MAXPIN - i, LOW);
    }
    //Serial.println("GAME ABOUT TO END");
    // end game
    GameEnd();
  }
  // if far wins
  else if (scoreFar >= 3)
  {
    // turn off near pins
    for (int i = MAXPIN; i > MAXPIN - scoreNear; i--)
    {
      digitalWrite(i,LOW);
      delay(DELAYBASE);
    }

    // winner celebration dance
    for (int i = 0; i < BLINK; i++)
    {
      digitalWrite(MINPIN, HIGH);
      digitalWrite(MINPIN + 1, LOW);
      digitalWrite(MINPIN + 2, HIGH);
      delay(DELAYBASE);
      digitalWrite(MINPIN, LOW);
      digitalWrite(MINPIN + 1, HIGH);
      digitalWrite(MINPIN + 2, LOW);
      delay(DELAYBASE);
    }
    
    // turn off winner pins
    for (int i = 0; i < 3; i++)
    {
      digitalWrite(MINPIN + i, LOW);
    }
    
    // end game
    GameEnd();
  }
  else
  {
    // Clear Score
    for (int i = MAXPIN; i > MAXPIN - scoreNear; i--)
    {
      digitalWrite(i, LOW);
    }
    for (int i = MINPIN; i < MINPIN + scoreFar; i++)
    {
      digitalWrite(i,LOW);
    }
    
    // pause before next point
    delay(DELAYBASE);

    // begin next point
    GameInit();
  }
}
  
// Initialize the game
void GameInit() {
  //Serial.println("GAME START");
  // Set up for next point
  pin = (MAXPIN - MINPIN) / 2 + MINPIN; // pin is center pin
  down = !nearStart;                    // direction is opposite of the winner
  zoom = MINZOOM;                       // set to slowest speed
  prevNear = false;                     // no buttons were pressed
  prevFar = false;                      // "
  timer = 0;                            // light movement timer reset
  nearTimer = 0;                        // near button timer reset
  near = false;                         // near button not pressed
  farTimer = 0;                         // far button timer reset
  far = false;                          // far button not pressed

  // Show starting light and have it blink before moving (to attract attention)
  for (int i = 0; i < BLINK; i++)
  {
    digitalWrite(pin, HIGH);
    delay(DELAYBASE);
    digitalWrite(pin,LOW);
    delay(DELAYBASE);
  }
  
  // set pin to on
  digitalWrite(pin,HIGH);
  
  // delay before starting
  delay(5 * DELAYBASE);
  //Serial.print("Down: ");
  //Serial.println(down);
}

// End the game and return to idle
void GameEnd(){
  //Serial.println("Game ENDING");
  // For some reason reseting all of the variables does not perfectly reset the game
  // Plan A
  //idle = true;
  //pin = MAXPIN + 1;
  //zoom = MINZOOM;
  //down = true;
  //moarZoom = true;
  //near = false;
  //prevNear = false;
  //far = false;
  //prevFar = false;
  //lastMil = 0;
  //mil = 0;
  //timer = 0;
  //nearTimer = 0;
  //farTimer = 0;
  //idle = true;
  //scoreNear = 0;
  //scoreFar = 0;
  //nearStart = true;
  //Serial.println("GAME ENDED");

  // Plan B: Nuke it
  reset();
}

