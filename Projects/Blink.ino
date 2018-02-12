// first is 10
// last is 2
#include <avr/wdt.h>
const int MINZOOM = 500;
const int MAXZOOM = 25;
const int MAXPIN = 10;
const int MINPIN = 2;
const float ZOOMRECTIFIER = 1.05;
const long BUTTONCOOLDOWN = 1;
const int DELAYBASE = 150;
const int BLINK = 10;
int pin = MAXPIN + 1;
int zoom = MINZOOM;
bool down = true;
bool moarZoom = true;
bool near = false;
bool prevNear = false;
bool far = false;
bool prevFar = false;
long lastMil = 0;
long mil = 0;
long timer = 0;
long nearTimer = 0;
long farTimer = 0;
bool idle = true;
int scoreNear = 0;
int scoreFar = 0;
bool nearStart = true;

// This works
void (*reset_oh_yeah)(void) = -0;

void setup() {
  // put your setup code here, to run once:
  for (int i = MAXPIN; i >= MINPIN; i--)
  {
    pinMode(i, OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Time since last loop
  mil = millis() - lastMil;
  lastMil = millis();

  // Running timer
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

  if (idle)
  {
    if (near || far)
    {
      idle = false;
      digitalWrite(pin,LOW);
      GameInit();
    }
    else
    {
      // Idle Phase
      // Determine if light needs to move and move it
      if (down && timer >= zoom)
      {
        //Serial.println("up");
        digitalWrite(pin, LOW);
        pin--;
        BounceCheckIdle();
        digitalWrite(pin, HIGH);
        timer = 0;
      }
      else if (timer >= zoom)
      {
        //Serial.println("down");
        digitalWrite(pin, LOW);
        pin++;
        BounceCheckIdle();
        digitalWrite(pin, HIGH);
        timer = 0;
      }
    }
  }
  else
  {
    // Game Phase
    if (down && timer >= zoom)
    {
      digitalWrite(pin, LOW);
      pin--;
      digitalWrite(pin, HIGH);
      timer = 0;
    }
    else if (timer >= zoom)
    {
      digitalWrite(pin, LOW);
      pin++;
      digitalWrite(pin, HIGH);
      timer = 0;
    }

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
    // Check for point end
    //Serial.print("Pin: ");
    //Serial.println(pin);
    if (pin < MINPIN)
    {
      // Near gets a point
      nearStart = false;
      ResetPoint();
      //digitalWrite(4, HIGH);
      //delay(100);
      //digitalWrite(4,LOW);
    }
    if (pin > MAXPIN)
    {
      // Far gets a point
      nearStart = true;
      ResetPoint();
    }
  }
}

// Bounce the light
void SpeedIdle() {
  if (moarZoom)
  {
    zoom /= ZOOMRECTIFIER;
    if (zoom < MAXZOOM)
    {
      zoom = MAXZOOM;
      moarZoom = !moarZoom;
    }
  }
  else
  {
    zoom = (zoom * ZOOMRECTIFIER) + 1;
    if (zoom > MINZOOM)
    {
      zoom = MINZOOM;
      moarZoom = !moarZoom;
    }
  }
  //Serial.println(zoom);
}

// Check if light needs to be bounced
void BounceCheckIdle() {
  if (pin < MINPIN)
  {
    down = false;
    pin = MINPIN + 1;
    SpeedIdle();
  }
  else if (pin > MAXPIN)
  {
    down = true;
    pin = MAXPIN - 1;
    SpeedIdle();
  }
}

// Bounce the light IN THE GAME
void BounceGame() {
  down = !down;
  digitalWrite(pin, LOW);
  if (down)
  {
    pin--;
  }
  else
  {
    pin++;
  }
  digitalWrite(pin, HIGH);
  zoom /= ZOOMRECTIFIER;
  timer = 0;
}

// Reset the point
void ResetPoint() {
  // Show Score as Constant
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

  // Show Most Recent Point
  for (int i = 0; i < BLINK; i++)
  {
    digitalWrite(pointPin, LOW);
    delay(DELAYBASE);
    digitalWrite(pointPin, HIGH);
    delay(DELAYBASE);
  }
  delay(4 * DELAYBASE);

  // Determine Game end
  if (scoreNear >= 3)
  {
    //Serial.println("BEGIN NEAR WIN");
    for (int i = MINPIN; i < MINPIN + scoreFar; i++)
    {
      digitalWrite(i,LOW);
      delay(DELAYBASE);
    }
    //Serial.println("DOOTALY DOOTALY BIT");
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
    for (int i = 0; i < 3; i++)
    {
      digitalWrite(MAXPIN - i, LOW);
    }
    //Serial.println("GAME ABOUT TO END");
    GameEnd();
  }
  else if (scoreFar >= 3)
  {
    for (int i = MAXPIN; i > MAXPIN - scoreNear; i--)
    {
      digitalWrite(i,LOW);
      delay(DELAYBASE);
    }

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
    for (int i = 0; i < 3; i++)
    {
      digitalWrite(MINPIN + i, LOW);
    }
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
    delay(DELAYBASE);

    GameInit();
  }
}
  
// Initialize the game
void GameInit() {
  //Serial.println("GAME START");
  // Set up for next point
  pin = (MAXPIN - MINPIN) / 2 + MINPIN;
  down = !nearStart;
  zoom = MINZOOM;
  prevNear = false;
  prevFar = false;
  timer = 0;
  nearTimer = 0;
  near = false;
  farTimer = 0;
  far = false;

  // Show starting light
  for (int i = 0; i < BLINK; i++)
  {
    digitalWrite(pin, HIGH);
    delay(DELAYBASE);
    digitalWrite(pin,LOW);
    delay(DELAYBASE);
  }
  digitalWrite(pin,HIGH);
  delay(5 * DELAYBASE);
  //Serial.print("Down: ");
  //Serial.println(down);
}

// End the game and return to idle
void GameEnd(){
  //Serial.println("Game ENDING");
  idle = true;
  pin = MAXPIN + 1;
  zoom = MINZOOM;
  down = true;
  moarZoom = true;
  near = false;
  prevNear = false;
  far = false;
  prevFar = false;
  lastMil = 0;
  mil = 0;
  timer = 0;
  nearTimer = 0;
  farTimer = 0;
  idle = true;
  scoreNear = 0;
  scoreFar = 0;
  nearStart = true;
  //Serial.println("GAME ENDED");

  // I have no idea why a near win does not work after this so reset
  reset_oh_yeah();
}

