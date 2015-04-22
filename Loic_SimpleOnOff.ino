
#include <Button.h>        //https://github.com/JChristensen/Button
#include <Time.h>
#include <TimeAlarms.h>      // http://www.pjrc.com/teensy/td_libs_TimeAlarms.html
#include <avr/wdt.h>         // watchdog

#define BUTTON_PIN 2       //Connect a tactile button switch (or something similar)
                           //from Arduino pin 2 to ground.
#define BUZZER_PIN  8      
#define RELAY_PIN   7

#define PULLUP true        //To keep things simple, we use the Arduino's internal pullup resistor.
#define INVERT true        //Since the pullup resistor will keep the pin high unless the
                           //switch is closed, this is negative logic, i.e. a high state
                           //means the button is NOT pressed. (Assuming a normally open switch.)
#define DEBOUNCE_MS 20     //A debounce time of 20 milliseconds usually works well for tactile button switches.
#define LED_PIN 13         //The standard Arduino "Pin 13" LED

Button myBtn(BUTTON_PIN, PULLUP, INVERT, DEBOUNCE_MS);    //Declare the button
boolean isWorking;          //A variable that keeps the current Working Status

void setup(void)
{
  Serial.begin(57600);
  Serial.println("Hello World !");
  isWorking = false;  // We are not Working yet
    pinMode(LED_PIN, OUTPUT);    //Set the LED pin as an output
  // BUZZER SETUP
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);    // No Relay Connection
  wdt_enable(WDTO_2S);// Enable WatchDog every 2Seconds
  
}

void connectRelay (/* ms duration for connection or 0 for infinite */ int duration) {
  Serial.println("Relay Opening");
  // Make your stuff here
    digitalWrite(RELAY_PIN, HIGH);
    if (duration>0) {
        delay(duration); // if Microseconds needed chnage delay() by delayMicroseconds() !
        digitalWrite(RELAY_PIN, LOW);
        Serial.println("Relay Stopped by delay duration");
    }
}

void stopRelay() {  // Not used in pulse needed scenario 
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("Relay Stopped.");
}

void beep() {
  digitalWrite(BUZZER_PIN, LOW);
  delay(150);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(50);
  digitalWrite(BUZZER_PIN, LOW);
  delay(150);
  digitalWrite(BUZZER_PIN, HIGH);  
  Serial.println("Beep ... !! ...");
}


void doStop() {
    if (isWorking) {
    connectRelay(100); // Assume 100ms impulsion
    digitalWrite(LED_PIN, LOW);   // Turn OFF the Led of the Arduino (Not Working)
    Serial.println("STOP WORKING");
    } else {
          Serial.println("ERROR - STOP WORKING");
          beep();beep();
    }
   isWorking = false;
}

void doStart() {
  if (!isWorking) {
    isWorking = true;
    connectRelay(100); // Assume 100ms impulsion
    digitalWrite(LED_PIN, HIGH);   // Turn ON the Led of the Arduino (Working)    
    Serial.println("START WORKING");
    } else {
          Serial.println("ERROR - START WORKING");
          beep();beep();   
    }
}


void loop(void)
{
 
    myBtn.read();                    //Read the button
    if (myBtn.wasReleased()) {       //If the button was released, change the LED state
        beep(); delay(500);
        if (!isWorking) {
            doStart(); // Call for opening the pump
            Alarm.timerOnce(30, doStop); // in 30 Seconds, we will trigger doStop
        } else { // Assume we have to stop immediately
        doStop();
        }
    }
  wdt_reset();  // RIP WatchDog
  Alarm.delay(5);  // Check Alarms with 5ms delay
    
}


