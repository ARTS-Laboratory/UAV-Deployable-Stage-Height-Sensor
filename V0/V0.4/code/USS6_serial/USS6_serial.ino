 /*
 * Written by Corinne Smith
 * July 9 2021
 * Modified from ArduinoGetStarted, www.ArduinoGetStarted.com
 * Modified from Dejan Nedelkowski, www.HowToMechatronics.com
 * Modified from ArduinoLearning, www.ArduinoLearning.com
 */

#include <SD.h>                         // for SD card module
#include <SPI.h>                        // for SD card and nRF
#include <Wire.h>                       // for I2C
#include <HCSR04.h>                     // for the USS
#include <RF24.h>                       // for the nRF
#include <DS3232RTC.h>                  // RTC Library https://github.com/JChristensen/DS3232RTC
#include <avr/sleep.h>                  // for sleep mode
#include <Adafruit_Sensor.h>            // for BME 
#include <Adafruit_BME280.h>            // for BME
#include <Adafruit_INA219.h>            // for voltage monitor


#define CE 7
#define CSN 6
#define MAX_DAT_SIZE 16
#define RTCinterrupt 2                  // RTC interrupt from sleep mode
#define nRFinterrupt 3
#define SEALEVELPRESSURE_HPA (1013.25)


//RF ----------------------------------------------------------------------------------------------------
  RF24 radio(CE, CSN); 
  const byte addresses [][6] = {"00001", "00002", "00003", "00004"};  // Package addresses. Transmitter address is 00001, receiver addresses can be configured as 00002-00005
  int command = 0;
  boolean mag_on = false;
  boolean paired = false;
  boolean just_set = false;

//Ultrasonic ----------------------------------------------------------------------------------------------
  const int trigPin = 9;         
  const int echoPin = 8;         
  UltraSonicDistanceSensor distanceSensor(trigPin, echoPin);  

//SD ------------------------------------------------------------------------------------------------------
  const int pinCS = 10;          //defines ChipSelect pin for SD module

//BME -----------------------------------------------------------------------------------------------------
  Adafruit_BME280 bme;

//Voltage Regulator ---------------------------------------------------------------------------------------
  Adafruit_INA219 ina219;

//control pins
  const int LED = A3;
  const int magPin = 5;
  const int magPower = 4;
  const int time_interval = 5;  // interval in which the minutes will be delayed
  
void setup() {
    Serial.begin(9600);
    pinMode(pinCS, OUTPUT);            
    pinMode(LED, OUTPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(RTCinterrupt,INPUT_PULLUP); //Set pin d2 to input using the buildin pullup resistor
    pinMode(nRFinterrupt, INPUT_PULLUP);
    pinMode(magPower, OUTPUT);
    pinMode(magPin, OUTPUT);
    Serial.println("Executing USS6_serial.ino");

// nRF24L01 initialization --------------------------------------------------------------------------------------------------------------------------
    radio.begin();
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1, addresses[1]);       //Setting the address at which we will receive the data. This is receiver package with address 00002
    radio.setPALevel(RF24_PA_MIN);                //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
    if (radio.setDataRate(RF24_1MBPS) == true)
      {
         Serial.println("DATA RATE SET");
      }
    
// SD card initialization --------------------------------------------------------------------------------------------------------------------------
   if (!SD.begin())               // tests if SD module began
      {
        digitalWrite(LED, HIGH);
        Serial.println("no SD found");
       while(1);                  // LED remains on if SD card does not work
     }
     else
     {
      Serial.println("SD found");
     }

// RTC initializaiton ------------------------------------------------------------------------------------------------------------------------------
    RTC.begin();
    // initialize the alarms to known values, clear the alarm flags, clear the alarm interrupt flags
    RTC.setAlarm(ALM1_MATCH_DATE, 0, 0, 0, 1);
    RTC.setAlarm(ALM2_MATCH_DATE, 0, 0, 0, 1);
    RTC.alarm(ALARM_1);
    RTC.alarm(ALARM_2);
    RTC.alarmInterrupt(ALARM_1, false);
    RTC.alarmInterrupt(ALARM_2, false);
    RTC.squareWave(SQWAVE_NONE);
    time_t t;                               // create a temporary time variable so we can set the time and read the time from the RTC
    t = RTC.get();                            // Gets the current time of the RTC
//    if(second(t)<55){
//      RTC.setAlarm(ALM1_MATCH_SECONDS , second(t)+time_interval, 0, 0, 0);
//    }
//    else {
//      RTC.setAlarm(ALM1_MATCH_SECONDS , second(t)-60+time_interval, 0, 0, 0);
//    }
    if(minute(t)<55){
        RTC.setAlarm(ALM1_MATCH_MINUTES , 0, minute(t)+time_interval, 0, 0);
      }
    else {
        RTC.setAlarm(ALM1_MATCH_MINUTES , 0, minute(t)-60+time_interval, 0, 0);
    }
    // clear the alarm flag
    RTC.alarm(ALARM_1);
    // configure the INT/SQW pin for "interrupt" operation (disable square wave output)
    RTC.squareWave(SQWAVE_NONE);
    // enable interrupt output for Alarm 1
    RTC.alarmInterrupt(ALARM_1, true);
    
// BME initialization ------------------------------------------------------------------------------------------------------------------------------
    bme.begin(0x76);

// Voltage regulator initialization -----------------------------------------------------------------------------------------------------------------
    uint32_t currentFrequency;
    ina219.begin();
  }

void loop() {
    digitalWrite(LED, LOW);
    radio.startListening();     //Makes module a receiver
    delay(100);
    goSleep();
  }

