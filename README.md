SEEEDUINO temperature and humidity logger
================================================


Seeeduino Stalker code for logging temperature and humidity from a DHT-22 sensor on an SD-card from:
<http://www.seeedstudio.com/depot/Seeeduino-Stalker-Waterproof-Solar-Kit-p-911.html>

The scripts include some basic checks to repeat logging given an erroneous reading.

Load this script on the device to start logging, setting the sampling delay appropriately:
stalker_temperature_log_to_sdcard.ino

Load this script when wanting to print off values:
stalker_print_log_file_to_serial.ino