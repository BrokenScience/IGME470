#include <toneAC.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int signalMax = 0;
   int signalMin = 0;
// collect data for 50 mS
   for (int i = 0; i < 50; i++)
   {
      int sample = analogRead(A0);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   int out = signalMax - signalMin;
   Serial.println(out);
   out -= 600;
   if (out <= 0)
   {
    out = 0;
   }
   out *= 10;
   toneAC(1000, out);
}