void goSleep(){
    Serial.println("Going to sleep...");
    just_set = false;
    delay(100);
    sleep_enable();                               // Enable sleep mode
    attachInterrupt(0, RTCtrigger, LOW);              // attach an interrupt to pin 2 and run the wakeUp() function upon waking
    attachInterrupt(1, nRFtrigger, LOW);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);          // Set full sleep mode
    sleep_cpu();                                  // Activate sleep mode
    if(just_set == false) {
      logData();                                    // Run the logData() function after the wakeUp() function
      time_t t;
      t=RTC.get(); 
      //Set New Alarm
  //    if(second(t)<55){
  //      RTC.setAlarm(ALM1_MATCH_SECONDS , second(t)+time_interval, 0, 0, 0);
  //    }
  //    else {
  //      RTC.setAlarm(ALM1_MATCH_SECONDS , second(t)-60+time_interval, 0, 0, 0);
  //    }
      if(minute(t)<55){
          RTC.setAlarm(ALM1_MATCH_MINUTES , 0, minute(t)+time_interval, 0, 0);
        }
      else {
          RTC.setAlarm(ALM1_MATCH_MINUTES , 0, minute(t)-60+time_interval, 0, 0);
      }
      // clear the alarm flag
      RTC.alarm(ALARM_1);
     }
  }
  
void RTCtrigger() {
    Serial.println("RTC interrupt fired");
    delay(100);
    sleep_disable();                        // Disable sleep mode
    detachInterrupt(0);                     // Remove the interrupt from pin 2
}

void nRFtrigger() {
  Serial.println("nRF interrupt fired");
  delay(100);
  sleep_disable();
  detachInterrupt(1);
  delay(2000);
  
  bool tx,fail,rx;
  radio.whatHappened(tx,fail,rx);                     // What happened?
  
  if ( rx || radio.available()){                      // Did we receive a message?
    radio.read(&command, sizeof(command));
    Serial.println(command);
    delay(500);
    if(command == 1) {
      Serial.println("Paired to receiver");
      radio.stopListening();
      delay(50);
      radio.write(&command, sizeof(command));
      delay(50);
      radio.startListening();
      just_set = true;
    }
    if(command == 2 && mag_on == false) { 
      Serial.println("toggling on");
      mag_on = true;
    }
    else if(command == 3 && mag_on == true) { 
      Serial.println("toggling off");
      mag_on = false;
    }
  }
  toggleMag();
}

void toggleMag() {
  if(mag_on == true) {
      digitalWrite(magPower, HIGH);
      delay(1500);
      for (int i = 0; i < 30; i++)
        {
          digitalWrite(magPin, HIGH);
          delay(2);
          digitalWrite(magPin, LOW);
          delay(18);
        }
        Serial.println("Mag on");
        delay(2000);
        digitalWrite(magPower, LOW);
    }
    else if(mag_on == false) {
      digitalWrite(magPower, HIGH);
      for (int i = 0; i < 30; i++)
        {
          digitalWrite(magPin, HIGH);
          delay(1.5);
          digitalWrite(magPin, LOW);
          delay(19);
        }
        Serial.println("Mag off");
        delay(4000);
        digitalWrite(magPower, LOW);
    }
}

void logData() {
      digitalWrite(LED, HIGH);    
      delay(10);
      
    // for the USS Module
      float dist[5];
      float total = 0;
      float num = 0;
      for(int i=0; i<5; i++) {
        dist[i] = distanceSensor.measureDistanceCm();
        delay(500);
        if(dist[i] > 0) {
          total = total + dist[i];
          num++;
        }
      }
      float avgDist = total/num;

    // for the BME
      float temp = bme.readTemperature();
      float humidity = bme.readHumidity();
      float pressure = bme.readPressure() / 100.0F;
      delay(50);

    // for the voltage regulator
      float busvoltage = ina219.getBusVoltage_V();
      float current_mA = ina219.getCurrent_mA();
      float power_mW = ina219.getPower_mW();

//      while(busvoltage < 6.65) {
//        Serial.print("Voltage: "); Serial.println(busvoltage);
//        tone(buzz, 500, 3000);
//        delay(1000);
//        busvoltage = ina219.getBusVoltage_V();
//      }

    // Now print the data to the file that will be saved on the SD card
      File myFile = SD.open("002.csv", FILE_WRITE);

      if (myFile)                         // tests if the file has opened
        {
          // write the RTC data
          time_t t = RTC.get();
          myFile.print(String(month(t)));
          myFile.print("/");
          myFile.print(String(day(t)));
          myFile.print("/");
          myFile.print(String(year(t)));
          myFile.print(" ");
          myFile.print(String(hour(t)));
          myFile.print(":");
          myFile.print(String(minute(t)));
          myFile.print(":");
          myFile.print(String(second(t)));
          myFile.print(" ");
      
          // write the USS data
          for(int i=0; i<5; i++) {
            myFile.print(dist[i]); myFile.print(" ");
          }
          myFile.print(avgDist); myFile.print(" ");

          // write the BME data
          myFile.print(temp); myFile.print(" ");
          myFile.print(humidity); myFile.print(" ");
          myFile.print(pressure); myFile.print(" ");
 

          // write the voltage data
          myFile.print(busvoltage); myFile.print(" ");
          myFile.print(current_mA); myFile.print(" ");
          myFile.print(power_mW); myFile.print(" ");

          myFile.println("");
          myFile.close();                   // closes and saves the file to the SD card
        }
      else
        {
          digitalWrite(LED, HIGH);         // if the file doesn't open indicate with the LED 
          while(1);
        }
  delay(100);
  digitalWrite(LED, LOW);
  
}
