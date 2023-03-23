// Out Pins
int ALARM_PIN = 2;
int INDICATOR_PIN = 1;

// In Pins
int SENSIBILTY_PIN = 3;
int SENSOR_PIN = 4;

long IDLE_TIME = 120000; //two minutes to restart counter if the alar is not enable

int sensibilityValue;
unsigned int vibrationsCounter = 0;  // counter for the number vibrations
bool sensorState = false;        // current state of the sensor
bool lastSensorState = false;
long startIdleTime = 0; //Get current time to reset

bool alarm = false;

void setup() {

  delay(500);

  // put your setup code here, to run once:
  pinMode(ALARM_PIN, OUTPUT);
  pinMode(INDICATOR_PIN, OUTPUT);
  pinMode(SENSIBILTY_PIN, INPUT);
  pinMode(SENSOR_PIN, INPUT);
  
  //sensibilityValue = MIN_SENSIBILITY + ( (analogRead(SENSIBILTY_PIN) * (MAX_SENSIBILITY - MIN_SENSIBILITY)) / 1023);

  sensibilityValue = analogRead(SENSIBILTY_PIN)*2;
  alarm = false;

  tones(2);

  delay(100);

  /*
    pinMode(CONFIGURE_PIN,INPUT);
    // if configure is true wait to define sensibility 
    while () {
19	    ; // wait for serial port to connect. Needed for native USB port only
        push 1 sensibility 1 push 2 sesibility 2 push 3 sensibility 3 push 4 reset sensibility to sensibility 0
20	  }
  */
}

void loop() {
  // put your main code here, to run repeatedly: 
  
  sensorState = digitalRead(SENSOR_PIN);

  // compare the buttonState to its previous state
  if (sensorState != lastSensorState) {
    if (sensorState == HIGH) {
      vibrationsCounter++;
      startIdleTime = millis(); //get time after every vibration
    }
    lastSensorState = sensorState;
  } 

  // if time beetwen vibration is more than IDLE_TIME restart counter.
  vibrationsCounter = ((millis()-startIdleTime) >= IDLE_TIME) ? 0 : vibrationsCounter; 

  if(vibrationsCounter >= sensibilityValue && !alarm){
    alarm = true;
  }

  if(alarm){
    tones2(10);
    delay(5000);
    tones2(50);
    delay(10000);
  }

  delay(100);

}

void tones(int counts){
  for(int count = 0; count < counts; count++){
    digitalWrite(ALARM_PIN, HIGH);
    digitalWrite(INDICATOR_PIN, HIGH);
    delay(100);
    digitalWrite(ALARM_PIN, LOW);
    digitalWrite(INDICATOR_PIN, LOW);
    delay(50);
  }
}

void tones2(int counts){
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