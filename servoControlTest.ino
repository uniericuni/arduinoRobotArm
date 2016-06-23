#include<Servo.h>

Servo myServo_1;
Servo myServo_2;
Servo myServo_3;

const int maxAngle = 180;
const int minAngle = 3;
int analogPin_1  = 5;
int analogPin_2  = 4;
int analogPin_3  = 2;
double val_1 = 0;
double val_2 = 0;
double val_3 = 0;
int servoAng_1 = 0;
int servoAng_2 = 0;
int servoAng_3 = 0;

void setup() {
  myServo_1.attach(8, 500, 2400);
  myServo_2.attach(9, 500, 2400);
  myServo_3.attach(10 , 500, 2400);
  myServo_1.write(90);
  myServo_2.write(90);
  myServo_3.write(90);
  val_1 = analogRead(analogPin_1);
  val_2 = analogRead(analogPin_2);
  val_3 = analogRead(analogPin_3);
  servoAng_1 = int( (val_1/1023) * (maxAngle-minAngle) + minAngle );
  servoAng_2 = int( (val_2/1023) * (maxAngle-minAngle) + minAngle );
  servoAng_3 = int( (val_3/1023) * (maxAngle-minAngle) + minAngle );
  Serial.begin(9600);
  delay(3000);
  Serial.println("ready to go !");
}

void loop() {
  val_1 = analogRead(analogPin_1);
  servoAng_1 = int( (val_1/1023) * (maxAngle-minAngle) + minAngle );
  //Serial.println(servoAng_1);
  myServo_1.write(servoAng_1);
  
  val_2 = analogRead(analogPin_2);
  servoAng_2 = int( (val_2/1023) * (maxAngle-minAngle) + minAngle );
  //Serial.println(servoAng_2);
  myServo_2.write(servoAng_2);

  val_3 = analogRead(analogPin_3);
  servoAng_3 = int( (val_3/1023) * (maxAngle-minAngle) + minAngle );
  Serial.println(servoAng_3);
  myServo_3.write(servoAng_3);
  delay(30);
}
