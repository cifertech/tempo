#include <SPI.h>
#include <Ethernet.h>
#include <WiFi.h> 
#include <DHT.h>

byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
EthernetClient ethClient;
WiFiClient wifiClient; 
String data;

#define DHTPIN 16          
#define DHTTYPE DHT22     

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "CiferTech";
const char* password = "1234567891";

void setup() {
  Serial.begin(115200);

  Ethernet.init(25);
  Ethernet.begin(mac);
  
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
  }

    WiFi.begin(ssid, password);
    
    if (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  Serial.print("IP: ");
  Serial.println(Ethernet.localIP());

  dht.begin();
}

void loop() {
  float temperature = dht.readTemperature();  
  float humidity = dht.readHumidity();        

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Client* client;

  if (Ethernet.linkStatus() == LinkON) {
    client = &wifiClient;
  } else {
    client = &ethClient;
  }

  if (client->connect("api.thingspeak.com", 80)) {
    Serial.print("Connected \n");
    Serial.print("Making a HTTP request \n");
    String url = "/update?api_key=JQL03RWCB5MYANKW&field1=" + String(temperature) + "&field2=" + String(humidity);
    client->print("GET ");
    client->print(url);
    client->println(" HTTP/1.1");
    client->println("Host: api.thingspeak.com");
    client->println("Connection: close");
    client->println();
    Serial.print("Data sent: ");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  } else {
    Serial.print("Not Connected \n");
  }

  if (client->connected()) {
    client->stop();
  }

  delay(500);
}
