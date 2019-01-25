# Arduino boards: install esp8266 arduino board:

* Start Arduino and open Preferences window
* Enter http://arduino.esp8266.com/stable/package_esp8266com_index.json into Additional Board. Manager URLs field. You can add multiple URLs, separating them with commas.
* Open Boards Manager from Tools > Board menu and find esp8266 platform.
* Select the version you need from a drop-down box.
* Click install button.
* Don’t forget to select your ESP8266 board from Tools > Board menu after installation.

Example:
  - Board: NodeMCU 1.0 (ESP-12E Module)  

# Arduino dependencies: install knolleary/pubsubclient MQTT Arduino client:

* Download pubsubclient-2.7.zip from https://github.com/knolleary/pubsubclient.
* Start Arduino and open Include Libraries -> Add Library from ZIP File menu and select previous zip library


# Arduino dependencies: install sparkfun/SparkFun_RHT03_Arduino_Library RHT03 Arduino driver

* Download SparkFun_RHT03_Arduino_Library-V_1.0.0.zip from https://github.com/sparkfun/SparkFun_RHT03_Arduino_Library
* Start Arduino and open Include Libraries -> Add Library from ZIP File menu and select previous zip library