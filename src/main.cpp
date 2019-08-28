#include <Arduino.h>
#include <Servo.h>
#include "SBUS.h"


elapsedMillis ServoUpdateTimer = 0;
#define SERVO_REFRESH_RATE 2  //ms

#define LIFT1_PIN 5
#define LIFT2_PIN 6
#define THRUST1_PIN 3
#define THRUST2_PIN 4
#define RUDDER_PIN 20

SBUS x8r(Serial2);
Servo lift1;
Servo lift2;
Servo thrust1;
Servo thrust2;
Servo rudder;
uint16_t li1 = 1040, li2 = 1040, thr = 1040, rud = 1040;


// channel, fail safe, and lost frames data
uint16_t channels[16];
bool failSafe;
bool lostFrame;

void setup() {
  // begin the SBUS communication
  x8r.begin();
  lift1.attach(LIFT1_PIN);
  lift2.attach(LIFT2_PIN);
  thrust1.attach(THRUST1_PIN);
  thrust2.attach(THRUST2_PIN);
  rudder.attach(RUDDER_PIN);
}

void loop() {

  // look for a good SBUS packet from the receiver
  if (x8r.read(&channels[0], &failSafe, &lostFrame)) {
    li1 = map(channels[7], 295, 1688, 1000, 2000);
    li2 = map(channels[8], 295, 1688, 1000, 2000);
    thr = map(channels[6], 295, 1688, 1000, 2000);
    rud = map(channels[9], 295, 1688, 1000, 2000);
  }
  if ( ServoUpdateTimer >= SERVO_REFRESH_RATE)
  {
    if (li1 <999)
    {
      lift1.writeMicroseconds(1000);
      lift2.writeMicroseconds(1000);
      thrust1.writeMicroseconds(1000);
      thrust2.writeMicroseconds(1000);
      rudder.writeMicroseconds(1000);
      Serial.println("No Radio");
    }
    else
    {
      Serial.print("lift1 = ");
      Serial.print(li1);
      Serial.print(", lift2 = ");
      Serial.print(li2);
      Serial.print(", thruster = ");
      Serial.print(thr);
      Serial.print(", rudder = ");
      Serial.println(rud);
      lift1.writeMicroseconds(li1);
      lift2.writeMicroseconds(li2);
      thrust1.writeMicroseconds(thr);
      thrust2.writeMicroseconds(thr);
      rudder.writeMicroseconds(rud);

    }
  }
}