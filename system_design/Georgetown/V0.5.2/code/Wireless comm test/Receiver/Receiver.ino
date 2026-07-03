#include <SPI.h>
#include <RF24.h>


RF24 radio(6, 7);  // CE, CSN

const byte address[6] = "00001";

unsigned long receivedCounter;

void setup() {
  Serial.begin(9600);
  
  pinMode(3, OUTPUT);
  if (!radio.begin()) {
    Serial.println("NRF24 not found!");
    while (1);
  }


  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.startListening();

  Serial.println("Receiver Ready");
}

void loop() {
  if (radio.available()) {
    digitalWrite(3,HIGH);
    radio.read(&receivedCounter, sizeof(receivedCounter));
    
    Serial.print("Received: ");
    Serial.println(receivedCounter);
    delay(100);
    digitalWrite(3,LOW);
  }
}
