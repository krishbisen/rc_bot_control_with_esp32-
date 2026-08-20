#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled!
#endif

BluetoothSerial SerialBT;

// --- MOTOR PINS (L298N) ---
const int ENA = 14; const int IN1 = 26; const int IN2 = 27;
const int ENB = 25; const int IN3 = 32; const int IN4 = 33;

// --- STATE TRACKING FLAGS --- 
bool isConnected = false;
bool autoReconnectFlag = false; // FLAG: True when waiting for reconnection after drop

int currentSpeed = 200;
unsigned long lastPacketTime = 0;
const unsigned long TIMEOUT_MS = 1000;

void stopCar() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0); analogWrite(ENB, 0);
}

void moveForward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, currentSpeed); analogWrite(ENB, currentSpeed);
}

void moveBackward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, currentSpeed); analogWrite(ENB, currentSpeed);
}

void turnLeft() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, currentSpeed); analogWrite(ENB, currentSpeed);
}

void turnRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, currentSpeed); analogWrite(ENB, currentSpeed);
}

// Bluetooth callback handling connection state flags
void btCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if (event == ESP_SPP_SRV_OPEN_EVT) {
    isConnected = true;
    autoReconnectFlag = false; // RESET FLAG: Link successfully re-established
    lastPacketTime = millis();
    
    Serial.println("\n===========================================");
    Serial.println("[STATE FLAG] autoReconnectFlag = FALSE");
    Serial.println("[STATUS] Device connected! Drive mode ACTIVE.");
    Serial.println("===========================================");
  } 
  else if (event == ESP_SPP_CLOSE_EVT) {
    isConnected = false;
    autoReconnectFlag = true; // SET FLAG: Connection dropped, waiting for reconnect
    stopCar();               // Immediate safety stop
    
    Serial.println("\n===========================================");
    Serial.println("[STATE FLAG] autoReconnectFlag = TRUE");
    Serial.println("[STATUS] Connection Lost! Motors killed.");
    Serial.println("[STATUS] Advertising... Waiting for device to re-enter range.");
    Serial.println("===========================================");
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  stopCar();

  SerialBT.register_callback(btCallback);
  SerialBT.begin("ESP32_BT_Car"); 

  Serial.println("System Ready. Waiting for Bluetooth connection...");
}

void loop() {
  // 1. ACTIVE CONNECTION STATE (Flag is FALSE)
  if (isConnected && !autoReconnectFlag) {
    if (SerialBT.available()) {
      char cmd = SerialBT.read();
      lastPacketTime = millis();

      switch (cmd) {
        case 'F': moveForward(); break;
        case 'B': moveBackward(); break;
        case 'L': turnLeft(); break;
        case 'R': turnRight(); break;
        case 'S': stopCar(); break;

        case '0': currentSpeed = 0; break;
        case '5': currentSpeed = 180; break;
        case '9': currentSpeed = 255; break;
      }
    }

    // Safety timeout check while connected
    if (millis() - lastPacketTime > TIMEOUT_MS) {
      stopCar();
    }
  } 

  // 2. DISCONNECTED / AUTO-RECONNECTING STATE (Flag is TRUE)
  else if (autoReconnectFlag) {
    // Ensure motors remain safely powered down while waiting
    stopCar(); 
  }
}