/*
   Written by Corinne Smith 11-2021
   Upload this code to the rf controller
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9,10); // CE, CSN
const byte addresses [][6] = {"00001", "00002", "00003", "00004", "00005"};  // Package addresses. Transmitter address is 00001, receiver addresses can be configured as 00002-00005

// control buttons
int on_pin = 3;
int off_pin = 2;
boolean on_button = false;
boolean off_button = false;

// channel setting
int dial_pin = A0;
int read_channel = 0;
int pair_pin = 4;
boolean ch_set = false;

// indicator leds
int ch1 = 7;
int ch2 = 6;
int ch3 = 5;
int status_led = 8;

// rf ping
int command = 0;

void setup() {
  Serial.begin(9600);
  pinMode(on_pin, INPUT);
  pinMode(off_pin, INPUT);
  pinMode(pair_pin, INPUT);
  pinMode(ch1, OUTPUT);
  pinMode(ch2, OUTPUT);
  pinMode(ch3, OUTPUT);
  pinMode(status_led, OUTPUT);

  radio.begin();                                 // start the radio communication
  radio.openReadingPipe(1, addresses[0]);        // set the address at which we will receive the data as 00001
  radio.setPALevel(RF24_PA_MIN);                 // can be set as minimum or maximum depending on the distance between the transmitter and receiver
  Serial.println("Executing rf_controller.ino");
}


void loop()  {

  while (ch_set == false) {
    read_channel = analogRead(dial_pin);
    if (read_channel > 256 && read_channel <= 512) {
      digitalWrite(ch2, LOW);
      digitalWrite(ch3, LOW);
      digitalWrite(ch1, HIGH);
      if (digitalRead(pair_pin) == HIGH) {
        radio.openWritingPipe(addresses[1]);     // set the address to package 1
        sendPing();
      }
    }
    else if (read_channel > 512 && read_channel <= 768) {
      digitalWrite(ch1, LOW);
      digitalWrite(ch3, LOW);
      digitalWrite(ch2, HIGH);
      if (digitalRead(pair_pin) == HIGH) {
        radio.openWritingPipe(addresses[2]);     // set the address to package 2
        sendPing();
      }
    }
    else if (read_channel > 768 && read_channel <= 1023) {
      digitalWrite(ch2, LOW);
      digitalWrite(ch1, LOW);
      digitalWrite(ch3, HIGH);
      if (digitalRead(pair_pin) == HIGH) {
        radio.openWritingPipe(addresses[3]);     // set the address to package 3
        sendPing();
      }
    }
    else if (read_channel <= 256) {
      digitalWrite(ch2, LOW);
      digitalWrite(ch1, LOW);
      digitalWrite(ch3, LOW);
    }
    delay(50);
  }

  if (digitalRead(pair_pin) == HIGH) {           // used to unpair packages
    digitalWrite(status_led, LOW);
    ch_set = false;
  }

  radio.stopListening();                         // set module as the transmitter
  delay(5);
  command = 0;
  on_button = digitalRead(on_pin);
  off_button = digitalRead(off_pin);

  // send the ON command if the on button is pressed
  if (on_button == HIGH) {
    command = 2;
    delay(1000);                                // allow time to reassign the variable
    radio.write(&command, sizeof(command));     
    delay(5000);                                // delay ensures a button is not read twice
    Serial.print("Sent: "); Serial.println(command);
  }
  // send the OFF command if the off button is pressed
  else if (off_button == HIGH) {
    command = 3;
    delay(1000);                                // allow time to reassign the variable
    radio.write(&command, sizeof(command));     
    delay(5000);                                // delay ensures a button is not read twice
    Serial.print("Sent: "); Serial.println(command);
  }
  delay(200);
}

// validation function to make sure the receiver and transmitter have established communication
void sendPing() {                               
  radio.stopListening();                        // set module as the transmitter
  delay(20);
  command = 1;
  radio.write(&command, sizeof(command));       // send a validation ping
  radio.startListening();                       // set module as the receiver
  delay(20);
  if (radio.available()) {                      // check if the package nRF is available and transmitting a ping back
    radio.read(&command, sizeof(command));
    Serial.println("Ping successful");
    radio.stopListening();                      // set module back as the transmitter
    ch_set = true;
    digitalWrite(status_led, HIGH);             // status led turns on if validation ping is received
  }
  else {
    Serial.println("Paring failed, make sure the package addresses are correct and the package is on and in range");            
  }
  delay(2000);
//radio.stopListening();
//  delay(20);
//  command = 1;
//  radio.write(&command, sizeof(command));
//  radio.startListening();
//  delay(20);
//  if (radio.available()) {
//    radio.read(&command, sizeof(command));
//    Serial.println("Ping successful");
//    radio.stopListening();
//    ch_set = true;
//    digitalWrite(status_led, HIGH);
//  }
//  else {
//    Serial.println("paring failed");
//  }
//  delay(2000);
}
