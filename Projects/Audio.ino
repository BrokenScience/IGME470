#include <toneAC.h>

int uber = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
    int out;
    out = analogRead(A0);
    out -= 512;
    out = abs(out);
    Serial.println(out);
    if (out > 250)
    {
      if (out < 300)
      {
        toneAC(out % 10 * 500, 2);
        //Serial.println(out % 10 * 500);
      }
      else if (out < 350)
      {
        toneAC(out % 10 * 500, 4);
        //Serial.println(out % 10 * 500);
      }
      else if (out < 400)
      {
        toneAC(out % 10 * 500, 6);
        //Serial.println(out % 10 * 500);
      }
      else if (out < 450)
      {
        toneAC(out % 10 * 500, 8);
        //Serial.println(out % 10 * 500);
      }
      else
      {
        toneAC(out % 10 * 500, 10);
        //Serial.println(out % 10 * 500);
      }
   }
   else
   {
    toneAC();
   }
}
