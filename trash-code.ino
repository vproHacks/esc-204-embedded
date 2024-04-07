#include <Stepper.h>
#include <Servo.h>
#include <NewPing.h>


// Stepper Stuff
const int S_IN4 = 26, S_IN3 = 28, S_IN2 = 32, S_IN1 = 12, stepsPer = 200;
Stepper stepper(stepsPer, S_IN1, S_IN2, S_IN3, S_IN4);
int travel;

// Compressor Stuff
const int C_IN3 = 23, C_IN4 = 22, C_enB = 6, S1 = 27, S2 = 29;

// IR Stuff
const int IR1 = 33, IR2 = 35;

// Ultrasonic Stuff
const int ECHO = 25, TRIG = 31;
NewPing sonar(TRIG, ECHO, 50);
unsigned long pingTime = millis(), dist;
const int rightBoxCompress = 1323,
  rightBoxDispense = 555,
  leftBoxCompress = 1727,
  leftBoxDispense = 1343;

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
  stepper.setSpeed(80);
}

void loop() {
  // Asynchronous Ping-Response Checker
  /*if (millis() >= pingTime) {
    pingTime = millis() + 100; // Check every 100 ms
    dist = sonar.ping_median(10, 50);
    Serial.println("Dist: ");
    Serial.println(dist);
  }*/

  dist = sonar.ping_median(10, 50);
  delay(100);
  stepper.step(stepsPer);
  delay(500);
  dist = dist - sonar.ping_median(10, 50);
  Serial.print("Dist for 200steps: ");
  Serial.println(dist / 200);

  stepper.step(-stepsPer);
  delay(500);


}
