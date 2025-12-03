# ESP32 4-DOF Robotic Arm


This repository contains the firmware and documentation for a 4-DOF robotic arm controlled by an ESP32 over Bluetooth. The system accepts simple text commands (servo name, angle, delay) and moves each servo using a smooth sweep motion while enforcing safe angle ranges.


### Features


- Bluetooth Serial command interface
- Sweep-based motion for all servos (smooth transitions)
- Per-servo safe angle enforcement to prevent mechanical damage
- Serial + Bluetooth telemetry for debugging
- Default home-position initializer on boot


### Repo Layout
See the repository tree at the top of this document.


### Quick Start
1. Open `src/ESP32_Robot_Arm.ino` in the Arduino IDE.
2. Install the `ESP32Servo` library via Library Manager.
3. Configure pins in the top of the sketch if needed.
4. Upload to an ESP32 DevKit V1.
5. Pair/connect via Bluetooth (device name: `ESP32_ROBOT_ARM`) and send ASCII commands like `BASE 120 20`.


> NOTE: In the sample hardware, an Arduino board is used *only* as a 5V power source for two servos. The ESP32 controls the servos via signal pins. See `docs/WIRING.md` for details.


### License
This repo is licensed under the MIT License. See `LICENSE`.
