#include <PubSubClient.h>

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
#include <ArduinoJson.h>
#include <DS1302.h>

// WIFI values suitable for your network.
//const char* WIFI_SSID = "Thingtrack";
//const char* WIFI_PASSWORD = "234803685";
const char* WIFI_SSID = "maiind";
const char* WIFI_PASSWORD = "tullavealfuturo";

// MQTT values suitable for your network.
const char* MQTT_HOST = "192.168.1.38";
const int MQTT_PORT = 1883;
const char* MQTT_USERNAME = "admin";
const char* MQTT_PASSWORD = "uniovi";
const char* MQTT_CLIENT_ID = "ARD01";
const char* MQTT_TOPIC_SENSOR_TP01 = "uniovi/poc/temperature/TP01";
const char* MQTT_TOPIC_SENSOR_RH01 = "uniovi/poc/humidity/RH01";
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

const int kCePin   = D1;  // Chip Enable
const int kIoPin   = D2;  // Input/Output
const int kSclkPin = D3;  // Serial Clock

// Create a DS1302 object.
DS1302 rtc(kCePin, kIoPin, kSclkPin); 

void setup() {
  // Initialize the BUILTIN_LED pin as an output
  pinMode(BUILTIN_LED, OUTPUT);
  
  Serial.begin(9600);
  
  setup_wifi();
  
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(callback);

  setup_time();
                 
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

void setup_time() {
// Initialize a new chip by turning off write protection and clearing the
  // clock halt flag. These methods needn't always be called. See the DS1302
  // datasheet for details.
  rtc.writeProtect(false);
  rtc.halt(false);

  // Make a new time object to set the date and time.
  // Saturday, Februry 23, 2019 at 10:45:00.
  //Time t(2019, 2, 23, 10, 54, 00, Time::kSaturday);

  // Set the time and date on the chip.
  //rtc.time(t);
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
    //if (client.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
    if (client.connect(MQTT_CLIENT_ID)) {
      Serial.println("connected");
      
      // Once connected, publish an announcement...
      //client.publish(MQTT_SENSOR_TOPIC, MQTT_VALUE);
      
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
        
    // Get the current time and date from the chip.
    Time t = rtc.time();

    // Format the time and date and insert into the temporary buffer.
    char datetime[50];
    snprintf(datetime, sizeof(datetime), "%04d-%02d-%02dT%02d:%02d:%02d", t.yr, t.mon, t.date, t.hr, t.min, t.sec);
  
    // Print the formatted string to serial so we can see the time.
    Serial.println(datetime);
  
    // get sensor Data
    float *value = getSensorData();    

    // convert float to array char and send
    String meassureRH = String(*(value + RHT03_HUMIDITY));
    String meassureTC = String(*(value + RHT03_TEMPERATURE_C));
    String meassureTF = String(*(value + RHT03_TEMPERATURE_F));

    char charMeassure[10];

    StaticJsonBuffer<300> JSONbuffer;
    char sensorJSONmessageBuffer[100];
    
    // publish humidity JSON value
    meassureRH.toCharArray(charMeassure, 10);

    JsonObject& humidityJSON = JSONbuffer.createObject();
    
    humidityJSON["device"] = "RH01";
    humidityJSON["value"] = charMeassure;
    humidityJSON["date"] = datetime;

    humidityJSON.printTo(sensorJSONmessageBuffer, sizeof(sensorJSONmessageBuffer));
    //String input = "{\"device\":\"RH01\",\"time\":1351824120,\"value\":" + charMeassure + "]}";
      
    client.publish(MQTT_TOPIC_SENSOR_RH01, sensorJSONmessageBuffer);
    //client.publish(MQTT_TOPIC_SENSOR_RH01, charMeassure);
        
    // publish temperature JSON value
    meassureTC.toCharArray(charMeassure, 10);

    JsonObject& temperatureJSON = JSONbuffer.createObject();
    
    temperatureJSON["device"] = "TP01";
    temperatureJSON["value"] = charMeassure;
    temperatureJSON["date"] = datetime;

    temperatureJSON.printTo(sensorJSONmessageBuffer, sizeof(sensorJSONmessageBuffer));
    //String input = "{\"device\":\"TP01\",\"time\":1351824120,\"value\":" + charMeassure + "]}";
    
    client.publish(MQTT_TOPIC_SENSOR_TP01, sensorJSONmessageBuffer);
    //client.publish(MQTT_TOPIC_SENSOR_TP01, charMeassure);   
  }
}
