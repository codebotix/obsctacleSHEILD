#include "AFMotor.h"
#include <Servo.h>

#define echopin A3 // echo pin
#define trigpin A4 // Trigger pin

Servo myservo;

const int MOTOR_1 = 1; 
const int MOTOR_2 = 2; 

AF_DCMotor motor1(MOTOR_1, MOTOR12_64KHZ); // create motor object, 64KHz pwm
AF_DCMotor motor2(MOTOR_2, MOTOR12_64KHZ); // create motor object, 64KHz pwm

//===============================================================================
//  Initialization
//===============================================================================

int distance_L, distance_F, distance_R;
long distance;

int set = 20;
 
void setup() {
  Serial.begin(9600);           // Initialize serial port
  Serial.println("Start");

  myservo.attach(10);
  myservo.write(90);

  pinMode (trigpin, OUTPUT);
  pinMode (echopin, INPUT );
  
  motor1.setSpeed(180);          // set the motor speed to 0-255
  motor2.setSpeed(180);
}
//===============================================================================
//  Main
//=============================================================================== 
void loop() {
 distance_F = data();
 Serial.print("S=");
 Serial.println(distance_F);
  if (distance_F > set){
   Serial.println("Forward");
  motor1.run(FORWARD);         // turn it on going forward
  motor2.run(FORWARD); 
    }
    else{hc_sr4();}
}


long data(){
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  distance = pulseIn (echopin, HIGH);
  return distance / 29 / 2;
}


void compareDistance() {
  if (distance_L > distance_R && distance_L > distance_F) {
    Serial.println("Turning Left");
    motor1.run(BACKWARD);
    motor2.run(FORWARD);
    delay(350);
  } else if (distance_R > distance_L && distance_R > distance_F) {
    Serial.println("Turning Right");
    motor1.run(FORWARD);
    motor2.run(BACKWARD);
    delay(350);
  } else {
    Serial.println("Going Backward");
    motor1.run(BACKWARD);
    motor2.run(BACKWARD);
    delay(300);
    Serial.println("Turning Left");
    motor1.run(BACKWARD);
    motor2.run(FORWARD);
    delay(500);
  }
}

void hc_sr4(){
    Serial.println("Stop");
    motor1.run(RELEASE);         // stopped
    motor2.run(RELEASE);
   
    myservo.write(0);
    delay(300);
    distance_R = data();
    delay(100);
    myservo.write(170);
    delay(500);
    distance_L = data();
    delay(100);
    myservo.write(90);
    delay(300);
    compareDistance();
}
