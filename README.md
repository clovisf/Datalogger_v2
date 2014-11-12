Datalogger_v2
=============

Weather station featuring an Arduino Uno+ SDcard shield+ some sensors >>

This is a Weather station project, based on an Arduino Uno and a SDcard shield (for saving data on a .CSV file).

It features some environment sensors: 
- LM35 for temperature
- DHT11 for temp+ humidity
- BMP085 for temp+ pressure+ altitude
- Li-Po 3.7V (with boost for 5V) for supplying the system (its voltage is logged as well).


All the data from the sensors above is read once every 5 seconds and stored on a .CSV file. The schamatics (as well a detailed blog post) is coming on my blog: 
http://embedded-clovis.blogspot.ca/

