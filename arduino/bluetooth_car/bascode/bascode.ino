#include <Servo.h>  //servo library

#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define LED 13

enum CarState { HALT, FWD, RIGHT_T, LEFT_T, LINE_TRACKING, BWD };
enum RoadState { CLEAR, LEFT_BLOCK, RIGHT_BLOCK, FULL_BLOCK };
enum CarSpeedState { STOP, SLOW, AVERAGE, FAST };

Servo servo; // create servo instance to control servo
const int ECHO = A4;  
const int TRIG = A5; 

char getstr;
CarState carState = HALT;
RoadState roadState = CLEAR;
CarSpeedState carSpeedState = STOP;
int leftDistance = 0, rightDistance = 0, middleDistance = 0;

/****************************
* HELPER
****************************/
int getCarSpeedFromState(CarSpeedState speedState) {
  switch (speedState) {
    case STOP:
      return 0;
    case SLOW:
      return 64;
    case AVERAGE:
      return 128;
    case FAST:
      return 255;
    default:
      return 128;
  }
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
  carSpeedState = STOP;
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
  if (carSpeedState != STOP) return;
  servo.write(90);
  middleDistance = Distance_test();
  servo.write(10);
  delay(500);
  rightDistance = Distance_test();
  servo.write(170);
  delay(500);
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

    if(middleDistance <= 30) {     
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
/****************************
* 
****************************/

/****************************
* INITIALIZATION
****************************/
void setup() { 
  servo.attach(3);  // attach servo on pin 3 to servo object
  Serial.begin(9600);
  pinMode(ECHO, INPUT);    
  pinMode(TRIG, OUTPUT);  
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  void immediateStop();
}

void loop() { 
//  if (isObjectDetectedInFront() && carState != 0) {
//    if (carState == 1) {
//      stop();
//      carState = 0;
//    }
//  } 
//  getstr = Serial.read();
//  switch(getstr){
//    case 'f': forward(); carState=1; break;
//    case 'b': back(); carState=2;   break;
//    case 'l': left(); carState=3;   break;
//    case 'r': right(); carState=4; break;
//    case 's': stop(); carState=0;  break;
//    default:  break;
//  }
}

/****************************
* 
****************************/

