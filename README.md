weatherstation
-------------

The weatherstation provides a small web server to provide sensor data
web page using 'justagage' javascript graphical gauges. The web server
provides two HTTP URL Endpoints: / for the main html page and /sensors
simple REST API for reading the latest sensor values. The server has
two threads: One thread to read the latest sensors and second thread
serving the HTTP requests.

The server is based on
https://www.fontenay-ronan.fr/web-interface-for-raspberrys-sensors-gpio/
examples.

This project was done on Aalto University Electronics lab course in
spring 2019. For more details see Aalto wiki page at:
https://wiki.aalto.fi/pages/viewpage.action?pageId=149882850

Installation
------------
Step 1: Install python flask package: sudo pip install flask
Step 2: weatherserver.zip package
Step 3: Connect arduino to the Linux
Step 4: Check with dmesg the usb tty port: dmesg | grep tty|tail
Step 5: Edit the server.py to use tty file, e.g. INPUTDEVICE='/dev/ttyUSB0'
Step 6: Start server: ./server.sh
Step 7: Connect to the server with browser: firefox http://localhost:8080


Files
-----
readme.md - this file
webserver/sensors.input - example input file for testing
webserver/server.py - contains the http server with poller thread for sensor feed
webserver/templates/main.html - the main web page template
webserver/static/justagage.js - justgage javascript code for html
webserver/static/raphael-2.1.4.min.js - raphael javascript needed by justgage for html
webserver/static/jquery-1.7.1.min.js - jquery javascripted needed by data feed for html
arduino/server.c - the arduino weather station server
arduino/receiver.c - the arduno weather station receiver
