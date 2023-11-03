
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600) ;
  pinMode(A0, INPUT) ;
  pinMode(2, OUTPUT) ;
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensor_val = analogRead(A0) ;
  Serial.print(sensor_val) ;
  Serial.println() ;
  delay(500) ;

  digitalWrite(2, HIGH) ;
  delay(1000) ;
  digitalWrite(2, LOW) ;
  delay(1000) ;
}
