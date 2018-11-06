# serial-test
test repository for serial communication between VM and Arduino Mega 2560

test involves sending data over a serial port from a virtualbox ubuntu 64 bit with a usb extension pack, with USB set to 3.0 and Port connections set from COM1, Host Device into COM3, which was the port of the Arduino Mega on my Windows 8 Host.

Changelog:
November 6th, 2018:
  1. Made a proper Makefile
  2. Fixed declaration issues in main.c
  3. Fixed Arduino serial loop
  4. Added PSQL connections
  5. Added config and INI support derived from Phil's GS-Schduler
  6. Fixed timing issues between arduino and C application
  7. Fixed buffer memory allocation issues by clearing buffer after every read; see: sensorData buffer + memset functions
  8. Fixed arduino loops to prevent overwriting over each element
  9. added flush and memset as well on arduino end to make it better
  10. added config.ini.example from groundstation-tracking-scheduler as reference

Note: the client doesnt work if the serial monitor of the Arduino is listening to the Arduino as well.

To-do list:
  1. consider making two seperate buffers for azimuth and elevation, although clearing both with memset after done should be much better as azimuth and elevation ranges from 1 digit to 3 digits.
  2. add servo functions to interface with monitor
  3. Maybe add graphical interface(EXTRA)
  4. Test on a clean ubuntu
  5. fix config.ini issues

Compiling:

Make sure to add submodules in and download them as well.
Compile using make all in main folder.
Run SQL script in groundstation-tracking-scheduler
Compile and run the gss in groundstation-tracking-scheduler

Issues:
