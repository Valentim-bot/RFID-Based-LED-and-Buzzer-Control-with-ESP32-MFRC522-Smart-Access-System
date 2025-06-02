/*
  RFID UID Reader (Serial Only)
  -----------------------------
  Wiring (MFRC522 → ESP32 ):
    SDA  → GPIO5   (SS)         — change SS_PIN below if you wire SS elsewhere
    SCK  → GPIO18   (SCK)
    MOSI → GPIO23   (MOSI)
    MISO → GPIO19   (MISO)
    RST  → GPIO21    — change RST_PIN below if you wire RESET elsewhere
    3V3  → 3.3 V
    GND  → GND
*/

#include <SPI.h>          // Hardware‑SPI for MFRC522
#include <MFRC522.h>      // RFID driver

// -----------------------------------------------------------------------------
// Pin definitions – adjust to match your wiring
// -----------------------------------------------------------------------------
#define SS_PIN   5        // MFRC522 SS  (SDA)  — change if needed
#define RST_PIN  21       // MFRC522 RST — change if needed

MFRC522 rfid(SS_PIN, RST_PIN);   // RFID reader instance

void setup() {
  Serial.begin(115200);            // Open serial port
  while (!Serial) { /* wait for native USB */ }

  SPI.begin();                   // Init SPI bus
  rfid.PCD_Init();               // Init MFRC522 reader

  Serial.println("Ready – Scan an RFID tag…");
}

void loop() {
  
  // Abort early if no new card is present
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  // ---------------------------------------------------------------------------
  // A tag has been detected – print its UID:
  // ---------------------------------------------------------------------------
  Serial.print(F("UID tag : "));
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) Serial.print('0');  // leading zero for one‑digit hex
    Serial.print(rfid.uid.uidByte[i], HEX);
    if (i < rfid.uid.size - 1) Serial.print(' ');       // space between bytes
  }
  Serial.println();
  Serial.println();

  rfid.PICC_HaltA();             // Stop reading this PICC
}
