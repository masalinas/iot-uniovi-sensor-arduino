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

* Start Arduino and open Include Libraries -> Add Library from ZIP File menu.
* Select the previous zip library file: pubsubclient-2.7.zip