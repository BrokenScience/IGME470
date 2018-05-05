void setup() {
  // start serial port at 9600 bps and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Set button pins
  for (int i = 2; i <= 7; i++)
  {
    pinMode(i, INPUT);
  }
  establishContact();  // send a byte to establish contact until receiver responds
}

void loop() {
  if (Serial.available() > 0) {
    for (int i = 2; i <= 7; i++)
    {
      Serial.print(digitalRead(i));
    }
    Serial.println();
    
    // wait for character to continue
    Serial.read();
  }
}

void establishContact() {
  Serial.println("0");   // send an initial string
  while (Serial.available() <= 0) {
    delay(300);
  }
}
