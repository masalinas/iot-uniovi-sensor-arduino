/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SparkFun_RHT03.h>

// WIFI values suitable for your network.
const char* WIFI_SSID = "Thingtrack";
const char* WIFI_PASSWORD = "234803685";

// MQTT values suitable for your network.
const char* MQTT_HOST = "192.168.1.19";
const int MQTT_PORT = 1885;
const char* MQTT_USERNAME = "admin";
const char* MQTT_PASSWORD = "uniovi";
const char* MQTT_CLIENT_ID = "ARD01";
const char* MQTT_SENSOR_TOPIC = "sensors/temperature";
const char* MQTT_DEVICE_TOPIC = "devices/ARD01";
const char* MQTT_VALUE = "69";
const int MQTT_FRECUENCY = 5000;

// RHT03 Sensor values suitable
const int RHT03_DATA_PIN = D4; // RHT03 data pin
const int RHT03_HUMIDITY = 0;
const int RHT03_TEMPERATURE_C = 1;
const int RHT03_TEMPERATURE_F = 2;

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;
RHT03 rht;

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  
  Serial.begin(9600);
  
  setup_wifi();
  
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(callback);

  // Call rht.begin() to initialize the sensor and our data pin
  rht.begin(RHT03_DATA_PIN);
  Serial.println("RHT03 configured");
}

void setup_wifi() {
  delay(10);
  
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }
}

float* getSensorData() {  
  // Call rht.update() to get new humidity and temperature values from the sensor.
  int updateRet = rht.update();
  
  // If successful, the update() function will return 1.
  // If update fails, it will return a value <0
  float data[3];
  if (updateRet == 1) {
      Serial.println("RHT03 pin updated");
      
      // Now print the values:
      Serial.println("Humidity: " + String(rht.humidity(), 1) + " %");
      Serial.println("Temp (C): " + String(rht.tempC(), 1) + " deg C");
      Serial.println("Temp (F): " + String(rht.tempF(), 1) + " deg F");
      
      data[0] = rht.humidity();
      data[1] = rht.tempC();
      data[2] = rht.tempF();
  } 
  else {
    Serial.println("RH03 retry update");
    
    // If the update failed, try delaying for RHT_READ_INTERVAL_MS ms before trying again.
    delay(RHT_READ_INTERVAL_MS);
  }

  return data;
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // Attempt to connect
    if (client.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("connected");
      
      // Once connected, publish an announcement...
      client.publish(MQTT_SENSOR_TOPIC, MQTT_VALUE);
      
      // ... and resubscribe
      client.subscribe(MQTT_DEVICE_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      String mess = "try again in ";
      mess.concat(MQTT_FRECUENCY);
      mess.concat(" seconds"); 
      Serial.println(mess);
      
      // Wait MQTT_FRECUENCY seconds before retrying
      delay(MQTT_FRECUENCY);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;

    //long value = random(0, 45);
    //String meassure = String(value);

    // get sensor Data
    float *value = getSensorData();    

    // convert float to array char and send
    String meassureH = String(*(value + RHT03_HUMIDITY));
    String meassureTC = String(*(value + RHT03_TEMPERATURE_C));
    String meassureTF = String(*(value + RHT03_TEMPERATURE_F));

    char charMeassure[10];
    meassureTC.toCharArray(charMeassure, 10);
    client.publish(MQTT_SENSOR_TOPIC, charMeassure);
  }
}
