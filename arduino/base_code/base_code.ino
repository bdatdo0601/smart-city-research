//www.elegoo.com

//    The direction of the car's movement
//  ENA   ENB   IN1   IN2   IN3   IN4   Description
//  HIGH  HIGH  HIGH  LOW   LOW   HIGH  Car is runing forward
//  HIGH  HIGH  LOW   HIGH  HIGH  LOW   Car is runing back
//  HIGH  HIGH  LOW   HIGH  LOW   HIGH  Car is turning left
//  HIGH  HIGH  HIGH  LOW   HIGH  LOW   Car is turning right
//  HIGH  HIGH  LOW   LOW   LOW   LOW   Car is stoped
//  HIGH  HIGH  HIGH  HIGH  HIGH  HIGH  Car is stoped
//  LOW   LOW   N/A   N/A   N/A   N/A   Car is stoped

//define L298n module IO Pin
#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11

// If you haven't configured your device before use this
#define BLUETOOTH_SPEED 9600

#include <SoftwareSerial.h>

// Swap RX/TX connections on bluetooth chip
//   Pin 10 --> Bluetooth TX
//   Pin 11 --> Bluetooth RX
SoftwareSerial mySerial(10, 11); // RX, TX


void forward()
{
    digitalWrite(ENA, HIGH);   //enable L298n A channel
    digitalWrite(ENB, HIGH);   //enable L298n B channel
    digitalWrite(IN1, HIGH);   //set IN1 hight level
    digitalWrite(IN2, LOW);    //set IN2 low level
    digitalWrite(IN3, LOW);    //set IN3 low level
    digitalWrite(IN4, HIGH);   //set IN4 hight level
    Serial.println("Forward"); //send message to serial monitor
}

void back()
{
    digitalWrite(ENA, HIGH);
    digitalWrite(ENB, HIGH);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    Serial.println("Back");
}

void left()
{
    digitalWrite(ENA, HIGH);
    digitalWrite(ENB, HIGH);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.println("Left");
}

void right()
{
    digitalWrite(ENA, HIGH);
    digitalWrite(ENB, HIGH);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    Serial.println("Right");
}

void slowDown(int delayTime)
{
    for (int i = 255; i >= 0; i--)
    {
        analogWrite(ENB, i);
        analogWrite(ENA, i);
        delay(delayTime);
    }
}

void speedUp(int delayTime)
{
    for (int i = 0; i <= 255; i++)
    {
        analogWrite(ENB, i);
        analogWrite(ENA, i);
        delay(delayTime);
    }
}

void instaStop()
{
    digitalWrite(ENB, LOW);
    digitalWrite(ENA, LOW);
}

void waitForResponse() {
    delay(1000);
    while (mySerial.available()) {
      Serial.write(mySerial.read());
    }
    Serial.write("\n");
}

void btSetup() {
  Serial.println("Starting config");
  mySerial.begin(BLUETOOTH_SPEED);
  delay(1000);

  // Should respond with OK
  mySerial.print("AT");
  waitForResponse();

  // Should respond with its version
  mySerial.print("AT+VERSION");
  waitForResponse();

  // Set pin to 0000
  mySerial.print("AT+PIN0000");
  waitForResponse();

  // Set the name to ROBOT_NAME
  mySerial.print("AT+NAME");
  mySerial.print("BT_CAR0");
  waitForResponse();

  // Set baudrate to 57600
  mySerial.print("AT+BAUD7");
  waitForResponse();

  Serial.println("Done!");
}

void setup()
{
    //before useing io pin, pin mode must be set first
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
    while (Serial.available()) {
      char input = (char) Serial.read();
      if (input == 'f') {
        forward();
      } else {
        instaStop();
      }
    }
}
