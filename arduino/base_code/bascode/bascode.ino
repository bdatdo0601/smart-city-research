#include <Servo.h>  //servo library

#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define LED 13

//Line Tracking IO define
#define LT_R !digitalRead(10)
#define LT_M !digitalRead(4)
#define LT_L !digitalRead(2)

enum CarState { HALT, FWD, RIGHT_T, LEFT_T, LINE_TRACKING, BWD };
enum RoadState { CLEAR, LEFT_BLOCK, RIGHT_BLOCK, FULL_BLOCK };
enum CarSpeedState { SLOW, AVERAGE, FAST };

Servo servo; // create servo instance to control servo
const int ECHO = A4;  
const int TRIG = A5; 

char getstr;
bool isLineTracking = false;
int leftDistance = 0, rightDistance = 0, middleDistance = 0;
CarState carState = HALT;
RoadState roadState = CLEAR;
CarSpeedState carSpeedState = AVERAGE;

/****************************
* HELPER
****************************/
int getCarSpeedFromState(CarSpeedState speedState) {
  switch (speedState) {
    case SLOW:
      return 100;
    case AVERAGE:
      return 128;
    case FAST:
      return 255;
  }
}

int getCarSpeedState() {
  switch (carSpeedState) {
    case SLOW:
      return 1;
    case AVERAGE:
      return 2;
    case FAST:
      return 3;
  }
}

int getRoadState() {
  switch (carSpeedState) {
    case CLEAR:
      return 0;
    case LEFT_BLOCK:
      return 1;
    case RIGHT_BLOCK:
      return 2;
    case FULL_BLOCK:
      return 3;
  }
}

int getCarState() {
  switch (carState) {
    case HALT:
      return 0;
    case FWD:
      return 1;
    case BWD:
      return 2;
    case RIGHT_T:
      return 3;
    case LEFT_T:
      return 4;
    case LINE_TRACKING:
      return 5;
  }
}

int getIntValue(bool data) {
  if (data) {
    return 1;
  }
  return 0;
}

/****************************
* 
****************************/

/****************************
 * MOVEMENT
 ****************************/

void forward(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  carState = FWD;
  Serial.println("Forward");
}

void backward(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  carState = BWD;
  Serial.println("Back");
}

void turnLeft(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  carState = LEFT_T;
  Serial.println("Left");
}

void turnRight(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW); 
  carState = RIGHT_T;
  Serial.println("Right");
}

void speedUp(int initialSpeed, int finalSpeed) {
  for (int i = initialSpeed; i <= finalSpeed; i+=5 ) {
    analogWrite(ENA, i);
    analogWrite(ENB, i);
    delay(10);  
  }
}

void slowDown(int initialSpeed, int finalSpeed) {
  for (int i = initialSpeed; i >= finalSpeed; i-=5 ) {
    analogWrite(ENA, i);
    analogWrite(ENB, i);
    delay(10);  
  }
}

void changeSpeed(CarSpeedState desiredSpeedState) {
  int currentSpeed = getCarSpeedFromState(carSpeedState);
  int desiredSpeed = getCarSpeedFromState(desiredSpeedState);
  if (currentSpeed > desiredSpeed) {
    speedUp(currentSpeed, desiredSpeed);
  } else {
    slowDown(currentSpeed, desiredSpeed);
  }
  carSpeedState = desiredSpeedState;
}

void immediateStop(){
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  carState = HALT;
  Serial.println("Halt!");
}

/****************************
* 
****************************/

/****************************
* OBJECT DETECTION
****************************/
//Ultrasonic distance measurement Sub function
int Distance_test() {
  digitalWrite(TRIG, LOW);   
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);  
  delayMicroseconds(20);
  digitalWrite(TRIG, LOW);   
  float Fdistance = pulseIn(ECHO, HIGH);  
  Fdistance = Fdistance / 58;       
  return (int)Fdistance;
}

