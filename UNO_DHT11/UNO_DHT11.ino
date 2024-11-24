#include <DHT11.h>
#include <SoftwareSerial.h>

#define DHT11_PIN 7
#define BUTTON_PIN 2
#define myRX_PIN 4
#define myTX_PIN 5

typedef struct { //If you use other then char or char[], specify in format mentioned below.
    uint32_t temperature;
    uint32_t humidity;
    uint32_t result;
} SensorData;

SensorData sensorData;

DHT11 dht11(DHT11_PIN);
SoftwareSerial mySerial (myRX_PIN, myTX_PIN);

int temperature=0, humidity=0,result;
volatile int reading=0;

void setup() {
    Serial.begin(9600);
    mySerial.begin(9600);
    pinMode(BUTTON_PIN,INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), setReading, FALLING);
}

void loop() {
  if(reading==1)
  {
    getTemperatureHumidity();
    sendSensorData();  
  }
  
  while(mySerial.available()){
      String messege = mySerial.readString();
      Serial.println(messege);
      if(messege=="GET data")
        setReading();
    }
}

void getTemperatureHumidity(){   
    result = dht11.readTemperatureHumidity(temperature, humidity);
    if (result == 0) {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.print(" °C\tHumidity: ");
        Serial.print(humidity);
        Serial.println(" %");
        reading=0;
    } else {
        // Print error message based on the error code.
        Serial.println(DHT11::getErrorString(result));
    }
}

void setReading(){
  reading=1;
}

void sendSensorData(){
  sensorData.temperature=temperature;
  sensorData.humidity=humidity;
  sensorData.result=result;
  int sizeSensorData = sizeof(SensorData);
  send(&sensorData,sizeSensorData);
}

void send(const SensorData* data, int sizeSensorData){
  mySerial.write((const char*)data, sizeSensorData); 
}
