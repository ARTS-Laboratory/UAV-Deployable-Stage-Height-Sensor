#include <SPI.h>
#include <RF24.h>
#include <printf.h> // Required for printDetails() to work

// CE, CSN pin definitions
RF24 radio(6, 7);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect (needed for Leonardo/Micro/native USB)
  }
  
  // Initialize printf library so the radio can format its register report
  printf_begin();
  
  Serial.println(F("===================================="));
  Serial.println(F("  nRF24L01+ CHIP DIAGNOSTIC TEST    "));
  Serial.println(F("===================================="));

  if (!radio.begin()) {
    Serial.println(F("ERROR: Radio hardware not responding at all!"));
    Serial.println(F("Check your VCC, GND, and SPI (MOSI/MISO/SCK) wiring."));
    while (1);
  }

  // 1. Try setting the data rate to 250 KBPS. 
  // Genuine Nordic nRF24L01+ chips fully support this.
  // Many counterfeit clones only support 1MBPS or 2MBPS and will ignore this command.
  radio.setDataRate(RF24_250KBPS);
  
  // 2. Set a known dummy address to see if the registers hold it accurately
  const byte testAddress[6] = "DIAGN";
  radio.openWritingPipe(testAddress);
  radio.openReadingPipe(1, testAddress);

  // 3. Print the raw register dump to the Serial Monitor
  radio.printDetails();
  
  Serial.println(F("===================================="));
}

void loop() {
  // Nothing to loop, we only need to read the registers once at boot
}
