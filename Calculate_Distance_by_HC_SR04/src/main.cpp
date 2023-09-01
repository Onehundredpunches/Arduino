#include <Arduino.h>

#define ledPin A0
#define echoPin 10
#define trigPin 11

unsigned long duration;
float distance;

void setup()
{
  Serial.begin(9600);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  digitalWrite(trigPin, LOW); // off chân trig
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); // on chân trig --> phát xung
  delayMicroseconds(10);       // delay 10us để kích hoạt đo Kc

  digitalWrite(trigPin, LOW); // off chân trig

  // Tính độ rộng xung mức HIGH ở chân echo, tg truyền và nhận về
  duration = pulseIn(echoPin, HIGH);

  /*
    distance = speed of sound * duration
    the speed of sound is 343,4 m/s or 0,0343 cm/microsecond
    distance = 0,0343 * duration (sending and returning)
    distance = 0,0343 * duration/2 (returning)
    0,0343/2 = 0,01715 = 1/58,31
    --> distance = duration/58,31
  */

  if (duration >= 38000)
  {
    Serial.print("Out of range ");
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    distance = float(duration / 58.31);

    if (distance <= 9)
    {
      digitalWrite(ledPin, HIGH);
      Serial.print("Obstacle detection distance: ");
      Serial.print(distance);
      Serial.println(" cm");
    }
    else
    {
      digitalWrite(ledPin, HIGH);
      delay(100);
      digitalWrite(ledPin, LOW);
      Serial.print("Obstacle detection distance: ");
      Serial.print(distance);
      Serial.println(" cm");
    }
  }
  delay(300);
}
