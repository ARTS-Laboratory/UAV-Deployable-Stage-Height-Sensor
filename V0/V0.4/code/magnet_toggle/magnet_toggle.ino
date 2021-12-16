/*
   Written by Corinne Smith 11-2021
   Open the Serial monitor and follow instructions to toggle EPM on or off
*/

int PC = 5;         //enables the magnet control circuit
int MAG = 3;        //sends a PWM signal to the magnet
String command;     //stores the user's command

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PC, OUTPUT);
  pinMode(MAG, OUTPUT);
  Serial.println("Type in the Serial command line what you want to do: \n toggle magnet on: \"ON\" \n toggle magnet off: \"OFF\" ");

}

void loop() {

  // Take user input from Serial
  if (Serial.available()) {
    command = Serial.readStringUntil('\n');

    //ON CODE
    if (command.equals("ON")) {
      digitalWrite(PC, HIGH);
      delay(500);
      for (int i = 0; i < 30; i++) {
        digitalWrite(MAG, HIGH);
        delay(2);
        digitalWrite(MAG, LOW);
        delay(18);
      }
      Serial.println("Magnet on");
    }

    //OFF CODE
    else if (command.equals("OFF")) {
      for (int i = 0; i < 30; i++)
      {
        digitalWrite(MAG, HIGH);
        delay(1.5);
        digitalWrite(MAG, LOW);
        delay(19);
      }
      Serial.println("Magnet off");
    }

    else {
      Serial.println("Command not recognized");
    }
  }
}
