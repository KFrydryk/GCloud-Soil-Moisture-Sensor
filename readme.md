# Google Cloud Soil Moisture Sensor

This is a home project and an exercise on using Cloud solutions in IoT. 

The project uses a generic soil moisture sensor and a photoresistor with ESP8266 and complementary devices (resistors, buttons, voltage regulator and some pin headers).

# How does it work?

The data is from the sensor is gathered as analog value and mapped to 0-100% range. ESP8266s analog in is rated for 0-1V only so the reading has to be transformed via a simple voltage divider.
Photoresistor value is mapped to binary values of 0 (dark) and 100 (light) as ESP8266 has only 1 analog input. To make it more consistent, the value is gathered form a variable voltage divider and reading routine sets the input pin value high before the actual reading process. It guarantees same voltage threshold for low/high exposition transitions despite the input hysteresis.
Data is filtered by a simple moving average filter and translated into JSON format, which can be sent using MQTT protocol to Google IoT service.
IoT routine sets off a google Cloud Function service to parse the JSON data and insert it into BigQuery database.
For graphical output Google Datastudio is used. Data is obtained using automated SQL query every hour or on users demand.

# PCB
PCB was designed in Autocad EAGLE, all of the necessary files should be updated in [Eagle_files folder](https://github.com/KFrydryk/GCloud-Soil-Moisture-Sensor/tree/master/Eagle_files)

# Schematic

![schematic](https://github.com/KFrydryk/GCloud-Soil-Moisture-Sensor/blob/master/Eagle_files/schematic.png?raw=true "Schematic")

# Board

![Board](https://github.com/KFrydryk/GCloud-Soil-Moisture-Sensor/blob/master/Eagle_files/board.png?raw=true "Board")

# Photo

![Pic](picture.jpg?raw=true "Photo")
