#include <avr/io.h>
#include <avr/interrupt.h>

#define INT_PIN PB4           
#define PCINT_VECTOR PCINT0_vect      // This step is not necessary - it's a naming thing for clarit

// Out Pins
#define ALARM_PIN PB2
#define INDICATOR_PIN PB1

// In Pins
#define SENSIBILTY_PIN PB3

int IDLE_TIME = 4; //two minutes to restart counter if the alar is not enable //Duration of alarm 36.000 milliseconds

int sensibilityValue;
unsigned int vibrationsCounter = 0;  // counter for the number vibrations
unsigned int vibrationsBefore = 0;  // counter for the number vibrations
unsigned long startIdleTime = 0; //Get current time to reset
bool alarm = false;

void setup() {

  delay(500);

  // put your setup code here, to run once:
  pinMode(ALARM_PIN, OUTPUT);
  pinMode(INDICATOR_PIN, OUTPUT);
  pinMode(SENSIBILTY_PIN, INPUT);

  vibrationsCounter = 0;
  vibrationsBefore = 0;
  startIdleTime = 0;

  delay(10000);

  tones(100);
  
  cli();                            // Disable interrupts during setup
  
  sensibilityValue = analogRead(SENSIBILTY_PIN)*100; /// 4500 y 2200
  alarm = false;

  PCMSK |= (1 << INT_PIN);          // Enable interrupt handler (ISR) for our chosen interrupt pin (PCINT1/PB1/pin 6)
  GIMSK |= (1 << PCIE);             // Enable PCINT interrupt in the general interrupt mask
  pinMode(INT_PIN, INPUT_PULLUP);

  sei(); 
     // Set our interrupt pin as input with a pullup to keep it stable

}

void loop() {
  
  // if time beetwen vibration is more than IDLE_TIME restart counter.
  if (startIdleTime >= IDLE_TIME){
    startIdleTime = 0;
    vibrationsCounter = 0;
    alarm = false;
  } 

  if(vibrationsCounter >= sensibilityValue){
    alarm = true;
  }

  if( alarm ){
    tones(10);
    delay(5000);
    tones(50);
    delay(10000);
    startIdleTime++;
  }

  delay(100);

}

void tones(int counts){
  for(int count = 0; count < counts; count++){
    digitalWrite(ALARM_PIN, HIGH);
    digitalWrite(INDICATOR_PIN, HIGH);
    delay(50);
    digitalWrite(ALARM_PIN, LOW);
    digitalWrite(INDICATOR_PIN, LOW);
    delay(50);
    digitalWrite(ALARM_PIN, HIGH);
    digitalWrite(INDICATOR_PIN, HIGH);
    delay(200);
    digitalWrite(ALARM_PIN, LOW);
    digitalWrite(INDICATOR_PIN, LOW);
    delay(50);
  }
}

ISR(PCINT_VECTOR)
{
  vibrationsCounter++;
}