
#define HOUR_DELAY 3600000
#define FIVEMIN_DELAY 300000
#define SENSOR1 A0
#define SENSOR2 A1
#define PUMP1 2
#define PUMP2 3

volatile unsigned int[60] sensor1_data ;
volatile unsigned int[60] sensor2_data ;
volatile unsigned int sec_reload = 0 ;
volatile unsigned int hour_reload = 0 ;

void setup() {
  Serial.begin(9600) ;
  //Sensor pin mode
  pinMode(SENSOR1, INPUT) ;
  pinMode(SENSOR2, INPUT) ;

  //Water pump pin mode
  pinMode(PUMP1, OUTPUT) ;
  pinMode(PUMP2, OUTPUT) ;
}

void loop() {
  int sensor_val1 = analogRead(A0) ;
  int sensor_val2 = analogRead(A1) ;

  /************Testing************/
  //Serial.print(sensor_val2) ;
  //Serial.println() ;
  //delay(500) ;
  
  //Collect data every second for 5 min then check thresholds

  if(sec_reload%16000000 == 0)  //clock is 16MHz 
  {
    sec_reload = 0 ;
    sensor1_data[hour_reload] = sensor_val1 ;
    sensor2_data[hour_reload] = sensor_val1 ;
    hour_reload++ ;
  }
  sec_reload++ ;

  if(hour_reload%3600)
  {
    hour_reload = 0 ;

    //compute the avg sensor value after 5 min 
    int sensor1_avg = 0 ;
    int sensor2_avg = 0 ;
    for(int i=0; i<60; i++)
    {
      sensor1_avg += sensor1_data[i] ;
      sensor2_avg += sensor2_data[i] ;
    }
    sensor1_avg /= 60 ;
    sensor2_avg /= 60 ;

    if(sensor1_avg > 325)
      digitalWrite(SENSOR1, HIGH) ; //pump water
    if(sensor2_avg > 325)
      digitalWrite(SENSOR2, HIGH) ; //pump water
  } 
}
