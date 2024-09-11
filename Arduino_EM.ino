 #include <Stepper.h>

#define TRIGGER_PIN1 2
#define ECHO_PIN1 3
#define TRIGGER_PIN2 4
#define ECHO_PIN2 5
#define TRIGGER_PIN3 6
#define ECHO_PIN3 7
#define TRIGGER_PIN4 8
#define ECHO_PIN4 9

const int stepsPerRevolution = 4096;  // Steps per revolution for 28BYJ-48 stepper motor
const int stepsPer45Degrees = stepsPerRevolution / 8;  // 45 degrees is 1/8th of a revolution

// Define stepper motor connections and create a stepper object
const int motorPin1 = 10;
const int motorPin2 = 11;
const int motorPin3 = 12;
const int motorPin4 = 13;

const int DISTANCE_THRESHOLD = 1; // centimeters
int Space1 = 1;
int Space2 = 1;
int Space3 = 1;
int CarWantToEnter = 0;
int Open = 0;

Stepper myStepper(stepsPerRevolution, motorPin1, motorPin3, motorPin2, motorPin4);

void setup() {
  // Set the speed of the stepper motor (RPM)
  myStepper.setSpeed(60);
  //Ultrasonic Sensors
  pinMode(TRIGGER_PIN1, OUTPUT); // Set TRIG pin as output
  pinMode(ECHO_PIN1, INPUT);  // Set ECHO pin as input
  pinMode(TRIGGER_PIN2, OUTPUT); // Set TRIG pin as output
  pinMode(ECHO_PIN2, INPUT);  // Set ECHO pin as input
  pinMode(TRIGGER_PIN3, OUTPUT); // Set TRIG pin as output
  pinMode(ECHO_PIN3, INPUT);  // Set ECHO pin as input
  pinMode(TRIGGER_PIN4, OUTPUT); // Set TRIG pin as output
  pinMode(ECHO_PIN4, INPUT);  // Set ECHO pin as input
  
  //Serial Communication
  Serial.begin(9600);  // Initialize serial communication with ESP8266
}

void loop() {
  Park1();
  Park2();
  Park3();
  GateSensor();
  
  if (Space1 && Space2 && Space3 && CarWantToEnter) {
    OpenTheGate();
  } else {
    CloseTheGate();
  }

  sendToESP();
  delay(1000); // Adjust delay as needed
}

void Park1() {
  // Trigger ultrasonic sensor
  digitalWrite(TRIGGER_PIN1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN1, LOW);

  // Measure duration of pulse from ECHO pin
  float duration1_us = pulseIn(ECHO_PIN1, HIGH);

  // Calculate distance
  float distance1_cm = duration1_us / 58.0; // converting microseconds to centimeters

  if (distance1_cm < DISTANCE_THRESHOLD) {
    Space1 = 0;
  } else {
    Space1 = 1;
  }
}

void Park2() {
  // Trigger ultrasonic sensor
  digitalWrite(TRIGGER_PIN2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN2, LOW);

  // Measure duration of pulse from ECHO pin
  float duration2_us = pulseIn(ECHO_PIN2, HIGH);

  // Calculate distance
  float distance2_cm = duration2_us / 58.0; // converting microseconds to centimeters

  if (distance2_cm < DISTANCE_THRESHOLD) {
    Space2 = 0;
  } else {
    Space2 = 1;
  }
}

void Park3() {
  // Trigger ultrasonic sensor
  digitalWrite(TRIGGER_PIN3, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN3, LOW);

  // Measure duration of pulse from ECHO pin
  float duration3_us = pulseIn(ECHO_PIN3, HIGH);

  // Calculate distance
  float distance3_cm = duration3_us / 58.0; // converting microseconds to centimeters

  if (distance3_cm < DISTANCE_THRESHOLD) {
    Space3 = 0;
  } else {
    Space3 = 1;
  }
}

void GateSensor() {
  // Trigger ultrasonic sensor
  digitalWrite(TRIGGER_PIN4, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN4, LOW);

  // Measure duration of pulse from ECHO pin
  float duration4_us = pulseIn(ECHO_PIN4, HIGH);

  // Calculate distance
  float distance4_cm = duration4_us / 58.0; // converting microseconds to centimeters

  if (distance4_cm < DISTANCE_THRESHOLD) {
    CarWantToEnter = 1;
  } else {
    CarWantToEnter = 0;
  }
}

void CloseTheGate() {
  if(Open == 1)
  {
    for (int i = 0; i < stepsPer45Degrees; i++) {
    myStepper.step(-1);
    delay(15);  // Adjust as necessary to control the speed of rotation
    }
    Open = 0;
  }
}

void OpenTheGate() {
  if(Open == 0)
  {
    for (int i = 0; i < stepsPer45Degrees; i++) {
    myStepper.step(1);
    delay(15);  // Adjust as necessary to control the speed of rotation
    }
    Open = 1;
  }
}

void sendToESP() {
  Serial.print(Space1);
  Serial.print(",");
  Serial.print(Space2);
  Serial.print(",");
  Serial.print(Space3);
  Serial.print(",");
  Serial.println(CarWantToEnter);
}
