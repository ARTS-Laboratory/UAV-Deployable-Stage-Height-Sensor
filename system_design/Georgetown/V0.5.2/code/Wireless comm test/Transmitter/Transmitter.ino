#include <SPI.h>
#include <RF24.h>

RF24 radio(6, 7);  // CE, CSN

const byte address[6] = "00001";

unsigned long counter = 0;

void setup() {
  Serial.begin(9600);

  if (!radio.begin()) {
    Serial.println("NRF24 not found!");
    while (1);
  }

  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.stopListening();

  Serial.println("Transmitter Ready");
}

void loop() {
  bool success = radio.write(&counter, sizeof(counter));

  if (success) {
    Serial.print("Sent: ");
    Serial.println(counter);
    counter++;
  } else {
    Serial.println("Send failed");
  }

  delay(1000);
}
