#include <Stepper.h>
#include <Servo.h>

// Stepper Stuff
const int S_IN4 = 30, S_IN3 = 32, S_IN2 = 34, S_IN1 = 36, stepsPer = 200;
Stepper stepper(stepsPer, S_IN1, S_IN2, S_IN3, S_IN4);

// Compressor Stuff
const int C_IN1 = 52, C_IN2 = 53, C_enA = 6, S1 = 43, S2 = 42;

// IR Stuff
const int IR1 = 51, IR2 = 49;

// Servo stuff
const int servoPin = 9;
Servo gate;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Compressor Motor
  pinMode(C_IN1, OUTPUT);
  pinMode(C_IN2, OUTPUT);
  pinMode(C_enA, OUTPUT);
  // Encoder
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);

  // IR Sensors
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);

  // Servo
  gate.attach(9);
  gate.write(0); // 0 the servo
  delay(1000); // Pause for movement
  gate.write(180); // Move to closed to begin with

  // Stepper
  stepper.setSpeed(60);

  digitalWrite(C_IN1, HIGH); // attempt default?
  digitalWrite(C_IN2, LOW); 
  analogWrite(C_enA, 128); // Move at half speed

  delay(1000); // Move for 1s 

}

void loop() {
  // Testing the Stepper

  /*
  stepper.step(stepsPer);
  delay(500);

  stepper.step(-stepsPer);
  delay(500);

  */
}
