#include <WiFi.h>
#include <ETH.h>
#include <SPI.h>
#include <Ethernet.h>
#include <TinyGSM.h>
#include <DHT.h>

// Configuration
#define DHTPIN 16
#define DHTTYPE DHT22
#define TINY_GSM_MODEM_SIM800

// Global Variables
DHT dht(DHTPIN, DHTTYPE);
EthernetClient ethClient;
TinyGSM modem(SerialAT);
TinyGSMClient gsmClient(modem);
WiFiClient wifiClient;

char ssid[] = "your_ssid";
char pass[] = "your_password";
#define APN "your_apn"
#define USERNAME "your_username"
#define PASSWORD "your_password"

void setup() {
  Serial.begin(115200);

  // DHT22 Sensor Test
  Serial.println("Testing DHT22 Sensor...");
  dht.begin();
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("DHT22 Sensor test failed!");
  } else {
    Serial.println("DHT22 Sensor test passed!");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("°C, Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
  }

  // Ethernet Test
  Serial.println("Testing W5500 Ethernet Module...");
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  Ethernet.begin(mac);
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("W5500 Ethernet Module test failed!");
  } else if (Ethernet.linkStatus() == LinkON) {
    Serial.println("W5500 Ethernet Module test passed!");
    Serial.print("IP Address: ");
    Serial.println(Ethernet.localIP());
  } else {
    Serial.println("W5500 Ethernet Module test failed!");
  }

  // WiFi Test
  Serial.println("Testing WiFi...");
  WiFi.begin(ssid, pass);
  for (int i = 0; i < 20; ++i) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("WiFi test passed!");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
      break;
    }
    delay(500);
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi test failed!");
  }

  // GSM Module Test
  Serial.println("Testing GSM Module...");
  modem.begin(9600);
  if (modem.gprsConnect(APN, USERNAME, PASSWORD)) {
    Serial.println("GSM Module test passed!");
    Serial.print("IP Address: ");
    Serial.println(modem.localIP());
  } else {
    Serial.println("GSM Module test failed!");
  }
}

void loop() {
  // Leave loop empty for testing
}
