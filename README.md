# serial-test
test repository for serial communication between VM and Arduino Mega 2560

test involves sending data over a serial port from a virtualbox ubuntu 64 bit with a usb extension pack, with USB set to 3.0 and Port connections set from COM1, Host Device into COM3, which was the port of the Arduino Mega on my board.

Changelog:
November 6th, 2018:
  1. Made a proper Makefile
  2. Fixed declaration issues in main.c
  3. Fixed Arduino serial loop
  4. Added PSQL connections
  5. Added config and INI support derived from Phil's GS-Schduler

Note: the client doesnt work if the serial monitor of the Arduino is listening to the Arduino as well.

To-do list:
  1. fix psql connection; currently the psql database generated is under phil's name, have to fix it to a generic name to be usable on new machines
