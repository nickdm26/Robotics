#include <DHT.h>
#include <ESP8266WiFi.h>

#define DHTPIN 5
#define DHTTYPE DHT22
#define AP_SSID "OP-Guest"
#define AP_PASSWORD "opwireless"

DHT dht(DHTPIN, DHTTYPE, 30);
WiFiClient client;
float hum;
float temp;

void setup() {
  dht.begin();
  WiFi.begin(AP_SSID, AP_PASSWORD); 
  Serial.begin(9600);
}

void loop() {
  delay(300000);
  getTemp();  
  WiFi.status() != WL_CONNECTED;
  Serial.println(WiFi.status() == WL_CONNECTED);
  ApiRequest();
}


void getTemp(){
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  //Serial.print(hum);
  //Serial.print("      ");
  //Serial.println(temp);
}

void ApiRequest(){
  const char* host = "api.thingspeak.com";
  const int httpPort = 80;
  String apiKey = "QKYX1Q7BYU4PDK1T";
  
  client.connect(host, httpPort);
  
  //# Constructing ~ThingSpeak URL
  String url = "/update?key=";
    url += apiKey;
    url += "&field1=";
    url += temp;
    url += "&field2=";
    url += hum; 
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");
}

void ApiResponse(){
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

client.stop();
}

