#include <ESP32Servo.h>
#include "BluetoothSerial.h"

BluetoothSerial BT;

// Servo objects
Servo baseServo, armServo, elbowServo, gripServo;

// Current angles
int basePos   = 90;
int armPos    = 90;
int elbowPos  = 60;
int gripPos   = 0;

// Ranges
#define BASE_MIN   0
#define BASE_MAX   180

#define ARM_MIN    40
#define ARM_MAX    180

#define ELBOW_MIN  60
#define ELBOW_MAX  110

#define GRIP_MIN   0
#define GRIP_MAX   70

// Attach pins
#define BASE_PIN   5
#define ARM_PIN    18
#define ELBOW_PIN  19
#define GRIP_PIN   21

// ---------- Sweep Function ----------
void sweepMove(Servo &servo, int &currentPos, int targetPos, int stepDelay, String name) {

  Serial.printf("[SWEEP] %s: %d -> %d (delay per step: %d ms)\n",
                name.c_str(), currentPos, targetPos, stepDelay);
  BT.printf("[SWEEP] %s: %d -> %d\n", name.c_str(), currentPos, targetPos);

  if (targetPos > currentPos) {
    for (int pos = currentPos; pos <= targetPos; pos++) {
      servo.write(pos);
      delay(stepDelay);
    }
  } else {
    for (int pos = currentPos; pos >= targetPos; pos--) {
      servo.write(pos);
      delay(stepDelay);
    }
  }

  currentPos = targetPos;
  Serial.printf("[DONE] %s now at %d\n", name.c_str(), currentPos);
  BT.printf("[DONE] %s now at %d\n", name.c_str(), currentPos);
}

// ---------- Range Checker ----------
int clamp(int value, int minV, int maxV) {
  if (value < minV) return minV;
  if (value > maxV) return maxV;
  return value;
}

// ---------- Command Processor ----------
void processCommand(String cmd) {
  cmd.trim();
  Serial.printf("[CMD RECEIVED] %s\n", cmd.c_str());

  int firstSpace = cmd.indexOf(' ');
  int secondSpace = cmd.indexOf(' ', firstSpace + 1);

  if (firstSpace == -1 || secondSpace == -1) {
    Serial.println("[ERROR] Invalid format. Use: NAME ANGLE DELAY");
    BT.println("[ERROR] Invalid format.");
    return;
  }

  String name = cmd.substring(0, firstSpace);
  int angle   = cmd.substring(firstSpace + 1, secondSpace).toInt();
  int del     = cmd.substring(secondSpace + 1).toInt();

  name.toUpperCase(); // normalize

  // ----- BASE -----
  if (name == "BASE") {
    angle = clamp(angle, BASE_MIN, BASE_MAX);
    sweepMove(baseServo, basePos, angle, del, "BASE");
  }
  // ----- ARM -----
  else if (name == "ARM") {
    angle = clamp(angle, ARM_MIN, ARM_MAX);
    sweepMove(armServo, armPos, angle, del, "ARM");
  }
  // ----- ELBOW -----
  else if (name == "ELBOW") {
    angle = clamp(angle, ELBOW_MIN, ELBOW_MAX);
    sweepMove(elbowServo, elbowPos, angle, del, "ELBOW");
  }
  // ----- GRIP -----
  else if (name == "GRIP") {
    angle = clamp(angle, GRIP_MIN, GRIP_MAX);
    sweepMove(gripServo, gripPos, angle, del, "GRIP");
  }
  else {
    Serial.println("[ERROR] Unknown servo name");
    BT.println("[ERROR] Unknown servo name");
  }
}

void setDefaultPosition() {
  Serial.println("[INIT] Moving to default home position...");
  BT.println("[INIT] Moving to default home position...");

  sweepMove(baseServo, basePos, 90, 10, "BASE");
  sweepMove(armServo, armPos, 90, 10, "ARM");
  sweepMove(elbowServo, elbowPos, 60, 10, "ELBOW");
  sweepMove(gripServo, gripPos, 0, 10, "GRIP");

  Serial.println("[READY] Arm default position set.");
  BT.println("[READY] Arm default position set.");
}

void setup() {
  Serial.begin(115200);
  BT.begin("ESP32_ROBOT_ARM");

  Serial.println("=== ESP32 Robotic Arm Controller ===");
  BT.println("Connected: Send commands like 'BASE 120 20'");

  baseServo.attach(BASE_PIN, 500, 2500);
  armServo.attach(ARM_PIN, 500, 2500);
  elbowServo.attach(ELBOW_PIN, 500, 2500);
  gripServo.attach(GRIP_PIN, 500, 2500);

  delay(500);
  setDefaultPosition();
}

void loop() {
  if (BT.available()) {
    String cmd = BT.readStringUntil('\n');
    processCommand(cmd);
  }
}
