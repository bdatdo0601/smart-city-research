//www.elegoo.com

//Line Tracking IO define
#define LT_R !digitalRead(10)
#define LT_M !digitalRead(4)
#define LT_L !digitalRead(2)

#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define LED 13

#define carSpeed 150

bool state = LOW;

void back(){
  digitalWrite(ENA,HIGH);
  digitalWrite(ENB,HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("go forward!");
}

void forward(){
  digitalWrite(ENA,HIGH);
  digitalWrite(ENB,HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("go back!");
}

void right(){
  digitalWrite(ENA,HIGH);
  digitalWrite(ENB,HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("go left!");
}

void left(){
  digitalWrite(ENA,HIGH);
  digitalWrite(ENB,HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW); 
  Serial.println("go right!");
} 

void stateChange(){
  state = !state;
  digitalWrite(LED, state);
  Serial.println("Light");  
}

void stop(){
   digitalWrite(ENA, LOW);
   digitalWrite(ENB, LOW);
   Serial.println("Stop!");
} 

void setup(){
  Serial.begin(9600);
  pinMode(LT_R,INPUT);
  pinMode(LT_M,INPUT);
  pinMode(LT_L,INPUT);
  pinMode(LED, OUTPUT); 
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  Serial.println("here"); 
  stop();  
}

void loop() {
  Serial.println("Cycle");
  Serial.println(LT_M);
  Serial.println( LT_L);
  Serial.println(LT_R);

  if(LT_M){
    forward();
    analogWrite(ENB, 0);
    analogWrite(ENA, 0);
    Serial.println("Moving forward");   
  } else if (LT_R) {
    left();
    analogWrite(ENB, 0);
    analogWrite(ENA, 0);
//    while(LT_R);
    Serial.println("Turn right");
  } else if (LT_L) {
    right();
    analogWrite(ENB, 0);
    analogWrite(ENA, 0);
//    while(LT_L);
    Serial.println("Turn left");
  } 
//  else if(LT_R) { 
//    right();
//    analogWrite(ENB, 40);
//    analogWrite(ENA, 40);
//    while(LT_R); 
//      stateChange() ;  
//      Serial.println("Turning Right");                     
//  }   
//  else if(LT_L) {
//    left();
//    analogWrite(ENB, 40);
//    analogWrite(ENA, 40);
//    while(LT_L); 
//      Serial.println("Turning Left");  
//  }
}

