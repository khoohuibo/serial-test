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

Note: the client doesnt work if the serial monitor of the host machine is listening to the Arduino as well.

Set-up Procedure:
1. Install Ubuntu 18.04, download from Ubuntu https://www.ubuntu.com/download/desktop
2. Minimal Installation, rest are default options or doesnt matter
3. Git clone this repo into the directory you want like so
`git clone https://github.com/khoohuibo/serial-test`

You will require git to download this.

`sudo apt-get install git`

4. Update the git submodules using `git submodule update --init`. In the event of an error, force the update using `git submodule update --init --force --remote`

5. Configuration is required of both the init_database.sql and example_spacecraft.sql, found within the serial-test/groundstation-tracking-scheduler/sql. Change the database and user as per desired.

6. Change the config.ini.example file to an ini file with the proper fields. A spacetrack account may be required. The host is usually localhost. Password for the user can be configured in the PSQL interface(later)

7. Install PSQL using `sudo apt-get install postgresql`. When installed, run `sudo -u postgres psql` to initialize the psql terminal interface

8. Run the sql scripts using `\i <your/file/path.sql>` and it should create the role with the name you want. You may alter the script to automatically assign a password to the role, but otherwise a simple `ALTER ROLE <username> WITH PASSWORD '<password>'` will suffice. The quotation marks are important.

9. After running both sql scripts, run `make` in the groundstation-tracking-scheduler folder. Dependencies required are similar to that found in https://github.com/philcrump/groundstation-tracking-scheduler. The dependencies are: libpq-dev and libcurl4-openssl-dev

10. Run the generated client, in the default case it is `./gss` and watch the magic.

11. To utilize the generated observation data, go to the upper `serial-test` folder and run `make` and its associated client(default: `./serial_test_client`). Copy the config.ini over from the previous folder and configure the main.c for anything you need.

12. The arduino must have the sketch uploaded into it, which is found in k3ng_rotator_controller

Compiling:

Make sure to add submodules in and download them as well.
Compile using make all in main folder.
Run SQL script in groundstation-tracking-scheduler
Compile and run the gss in groundstation-tracking-scheduler

Issues:
1. THIS BUILD IS INCOMPLETED 07/02/2019 there is a LOT to clean up
