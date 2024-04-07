#include <Stepper.h>
#include <Servo.h>
#include <NewPing.h>


// Stepper Stuff
const int S_IN4 = 26, S_IN3 = 28, S_IN2 = 32, S_IN1 = 30, stepsPer = 200;
Stepper stepper(stepsPer, S_IN1, S_IN2, S_IN3, S_IN4);

// Compressor Stuff
const int C_IN3 = 23, C_IN4 = 22, C_enB = 6, S1 = 27, S2 = 29;

// IR Stuff
const int IR1 = 33, IR2 = 35;

// Ultrasonic Stuff
const int ECHO = 25, TRIG = 31;
NewPing sonar(TRIG, ECHO, 100);
unsigned long pingTime = millis();

// Servo stuff
const int servoPin = 9;
Servo gate;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Compressor Motor
  pinMode(C_IN3, OUTPUT);
  pinMode(C_IN4, OUTPUT);
  pinMode(C_enB, OUTPUT);
  // Encoder
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);

  // IR Sensors
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);

  // Servo
  gate.attach(9);
  //gate.write(0); // 0 the servo
  //delay(1000); // Pause for movement
  //gate.write(180); // Move to closed to begin with

  // Stepper
  stepper.setSpeed(60);
}

void loop() {
  // Testing the Stepper
  delay(50);
  Serial.print("Dist: ");
  Serial.println(sonar.ping_cm());
  /*
  stepper.step(stepsPer);
  delay(500);

  stepper.step(-stepsPer);
  delay(500);

  */
}
