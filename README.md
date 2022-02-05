# Arduino boards: install esp8266 arduino board:

* Start Arduino and open Preferences from **File -> Preferences** menu option
* Enter the ESP8266 Repository link [http://arduino.esp8266.com/stable/package_esp8266com_index.json](http://arduino.esp8266.com/stable/package_esp8266com_index.json) into **Additional Board Manager URLs** field. You can add multiple URLs, separating them with commas:

![Board Repositories](captures/board_repositories.png "Board Repositories")

* Open Boards Manager from **Tools > Board** menu option and select esp8266 platform:
* Select the version you need from a drop-down box of set default:
* Click install button.

![ESP8266 Board](captures/esp8266_board.png "ESP8266 Board")

* Select the board used for your sketch, in our case **NodeMCU 1.0 (ESP-12E Module)**. And configure like this:

![Board configuration](captures/board_configuration.png "Board configuration")

# Arduino dependencies: 

Add arduino dependencies before compile your sketch

## Arduino MQTT client from knolleary/pubsubclient respository

* Download pubsubclient-2.7.zip from [pub-sub-client](https://www.arduinolibraries.info/libraries/pub-sub-client)
The sources from [github pub-sub-client](https://github.com/knolleary/pubsubclient).
* Select Arduino menu option: **Sketch -> Include Libraries -> Add Zip Library** and select previous zip library

## Arduino RHT03 driver from sparkfun/SparkFun_RHT03_Arduino_Library respository

* Download SparkFun_RHT03_Arduino_Library-V_1.0.0.zip from [SparkFun_RHT03_Arduino Repository](https://github.com/sparkfun/SparkFun_RHT03_Arduino_Library) 
* Select Arduino menu option: **Sketch Include Libraries -> Add Library** and select zip file

## Arduino JSON dependency from Arduino libraries management list

* From Arduino Library Manager **Sketch -> Include Libraries -> Manage Libraries..** install version 5.13.4 the last 5 version

![ArduinoJSON Dependency](captures/ArduinoJSON.png "ArduinoJSON Dependency")

## Arduino DS1302 dependency from msparks/arduino-ds1302 respository

* Download arduino-ds1302.zip from [arduino-ds1302](https://github.com/msparks/arduino-ds1302)
The sources from [arduino-ds1302](https://github.com/msparks/arduino-ds1302).
* Select Arduino menu option: **Sketch -> Include Libraries -> Add Zip Library** and select previous zip library
