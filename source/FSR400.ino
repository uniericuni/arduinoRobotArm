#include <Servo.h>

// feedback states
#define HOLD 0
#define BACKWARD 1
#define FORWARD 2

// pressure level
#define ZERO 0
#define TRANSIENT 1
#define ONE 2

Servo servo;
Servo feedbackServo;

const int minAngle = 10;
const int maxAngle = 180;

int servoPin = 9;
int feedbackServoPin = 10;
int fsrPin = 2;
int fsrVal = 0;
int presLevel = ZERO;
int state = HOLD;
int next_state = state;
double servoAngle = 3;
double currAngle = servoAngle;
double angleVec = 0;
 
void setup(void) {
  
  servo.attach(servoPin,500,2400);
  servo.write(minAngle);
  feedbackServo.attach(feedbackServoPin,500,2400);
  feedbackServo.write(minAngle);
  Serial.begin(9600);   
  delay(1000);
  Serial.print("\nReady to Go ...");
}
 
void loop(void) {

  Serial.print("\n");
  
  // evaluate current pressure level
  fsrVal = RCtime(fsrPin);
  presLevel = pressureLevel(fsrVal);

  // finite state machine
  Serial.print("\nCurrent State = ");
  switch(state) {
    case HOLD:
      Serial.print("HOLD");
      angleVec = 0;
      if(presLevel == ONE)
        next_state = BACKWARD;
      else if(presLevel == ZERO && isExhausted(currAngle))
        next_state = FORWARD;
      else
        next_state = HOLD;
      break;
      
    case BACKWARD:
      Serial.print("BACWARD");
      if(presLevel == TRANSIENT || presLevel == ZERO)
        next_state = HOLD;
      else
        next_state = BACKWARD;
      angleVec = 300.0 / double(fsrVal);
      ( angleVec > 10.0 ) ? angleVec = 10.0 : angleVec = angleVec;
      break;
      
    case FORWARD:
      Serial.print("FOWARD");
      if(presLevel == TRANSIENT || !isExhausted(currAngle))
        next_state = HOLD;
      else
        next_state = FORWARD;
        (servoAngle <= minAngle) ? angleVec = 0 : angleVec = -3;
      break;
      
    default:
      next_state = HOLD;
  }
  
  //////////////

  (servoAngle < minAngle) ? servoAngle=minAngle : (servoAngle > maxAngle) ? servoAngle=maxAngle : servoAngle += angleVec;
  servo.write(int(servoAngle));
  feedbackServo.write(int(servoAngle));
  Serial.print("\nServo Angle = ");
  Serial.print(servoAngle);
  Serial.print("\nAngle Velocity = ");
  Serial.print(angleVec);
  delay(100);
  currAngle = servoAngle;
  state = next_state;
}

// check if extented arm is exhausted
bool isExhausted(int currAngle) {
  
  if(currAngle > minAngle) {
    return  true;
  }
  else {
    return false;
  }

}

// evaluate current pressure level
int pressureLevel(int val) {

  Serial.print("\nCurrent Level = ");
  if(val >= 30000) {
    Serial.print("ZERO");
    return ZERO;
  }
  else if(val <= 500) {
    Serial.print("ONE");
    return ONE;
  }
  else {
    Serial.print("TRANSIENT");
    return TRANSIENT;
  }

}

// calculate RC time constant
int RCtime(int RCpin) {
  
  int reading = 0;
  pinMode(RCpin, OUTPUT);
  digitalWrite(RCpin, LOW); 
  pinMode(RCpin, INPUT);
  while (digitalRead(RCpin) == LOW)
  {
    reading++;
    if (reading == 30000)
      break;
  }
  Serial.print("\nRC value = ");
  Serial.print(reading);
  return reading;
  
}
