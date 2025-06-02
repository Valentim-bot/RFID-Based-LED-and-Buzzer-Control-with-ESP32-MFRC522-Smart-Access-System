#include <SPI.h>           // SPI library for RFID
#include <MFRC522.h>       // RFID library
#include <Wire.h>          // I2C library (not used but included if needed elsewhere)

// RFID pins
#define SS_PIN 5           // SDA for RFID
#define RST_PIN 21         // RST for RFID

#define Blue_LED 2         // Blue LED for authorized access
#define Red_LED 4          // Red LED for unauthorized access

MFRC522 rfid(SS_PIN, RST_PIN);    // Create MFRC522 instance

// Predefined UID for authorized card
byte authorizedUID[] = {0x0B, 0x23, 0x9B, 0x15}; // Replace with your actual UID

void setup() {
  Serial.begin(115200);

  pinMode(Blue_LED, OUTPUT);
  pinMode(Red_LED, OUTPUT);

  digitalWrite(Blue_LED, LOW);
  digitalWrite(Red_LED, LOW);

  SPI.begin();             // Init SPI bus
  rfid.PCD_Init();         // Init RFID reader
  Serial.println("Place your RFID card near the reader...");
}

void loop() {
  // Wait for a new RFID card
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("UID tag: ");
  String content = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
    content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(rfid.uid.uidByte[i], HEX));
  }
  Serial.println();

  // Check if UID is authorized
  bool authorized = true;
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] != authorizedUID[i]) {
      authorized = false;
      break;
    }
  }

  if (authorized) {
    Serial.println("Access Granted!");
    digitalWrite(Blue_LED, HIGH);
    delay(2000);
    digitalWrite(Blue_LED, LOW);
  } else {
    Serial.println("Access Denied!");
    digitalWrite(Red_LED, HIGH);
    delay(2000);
    digitalWrite(Red_LED, LOW);
  }

  rfid.PICC_HaltA();  // Stop reading
}
