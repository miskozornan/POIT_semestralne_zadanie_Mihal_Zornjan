#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11   // DHT 11

int sensorPin = A0;    // Analog select the input pin for the potentiometer
int soilMoisture = 0;  // The variable to store the value coming from the sensor
const int RELAY_PIN = A5;  // the Arduino pin, which connects to the IN pin of relay
int data[30];
int count = 0, i = 0;
int avgMoistValue = 0;
bool moistReady = false;
String dataPi = "30";

DHT dht(DHTPIN, DHTTYPE);

void setup() {
   //pinMode(myLed, OUTPUT);
   Serial.begin(9600);  //Start Serial Communication at boud rate 9600 To verify the ourput
   // initialize digital pin A5 as an output.
   pinMode(RELAY_PIN, OUTPUT);
   Serial.println(F("DHTxx test!"));

   dht.begin();
}

void loop() {
  //delay(3000);
  soilMoistureFcn();
  //startPump();
  //DHTsensorFcn();              
}

void soilMoistureFcn(){
  
  digitalWrite(RELAY_PIN, HIGH);
       
  soilMoisture = analogRead(sensorPin); //Reads the value from the sensor
  delay(1000);    // 1 sec 
  soilMoisture = map(soilMoisture,1023,190,0,100);
  
  if(count < 30){
    data[count] = soilMoisture;
//    Serial.print(count);
//    Serial.print("Data:");
//    Serial.println(data[count]);
    count++;    
  } 
  if(count == 30){
      for(i = 0; i < 30; i++){
      avgMoistValue = avgMoistValue + data[i];
      //Serial.print("AvgMoistValue: ");
      //Serial.println(avgMoistValue);
      //Serial.print("data[i]: ");
      //Serial.println(data[i]);
      
    }

    avgMoistValue = avgMoistValue/30;
    Serial.print("final sensor value = " );
    Serial.print(avgMoistValue);
    Serial.println(" %");
    count = 0;
    moistReady = true;
    DHTsensorFcn();
  } else {
    moistReady = false;
  }

  
  if(Serial.available() > 0){
    dataPi = Serial.readStringUntil('\n');
    Serial.print("You sent me: ");
    Serial.println(dataPi);
    DHTsensorFcn();
//    if(count == 30){
//      Serial.print("Moist value:");
//      Serial.println(avgMoistValue);
//    }
  }
    if(avgMoistValue < dataPi.toInt() && moistReady == true){   // ak je zem menej ako 30% mokra, tak sa zapne motor
      startPump();
      moistReady = false;
    
    }

    avgMoistValue = 0;
    
}

void startPump(){
  digitalWrite(RELAY_PIN, HIGH); // turn on pump 5 seconds
  delay(5000);
  digitalWrite(RELAY_PIN, LOW);  // turn off pump 5 seconds
  delay(1000);
  digitalWrite(RELAY_PIN, HIGH);
}

void DHTsensorFcn(){
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
}
