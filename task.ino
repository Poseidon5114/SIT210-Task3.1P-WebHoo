#include <DHT.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>

#define DHT_SENSOR_PIN 7
#define DHT_SENSOR_TYPE DHT11

char wifi_ssid[] = "Hitesh"; 
char wifi_password[] = "HiteshVandna"; 
unsigned long channel_ID = 2315378; 
const char API_key = "BL2T7G5OGOV8TC3R"; 
DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
WiFiSSLClient wifi_client;

void setup() {
  Serial.begin(9600);
  dht_sensor.begin();
  connectToWiFi();
  ThingSpeak.begin(wifi_client);
}

void loop() {
  delay(2500);
  float temperature = dht_sensor.readtemperatureerature();
  float humidity = dht_sensor.readhumidityidity();

  if (!isnan(temperature) && !isnan(humidity)) {
    sendDataToThingSpeak(temperature, humidity);
  } else {
    Serial.println("Failed to read data from DHT sensor!");
  }

  delay(40000);
}

void connectToWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(wifi_ssid, wifi_password);
    delay(1000);
  }
  Serial.println("Connected to WiFi");
}

void sendDataToThingSpeak(float temperature, float humidityidity) {
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidityidity);

  int response = ThingSpeak.writeFields(channel_ID, API_key);

  if (response == 200) {
    Serial.println("Data sent to ThingSpeak");
  } else {
    Serial.print("Error sending data to ThingSpeak. HTTP Response Code: ");
    Serial.println(response);
  }
}
