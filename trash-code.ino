#include <Stepper.h>

const int IN4 = 30, IN3 = 32, IN2 = 34, IN1 = 36, stepsPer = 200;

const int S1 = 43, S2 = 42;

const int IR1 = 51, IR2 = 49;

Stepper stepper(stepsPer, IN1, IN2, IN3, IN4);

void setup() {
  // put your setup code here, to run once:
  stepper.setSpeed(60);
}

void loop() {
  // put your main code here, to run repeatedly:
  stepper.step(stepsPer);
  delay(500);

  stepper.step(-stepsPer);
  delay(500);
}