void updateObstacleMeasurement() {
  if (carState != HALT) return;
  servo.write(90);
  middleDistance = Distance_test();
  
  servo.write(50);
  delay(1000);
  rightDistance = Distance_test();
  
  servo.write(130);
  delay(1000);
  leftDistance = Distance_test();
  
  servo.write(90);
  if ((middleDistance > 30) || (leftDistance > 30 && rightDistance >30)) {
    roadState = CLEAR;
  } else if (rightDistance <= 30) {
    roadState = RIGHT_BLOCK;
  } else if (leftDistance <= 30) {
    roadState = LEFT_BLOCK;
  } else {
    roadState = FULL_BLOCK;
  }
}

bool isObjectDetectedInFront() {
    servo.write(90);  //setservo position according to scaled value
    middleDistance = Distance_test();

    if(middleDistance <= 40) {     
        return true;
    }
    else {
        return false;
    }
}

/****************************
* 
****************************/
/****************************
* LINE TRACKING
****************************/

void toggleLineTracking(){
  isLineTracking = !isLineTracking;
  if (!isLineTracking) {
    immediateStop();
  }
}

void lineTracker() {
  if(LT_M){
    changeSpeed(carSpeedState);
    forward();
  }
  else if(LT_R) { 
    while (LT_R) {
      changeSpeed(FAST);
      turnRight();
      if (LT_L) break;
    }
  }   
  else if(LT_L) {
    while (LT_L) {
      changeSpeed(FAST);
      turnLeft();
      if (LT_R) break;
    }
  }
  if ((!LT_M && !LT_R && !LT_L) ) {
    immediateStop();
  }
}

/****************************
* 
****************************/

/****************************
* Utilities
****************************/
void sendCarStatus() { 
  String mdHeader = "md ";
  String rdHeader = "rd ";
  String ldHeader = "ld ";
  String csHeader = "cs ";
  String cssHeader = "css ";
  String rsHeader = "rs ";
  String iLTHeader = "iLT ";
  String lltHeader ="llt ";
  String rltHeader = "rlt ";
  String mltHeader = "mlt ";
  String mdCmd = mdHeader + middleDistance;
  String rdCmd = rdHeader + rightDistance;
  String ldCmd = ldHeader + leftDistance;
  String csCmd = csHeader + getCarState();
  String cssCmd = cssHeader + getCarSpeedState();
  String rsCmd = cssHeader + getRoadState();
  String iLTCmd = iLTHeader + getIntValue(isLineTracking);
  String lltCmd = lltHeader + getIntValue(LT_L);
  String rltCmd = rltHeader + getIntValue(LT_R);
  String mltCmd = mltHeader + getIntValue(LT_M);
  Serial.println(mdCmd);
  Serial.println(rdCmd);   
  Serial.println(ldCmd);   
  Serial.println(csCmd);   
  Serial.println(cssCmd);
  Serial.println(rsCmd);   
  Serial.println(iLTCmd); 
  
}
/****************************
* 
****************************/

/****************************
* INITIALIZATION
****************************/
void setup() { 
  servo.attach(3);  // attach servo on pin 3 to servo object
  Serial.begin(9600);
  pinMode(LT_R,INPUT);
  pinMode(LT_M,INPUT);
  pinMode(LT_L,INPUT);
  pinMode(ECHO, INPUT);   
  pinMode(TRIG, OUTPUT);  
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
}

void loop() { 
  sendCarStatus();
  getstr = Serial.read();
  if (isObjectDetectedInFront() && carState != HALT) {
    if (carState == FWD) {
      immediateStop();
      carState = HALT;
    }
  } else if (isLineTracking && (LT_M || LT_R || LT_L)) {
    lineTracker();
  }
  Serial.println(getstr);
  switch(getstr){
    case 'w': changeSpeed(carSpeedState); forward();  break;
    case 's': changeSpeed(carSpeedState); backward();  break;
    case 'a': changeSpeed(FAST); turnLeft();  break;
    case 'd': changeSpeed(FAST); turnRight(); break;
    case 'x': immediateStop();  break;
    case 'f': changeSpeed(SLOW); break;
    case 'g': changeSpeed(AVERAGE); break;
    case 'h': changeSpeed(FAST); break;
    case 'z': updateObstacleMeasurement(); break;
    case 'c': toggleLineTracking(); break;
    default: break;
  }
}

/****************************
* 
****************************/

