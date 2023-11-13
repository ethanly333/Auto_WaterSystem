#include <LiquidCrystal.h>
#include <Time.h>

#define HOUR_DELAY 3600000
#define FIVEMIN_DELAY 300000
#define SENSOR1 A0
#define SENSOR2 A1
#define PUMP1 11
#define PUMP2 12
#define PUSHBTN 10
#define ANODE 9
#define NUM_SAMPLES 5

volatile unsigned int sensor1_data[NUM_SAMPLES] ;
volatile unsigned int sensor2_data[NUM_SAMPLES] ;
volatile unsigned int btnState = 0 ;

LiquidCrystal lcd(2, 3, 4, 5, 6, 7) ;

void setup() {
  Serial.begin(9600) ;
  lcd.begin(16, 2) ;
  //Sensor pin mode
  pinMode(SENSOR1, INPUT) ;
  pinMode(SENSOR2, INPUT) ;

  //Water pump pin mode
  pinMode(PUMP1, OUTPUT) ;
  pinMode(PUMP2, OUTPUT) ;

  //display features
  pinMode(PUSHBTN, INPUT) ;
  pinMode(ANODE, OUTPUT) ;
}

void loop() {
  int sensor_val1 = analogRead(A0) ;
  int sensor_val2 = analogRead(A1) ;

  //using the range of values 200 - 550 as found in testing
  float moisture1 = (sensor_val1-200)/350.00 ;
  float moisture2 = (sensor_val2-200)/350.00 ;

  btnState = digitalRead(PUSHBTN) ;
  if(btnState) {
    digitalWrite(ANODE, HIGH) ;
    lcd.noBlink() ;
    lcd.print("Plant 1:") ;
    lcd.setCursor(0, 1) ;
    lcd.print(100.00 - (float)moisture1*100) ;
    lcd.print("%") ;
    lcd.setCursor(0, 0) ;
    delay(5000) ;

    lcd.clear() ;
    
    lcd.print("Plant 2:") ;
    lcd.setCursor(0, 1) ;
    lcd.print(100.00 - (float)moisture2*100) ;
    lcd.print("%") ;
    lcd.setCursor(0, 0) ;
    delay(5000) ;
  }
  digitalWrite(ANODE, LOW) ;

  /************Testing************/
  //Serial.print(sensor_val2) ;
  //Serial.println() ;
  //delay(500) ;
  
  //Collect data every second for 5 min then check threshold
  unsigned int cur_millis = millis() ;
  if(cur_millis%1000 == 0)  //clock is 16MHz 
  {
    sensor1_data[(cur_millis/1000)-1] = sensor_val1 ;
    sensor2_data[(cur_millis/1000)-1] = sensor_val1 ;
  }
  //Serial.print(cur_millis) ;
  //Serial.println() ;
  if(cur_millis%(NUM_SAMPLES*1000) == 0)
  {
    //compute the avg sensor value after 5 min 
    int sensor1_avg = 0 ;
    int sensor2_avg = 0 ;
    for(int i=0; i<NUM_SAMPLES; i++)
    {
      sensor1_avg += sensor1_data[i] ;
      sensor2_avg += sensor2_data[i] ;
    }
    sensor1_avg /= NUM_SAMPLES ;
    sensor2_avg /= NUM_SAMPLES ;

    Serial.print(sensor1_avg) ;
    Serial.println() ;

    if(sensor1_avg > 325) {
      digitalWrite(PUMP1, HIGH) ; //pump water
      delay(2000) ;
      digitalWrite(PUMP1, LOW) ;
    }
    if(sensor2_avg > 325) {
      digitalWrite(SENSOR2, HIGH) ; //pump water
      delay(2000) ;
      digitalWrite(PUMP2, LOW) ;
    }
  } 
}
