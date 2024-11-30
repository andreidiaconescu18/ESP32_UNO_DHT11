#include "arduino_secrets.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <StringSplitter.h>
#include <ESPAsyncWebServer.h>
#include <LiquidCrystal_I2C.h>

#define RX2_PIN 16
#define TX2_PIN 17

const char* ssid = SECRET_SSID; // Your Wi-Fi SSID
const char* password = SECRET_PASSWORD; // Your Wi-Fi Password

String serverName = SERVER_NAME; 

String dateTimeServer = "https://timeapi.io/api/time/current/zone?timeZone=Europe%2FBucharest"; //Server for date and time

typedef struct { //If you use other then char or char[], specify in format mentioned below.
    uint32_t temperature;
    uint32_t humidity;
    uint32_t result;
} SensorData;

AsyncWebServer server(80);
LiquidCrystal_I2C lcd(0x27, 16, 2);

SensorData sensorData;


void setup() {
  Serial.begin(9600);
  Serial2.begin(9600,SERIAL_8N1,RX2_PIN,TX2_PIN);

  lcd.init();
  lcd.backlight();
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  server.on("/getSensorData", HTTP_GET, [](AsyncWebServerRequest* request) {
    getSensorData();
    request->send(200, "text/html", "<html><body><h1>You will receive new data!</h1></body></html>");
  });

   // Start the server
  server.begin();
}

void loop() {
  while(Serial2.available())
  {
    receive(&sensorData);
    int temperature=sensorData.temperature;
    int humidity=sensorData.humidity;
    String date=getCurrentDate();
    Serial.println(temperature);
    Serial.println(humidity);
    Serial.println(date);
    sendToDatabase(temperature,humidity,date);
    printDataOnLCD(temperature,humidity,date);
  }
}

String getCurrentDate(){
  HTTPClient http;
  String date="";
  String urlPath=dateTimeServer;
  http.begin(urlPath);
  int httpResponseCode = http.GET();
  if (httpResponseCode>0) {
      String payload = http.getString();
      date=parsePayload(payload);
    }
  http.end();
  return date;
}

String parsePayload(String payload){
  StringSplitter *splitter = new StringSplitter(payload, ',', 10);
  String item = splitter->getItemAtIndex(7);
  splitter = new StringSplitter(item, '"', 10);
  item = splitter->getItemAtIndex(2);
  splitter = new StringSplitter(item, '.', 10);
  item = splitter->getItemAtIndex(0);
  splitter = new StringSplitter(item, 'T', 10);
  item = splitter->getItemAtIndex(0)+" "+splitter->getItemAtIndex(1);
  return item;
}

bool receive(SensorData* sd){
  return (Serial2.readBytes((char*)sd, sizeof(SensorData)) == sizeof(SensorData));
}

void sendToDatabase(int temperature, int humidity, String date){
  HTTPClient http;
  String urlPath="http://"+serverName+"/addSensorData.php";
  String httpRequestData ="temperature="+String(temperature)+"&humidity="+String(humidity)+"&date="+date;
  http.begin(urlPath);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded"); 
  int httpResponseCode = http.POST(httpRequestData);
  if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
  http.end();
}

void printDataOnLCD(int temperature, int humidity, String date){
   lcd.setCursor(0, 0);
   lcd.print("                ");
   lcd.setCursor(0, 0);
   lcd.print(date);
   lcd.setCursor(0, 1);
   lcd.print("                ");
   lcd.setCursor(0, 1);
   lcd.print("T: "+String(temperature)+" C H: "+String(humidity)+"%");
}

void getSensorData(){
  Serial2.print("GET data");
}
