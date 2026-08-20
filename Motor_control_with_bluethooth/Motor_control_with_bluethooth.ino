#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled!
#endif

BluetoothSerial SerialBT;

// Left Motor (Motor A)
const int ENA = 14;
const int IN1 = 26;
const int IN2 = 27;

// Right Motor (Motor B)
const int ENB = 25;
const int IN3 = 32;
const int IN4 = 33;

int currentSpeed = 200; // Default speed (0 - 255)

void setup() {
  Serial.begin(115200);
  
  // Pair name visible in phone Bluetooth settings
  SerialBT.begin("ESP32_BT_Car"); 
  Serial.println("Bluetooth RC Car Ready. Pair with 'ESP32_BT_Car'");

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopCar();
}

// Direction Logic Functions
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  
  analogWrite(ENA, currentSpeed);
  analogWrite(ENB, currentSpeed);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  
  analogWrite(ENA, currentSpeed);
  analogWrite(ENB, currentSpeed);
}

void turnLeft() {
  // Left motor reverses, Right motor moves forward (Tank Turn)
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  
  analogWrite(ENA, currentSpeed);
  analogWrite(ENB, currentSpeed);
}

void turnRight() {
  // Left motor moves forward, Right motor reverses
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  
  analogWrite(ENA, currentSpeed);
  analogWrite(ENB, currentSpeed);
}

void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void loop() {
  if (SerialBT.available()) {
    char cmd = SerialBT.read();

    // Log received command
    Serial.print("Received Command: ");
    Serial.println(cmd);

    // Map Bluetooth RC Car App Commands
    switch (cmd) {
      case 'F': moveForward(); break;
      case 'B': moveBackward(); break;
      case 'L': turnLeft(); break;
      case 'R': turnRight(); break;
      case 'S': stopCar(); break;

      // Dynamic Speed Controls (Sent by apps like 'Bluetooth RC Controller')
      case '0': currentSpeed = 0; break;
      case '1': currentSpeed = 100; break;
      case '2': currentSpeed = 120; break;
      case '3': currentSpeed = 140; break;
      case '4': currentSpeed = 160; break;
      case '5': currentSpeed = 180; break;
      case '6': currentSpeed = 200; break;
      case '7': currentSpeed = 220; break;
      case '8': currentSpeed = 240; break;
      case '9':
      case 'q': currentSpeed = 255; break;
    }
  }
}