
/* Arduino UNO*/

const uint8_t IN_LIGHT = A0;  // Configure light sensor analog input (values 0-1023)
const uint8_t OUT_LED = 7;    // configure LED output to digital pin 
const int LIGHT_THRESHOLD = 200;  // LED turn on/off level





void setup() {
  // put your setup code here, to run once:
  pinMode(OUT_LED, OUTPUT);
  Serial.begin(9600);
}



void loop() {
  // put your main code here, to run repeatedly:
  int sensorValue = analogRead(IN_LIGHT);
  if(sensorValue < LIGHT_THRESHOLD) 
    digitalWrite(OUT_LED, HIGH);
  else
    digitalWrite(OUT_LED, LOW);

  //sensorValue /= 10;

  String g = "";
  //for(int i = 0; i < sensorValue; i++) {
    //g += " ";
  //}
  g += ". " + String(sensorValue);
  Serial.println(g);

  delay(10);

}
