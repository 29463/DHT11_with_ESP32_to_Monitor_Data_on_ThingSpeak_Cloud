#include<ESP8266WiFi.h>
#include<WiFi.h>
#include<WiFiClient.h>
#include<DHT.h>
#include"ThingSpeak.h"

String ssid = "Jazz 4G MIFI_6A5D";
String password = "18472398";

WiFiserver = server(80);
WiFiClient client;
unsigned long myChannelNumber = 1;
const char* mywriteAPIKey = "ZTYIZ2GKIXR08BTM";

unsigned long lastTime = 0;
unsigned long timerdelay = 1000;

#define DHTPIN 5
#define DHTTYPE DHT11

DHT dht(DHTPIN,DHTTYPE);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Connecting to .....");
  Serial.println(ssid);
  
  WiFi.begin(ssid,password);
  while(WiFi.status()!= WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.print("WiFi Connected.");
  Serial.print("IP Address");
  Serial.println(WiFi.localIP());

  server.begin();
  dht.begin();
  ThingSpeak.begin(client);

}
  
void loop() {
  // put your main code here, to run repeatedly:
  float H,T,F;
  if((millis()-lastTime)>timerdelay){
    delay(2000);
    float H = dht.readHumidity();
    float T = dht.readTemperature();
    float F = dht.readTemperature(true);

    if(isnan(H)||isnan(T)||isnan(F)){
      Serial.println(F("Failed to read From DHT Sensor"));
      return;
    }

  }
  Serial.print("Temperature: ");
  Serial.print(T);
  Serial.print("Humidity: ");
  Serial.println(H);

  ThingSpeak.setField(1,T);
  ThingSpeak.setField(2,H);

  ThingSpeak.writeFields(myChannelNumber,mywriteAPIKey);
  lastTime = millis();


}
