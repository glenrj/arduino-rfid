/*Signal     Pin              Pin               Pin
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

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance.

void setup() {
	Serial.begin(9600);	// Initialize serial communications with the PC
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();	// Init MFRC522 card
	Serial.println("Scan PICC to see UID and type...");

 mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);
}

void loop() {
	// Look for new cards
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;//go to start of loop if there is no card present
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;//if ReadCardSerial returns 1, the "uid" struct (see MFRC522.h lines 238-45)) contains the ID of the read card.
	}

	// Dump debug info about the card. PICC_HaltA() is automatically called.
    mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
    delay(10);
}
