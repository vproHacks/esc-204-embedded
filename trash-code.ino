#include <Stepper.h>
#include <Servo.h>
#include <NewPing.h>


// Stepper Stuff
const int S_IN4 = 26, S_IN3 = 28, S_IN2 = 32, S_IN1 = 12, stepsPer = 200;
Stepper stepper(stepsPer, S_IN1, S_IN2, S_IN3, S_IN4);
bool toTravel = false;
double travel;

// Compressor Stuff
const int C_IN3 = 23, C_IN4 = 22, C_enB = 6, S1 = 27, S2 = 29;
bool toCompress = false;
unsigned long compressTime = millis();

// IR Stuff
const int plasticIR = 35, metalIR = 33;
bool testIR = false;
unsigned long irTest = millis(), irCallback = millis();

// Ultrasonic Stuff
const int ECHO = 25, TRIG = 31;
NewPing sonar(TRIG, ECHO, 50);
unsigned long pingTime = millis(), dist;
const int rightBoxCompress = 551,
  rightBoxDispense = 1010,
  leftBoxCompress = 200,
  leftBoxDispense = 555;

// Servo stuff
const int servoPin = 9;
Servo gate;
unsigned long dispenseTime = millis();
bool toDispense = false;

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
  pinMode(metalIR, INPUT);
  pinMode(plasticIR, INPUT);

  // Servo
  gate.attach(9);
  //gate.write(0); // 0 the servo
  //delay(1000); // Pause for movement
  gate.write(0); // Move to closed to begin with
  delay(500);

  // Stepper
  stepper.setSpeed(60);
}

double absd(double x) {
  return (x < 0 ? -x : x);
}

const double distPerStep = 3.1; // Roughly 3.1 Ultrasonic Sensor Dists per Step (reversed)
int travelCommand = 0; // 1 -> Dispense, 2 -> Compress, 0 -> Nothing

void loop() {
  // Asynchronous Calls

  // Dispense the Goods
  if (toDispense && millis() >= dispenseTime) {
    toDispense = false; // dispensing done
    gate.write(0); // Sufficient enough time between next command that another wait is not needed
  }

  // Compress if IR is called low
  if (toCompress && millis() >= compressTime) {
    toCompress = false;
    digitalWrite(C_IN3, LOW);
    digitalWrite(C_IN4, HIGH);
  }

  int metalRead = digitalRead(metalIR),
    plasticRead = digitalRead(plasticIR);

  if ((metalRead == LOW || plasticRead == LOW) && !testIR) {
    // check in the next 300ms, if the IR is still pulled high, then it is ready to get compressed
    Serial.println("Pulled LOW");
    irTest = millis() + 300;
    testIR = true;
  } 

  if (testIR && millis() >= irTest) {
    Serial.println("Checked out!");
    if (metalRead == LOW) {
      // compress metal
      toTravel = true;
      travelCommand = 2;
      travel = (dist - leftBoxCompress) / distPerStep;
    } else if (plasticRead == LOW) {
      // compress plastic
      toTravel = true;
      travelCommand = 2;
      travel = (dist - leftBoxCompress) / distPerStep;
    } else testIR = false;
  }

  // Manage distance and travelling
  if (millis() >= pingTime) {
    pingTime = millis() + 100; // Check every 100 ms
    dist = sonar.ping_median(12, 50);

    // Debug
    Serial.println("Dist: ");
    Serial.println(dist);
    //Serial.print("Metal IR: ");
    //Serial.println((metalRead == HIGH ? "HIGH" : "LOW"));
    //Serial.print("Plastic IR: ");
    //Serial.println((plasticRead == HIGH ? "HIGH" : "LOW"));

    // travel
    if (toTravel && !toCompress) { // Can't move while compressing
      Serial.print("Steps to Travel: ");
      Serial.println((int) travel);
      stepper.step((int) travel);
      toTravel = false;
      if (travelCommand == 1) {
        toDispense = true;
        dispenseTime = millis() + 1500;
        gate.write(180); // Open the gate
      } else if (travelCommand == 2) {
        toCompress = true;
        compressTime = millis() + 200000; // compress for 1 min
        analogWrite(C_enB, 255);
        digitalWrite(C_IN3, HIGH); // Default Direction
        digitalWrite(C_IN4, LOW);
      }
    }
  }



  while (Serial.available()) {
    char c = Serial.read();
    if (c == 'M') {
      toTravel = true;
      travelCommand = 1;
      travel = (dist - leftBoxDispense) / distPerStep;
    } else if (c == 'P') {
      toTravel = true;
      travelCommand = 1;
      travel = (dist - rightBoxDispense) / distPerStep;
    }
    Serial.print("Steps to Travel: ");
    Serial.println((int) travel);
  }

  delay(10); // Standard 10ms delay
}
