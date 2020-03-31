//RFID Write

/*
 Signal     Pin              Pin               Pin
 *            Arduino Uno      Arduino Mega      MFRC522 board
 * ------------------------------------------------------------
 * Reset      9                5                 RST
 * SPI SS     10               53                SDA
 * SPI MOSI   11               51                MOSI           YLLW      
 * SPI MISO   12               50                MISO           BRN
 * SPI SCK    13               52                SCK            PURP
 * 
  */

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN1 30
#define RST_PIN1 32
MFRC522 mfrc522A (SS_PIN1, RST_PIN1);  // Create MFRC522 instance.

#define SS_PIN2 34
#define RST_PIN2 36
MFRC522 mfrc522B (SS_PIN2, RST_PIN2);

MFRC522::MIFARE_Key key;

void setup() {
  
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();      // Init SPI bus


  
  mfrc522A.PCD_Init(); // Init MFRC522 card
  mfrc522B.PCD_Init(); // Init MFRC522 card

  for (byte i = 0; i < 6; i++) {
     key.keyByte[i] = 0xFF;
  }
  Serial.println("Gimme the goood stuff...");

 dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
}



void loop () {

   byte sector         = 1;
   byte blockAddr      = 4;
   byte dataBlock[]    = {
        0x01, 0x02, 0x03, 0x04, //  1,  2,   3,  4,
        0x05, 0x06, 0x07, 0x08, //  5,  6,   7,  8,
        0x09, 0x0a, 0xff, 0x0b, //  9, 10, 255, 11,
        0x0c, 0x0d, 0x0e, 0x0f  // 12, 13, 14, 15
    };
   byte trailerBlock   = 7;
   MFRC522::StatusCode status;
   byte buffer[18];
   byte size = sizeof(buffer);
  
  if (  mfrc522A.PICC_IsNewCardPresent()) {

    // Authenticate using key A
    Serial.println(F("Authenticating using key A..."));
    status = (MFRC522::StatusCode) mfrc522A.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522A.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522A.GetStatusCodeName(status));
        return;
    }
    
    Serial.print(F("Writing data into block ")); Serial.print(blockAddr);
    Serial.println(F(" ..."));
    dump_byte_array(dataBlock, 16); Serial.println();
    mfrc522A.MIFARE_Write(blockAddr, dataBlock, *buffer);
    status = (MFRC522::StatusCode) mfrc522A.MIFARE_Write(blockAddr, dataBlock, *buffer);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc522A.GetStatusCodeName(status));
    }
    Serial.println();
  }
  
  }


void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}
