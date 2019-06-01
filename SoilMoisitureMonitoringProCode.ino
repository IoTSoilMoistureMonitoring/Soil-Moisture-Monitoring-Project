#include <ESP8266WiFi.h>
#include <PubSubClient.h>
int WET= 16; // Wet Indicator at Digital PIN D0
int DRY= 2;  // Dry Indicator at Digital PIN D4
int sense_Pin= A0; // Soil Sensor input at Analog PIN A0
int value= 0;
const char* ssid = "ClementRegi4";
const char* password = "44448888";
#define TOKEN "A1E-BgFDEzhAYKGbZJ1q1dpWDS5W36Fklw"
#define MQTT_CLIENT_NAME "Janvier"
#define Variable_label "moisture"
#define device_label "moisture-based-project"
const char* mqttbroker = "things.ubidots.com";
char payload[100];
char topic[50];
WiFiClient espClient;
PubSubClient client(espClient); 
//Set-Up WiFi
void setup_wifi() {
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(MQTT_CLIENT_NAME,TOKEN,""))
    {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 6 seconds before retrying
      delay(1000);
    }
  }
} 
void callback(char* topic, byte* payload, unsigned int length) 
{
  } 

void setup() {
  Serial.begin(9600);
  pinMode(WET, OUTPUT);
  pinMode(DRY, OUTPUT);
  Serial.println("Moisture test!");
  setup_wifi();
  client.setServer(mqttbroker, 1883);
  reconnect();
}

void loop() {
  client.setCallback(callback);
  if (!client.connected()) {
  reconnect();
  }
  client.loop();
   Serial.print("MOISTURE LEVEL : ");
   value= analogRead(sense_Pin);
   value= value/10;
   Serial.println(value);
   if(value<50)
   {
      digitalWrite(WET, HIGH);
   }
   else
   {
      digitalWrite(DRY,HIGH);
   }
   digitalWrite(WET,LOW);
   digitalWrite(DRY, LOW);
   Serial.print("Publish a message Moisture:");
  int num=value;
  char cm[16];
  itoa(num,cm,10);
  Serial.println(cm);
  sprintf(topic,"%s%s","/V1.6/devices/",device_label);
 sprintf(payload,"%s","");
 sprintf(payload,"{\"%s\":",Variable_label);
 sprintf(payload,"%s{\"value\":%s}}",payload,cm);
 client.publish(topic, payload);
  delay(3000);
}
