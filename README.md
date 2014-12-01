# Intel Edison Hackday

This repository holds all the information for the [Intel Edison Demo Day](http://www.eventbrite.co.uk/e/intel-edison-demo-day-tickets-13546962329) run by [designswarm](http://designswarm.com/), Intel&reg; Corporation and [MCQN Ltd](http://www.mcqn.com) on November 28th.

It collects together all the code, etc. that we'll be using on the day and includes the steps that we'll be running through as we explore the Edison board.

## Repository contents

  * Arduino/Sketchbook: Example code.  Copy the contents into your Arduino sketchbook folder.
  * Arduino/IDE_patches: Updates to the Intel Edison Arduino IDE v1.0.4 to fix a couple of small bugs

## Order of the day

 * 9:00 Registration opens (Breakfast is served)
 * 10:00 Introduction to the day by Ana Bradley of designswarm
 * 10:15 What is the Intel® Edison
 * 10:25 Beginners tutorial for Intel® Edison by Adrian McEwen
   - Install the Edison Arduino environment
   - Get blink working on the Edison
   - Copy the day's examples, etc. into the right place
   - Hooking up sensors
   - Now control a servo
   - Using the LCD display 
 * 11:30 Break
 * 11:45 Continued Tutorial
   - Talking to the Internet
   - Simple web client
     - Setting your WiFi username/password
   - Being a web server
     - Background colour based on sensor input
   - Scraping web pages and talking to APIs
     - Getting the weather forecast from the Met Office
   - Pushing data to the cloud.
 * 13:00 Lunch
 * 14:00 Build a demo time
   - Anyone particularly interested in one of the sensors?
   - Is there a web service people would like to use?
 * 17:00 Team Demos (Drinks are served)
 * 18:00 End of the day and move to a nearby pub.


## Getting set up

 1. If you haven't already, download the [zip file of all the things you'll need](https://www.dropbox.com/sh/lwd98qpgqk3yso7/AAAMgd4cuz4PkUWHA8Aj_a14a?dl=0).  Once it's downloaded, unzip it (it doesn't matter where to, but somewhere you can find it again - for example, to your Desktop)
 1. While that downloads we'll unbox the goodies.
 1. Next we'll follow the steps to connect your Edison board and install the software.
    Follow the relevant link for your operating system:
   * [Windows](https://communities.intel.com/docs/DOC-23147)
   * [Mac](https://communities.intel.com/docs/DOC-23148)
   * [Linux](https://communities.intel.com/docs/DOC-23149)

    Your Edison boards have already been flashed with the latest image, so you don't need to worry about that.  On Windows, if you need something to open the .7z file containing the Arduino IDE you can find the 7-zip software to install in the 7-zip folder.

 1. In the Arduino IDE, open File -> Preferences.  Make a note of the sketchbook location (it's most likely to be an Arduino folder in your home directory)
 1. Copy the contents of the Sketchbook folder in the zip file you unpacked in step 1 into the sketchbook location on your machine.
 1. Restart the Arduino IDE so it spots the new files.  Once done you will have a new set of sketches available in the File -> Sketchbook menu, called EdisonExamples.
 1. Now you have Blink working, it's time to wire something up.  Open up your Grove Starter Kit Plus.  This has a number of basic input and output electronics, which can easily be hooked up to your Edison via the supplied ribbon cables (the black/red/white/yellow wires) and the Grove shield.
 1. The shield is the circuit board with a grid of white connectors hiding under the LCD display.  Take it out and carefully but firmly push it onto the black headers on your Edison breakout board.  It will only fit one way round, so check you've got everything lined up before forcing it.
 1. Now find the LED socket board, an LED, and a ribbon cable from your Grove kit.  Place the LED into the socket on the board (the longer leg of the LED goes into the hole marked +) and connect it with a ribbon cable to one of the sockets on the shield labelled with a D.  For example, D3.
    The sockets labelled with D are for digital inputs and outputs - things that are either on or off.  Those labelled with A are for analogue inputs - things that can vary.  We'll come to the analogue inputs shortly.
 1. To have the LED you just wired up do anything you need to tell your code to use it.  In the Blink example, find the line:
```
    int led = 13;
```
    Change it to match whichever socket you plugged your LED into.  In our example, that would be 3.
 1. Upload your modified code to your Edison and the LED should start blinking.

> <strong>Extra credit:</strong> Try modifying the values in the `delay` calls to vary how quickly the LED blinks.

## Hooking up sensors

 1. Now let's look at some inputs.  The Grove kit has a few different ones - the button and touch sensor are digital inputs, which can only be on or off.  We're going to look at the rotary angle, temperature and light sensors.  These turn what they measure into a voltage that varies between 0 and 5 volts.  The analogue input sockets (and associated analogRead code) let you read these values on your Edison.  Find the rotary angle sensor and a ribbon cable.
 1. Connect it up to the A0 socket
 1. Open the AnalogReadSerial example in the Arduino IDE.  File -> Examples -> 01.Basics -> AnalogReadSerial
 1. As you'll see in the `loop` function, this uses `analogRead` to read in the value of the sensor and then prints it out to the serial port.  The values will range from 0 to 1023, with 0 corresponding to 0V and 1023 to 5V.  Upload the sketch to your Edison
 1. Now open the serial monitor in the Arduino IDE - the little magnifying glass icon in the top right corner.  This will open a new window and start printing out a stream of numbers as the Edison takes readings and reports them.
 1. Twisting the control on the rotary angle sensor (or potentiometer, as they're known in electronics circles) and you'll see the value changing.

> <strong>Extra credit:</strong> Try plugging in the other sensors and see what range of numbers they report.

## Controlling a servo

 1. Here we'll bring together both input and output in a single example.  Find the servo in your Grove kit.  It's a small black box with a ribbon cable attached.
 1. Find the small bag of servo horns - white disc and rods.  Choose one and attach it to the servo.
 1. Connect the servo to the socket D3.
 1. Because the servo draws a bit more current than the LED we've used so far, we need to provide some extra power to the Edison board.  Plug in an external power supply to the black barrel jack in the corner of the board.
 1. Open the Grove_Servo sketch.  File -> Sketchbook -> Sketchbook_Starter_Kit_V2.0 -> Grove_servo
 1. Upload it to your Edison.
 1. Use the control on the rotary angle sensor to move the servo position.

> <strong>Extra credit:</strong> Using the range of values you could see when hooking up different sensors in the previous exercise, rework the `map` line in the sketch to give the full range of movement of the servo when using a different sensor.

## Using the LCD

 1. The LCD will let us display two lines of 16 characters.  It communicates with the Edison board via the [I2C protocol](http://en.wikipedia.org/wiki/I%C2%B2C).  Connect the LCD board up to one of the I2C sockets with a ribbon cable.
 1. Communicating with the LCD has been abstracted into the rgb_lcd library, so you don't need to worry about using the I2C protocol directly (although that would just need you to read through the datasheet for the LCD to work out what commands to send it).  Open the TimePosixLCD sketch.  File -> EdisonExamples -> TimePosixLCD.
 1. The sketch uses the `system`, `fopen`, `fgets` and `fclose` to access the time from the Linux side of the Edison, and then displays it on the LCD.  Upload the sketch to your Edison and you'll have a rather over-engineered electronic clock.

> <strong>Extra credit:</strong> Modify the sketch so that the screen backlight turns green when it's lunchtime or hometime.

## Talking to the Internet

Now we'll look at ways to get your Edison board to interact with the Internet.

## Downloading a web page

 1. Open the WiFiWebClient sketch.  File -> Examples -> WiFi -> WiFiWebClient.
 1. We need to tell it the details of the WiFi network we want to connect to.  Find the lines
```
char ssid[] = "yourNetwork";      //  your network SSID (name) 
char pass[] = "secretPassword";   // your network password
```
    And change them to match the network and password for the venue's WiFi.
 1. Upload the sketch to your Edison, then open the serial monitor.
 1. You'll see it print out its progress as it connects to the WiFi, and then connecting to the server and displaying the response that it receives.

> <strong>Extra credit:</strong> Read through the response that you get from www.google.com and modify the sketch so that it makes the request to the alternate URL that Google tries to redirect you to.

## Being a web server

 1. Open the LightSensitiveWebServer sketch.  File -> Sketchbook -> EdisonExamples -> LightSensitiveWebServer.
 1. Connect the light sensor to socket A0.
 1. Connect the temperature sensor to socket A1.
 1. Find the `ssid` and `pass` lines in the sketch, and update them to the relevant WiFi network.
 1. Upload the sketch to your Edison, then open the serial monitor.
 1. Once connected to the WiFi, your Edison will print out the URL you can use to connect to it.  Copy that into the address bar on your browser and it show a page with details of the light level and temperature.
 1. Vary the amount of light hitting the sensor and you will see both the light level value and the background colour of the web page change.

> <strong>Note:</strong> The Edison board already has a web server running on it, which is why your web server URL ends in ":8080".  That tells your browser to look for the web server on port 8080 rather than the default port 80.  If you try removing the ":8080" you'll see the Edison's default web server page.

## Scraping web pages and talking to APIs

 1. Next we'll use the Met Office API to download the weather forecast and extract some details from it.  Open the WeatherWatch sketch: File -> Sketchbook -> EdisonExamples -> WeatherWatch.
 1. Find the `ssid` and `pass` lines in the sketch, and update them to the relevant WiFi network.
 1. Connect the servo to socket D3.  The sketch will download a web page from the Met Office's server, in a similar way to the simple web client earlier.  However, rather than print out the downloaded page it uses `find` call to search through the response and `parseInt` to convert the text into values that can be used to show the conditions forecast by using the servo as a pointer.
 1. Upload the sketch to your Edison, then open the serial monitor.
 1. Once connected to the WiFi, your Edison will output some of the forecast details as it parses the results, and then will move the servo to point to one of six different weather types.

> <strong>Extra credit:</strong> Change the code to show the forecast temperature on the servo.  Modify the URL requested to get the forecast for your home town (rather than Liverpool, the current default).

## Publishing sensor data to the Internet

 1. Intel have a service called IoTKit which lets you store and graph simple sensor data.  Go to the [IoTKit website](https://dashboard.us.enableiot.com/) and create an account so that you can use it.
 1. Next we need to run a few commands on your Edison through its console to set up the IoTKit agent.  Connect up the second micro-USB cable to your Edison and run `PuTTY` or `screen` like you did in step 4 of the initial installation instructions to connect to your Edison's console.
 1. Run `iotkit-admin test` to check that the Edison connectivity is okay.  You shouldn't see any errors output.
 1. Retrieve the Device ID for your Edison by running `iotkit-admin device-id`.
 1. On the IoTKit website, go to the device view and click `Add a new device`.  Copy the Device ID into both the ID and Gateway fields, and give your device a name.  Then click `Save` to create the device.
 1. Click on your account name (next to "IoT Analytics" in the bar across the top) and click the eye icon to show your Activation Code.
 1. Back in the Edison console, type `iotkit-admin activate <activation code>`, replacing `<activation code>` with the code from your IoTKit account.
 1. Now you're ready to start pushing data to IoTKit, so start the IoTKit agent by typing `systemctl start iotkit-agent`.
 1. The IoTKit agent is waiting for JSON strings to be delivered over either UDP or TCP containing sensor values.  The SimpleIoTKitTemperatureSensor sketch will send some suitable readings to the Agent.  Find it in File -> Sketchbook -> EdisonExamples -> SimpleIoTKitTemperatureSensor.
 1. Find the `ssid` and `pass` lines in the sketch, and update them to the relevant WiFi network.
 1. Connect the temperature sensor to socket A0.
 1. Upload the sketch to your Edison.
 1. On your IoTKit Dashboard in your web browser (click the "IoT Analytics" link to get back to it if need be) you should see the "Last hour observations" counter go up as the readings are pushed up to the cloud.
 1. If you click the "burger menu" in the top left corner of the dashboard, and choose "Charts" you will then be able to select your device and the temperature component and graph the data being recorded.

# Links

 * [Arduino language reference](http://arduino.cc/en/Reference/HomePage)
 * [The Grove Starter Kit Plus](http://www.seeedstudio.com/wiki/Grove_-_Starter_Kit_V2.0) website, and [the official location for the example code](https://github.com/Seeed-Studio/Sketchbook_Starter_Kit_V2.0)
 * [Edison Arduino expansion board hardware guide](https://communities.intel.com/docs/DOC-23161) - includes details of the jumper settings, pin-out diagram, etc. for the expansion board
 * If you do need to flash your Edison board with a new image, follow the instructions for your operating system below:
   - [Windows](https://communities.intel.com/docs/DOC-23192)
   - [Mac](https://communities.intel.com/docs/DOC-23193)
   - [Linux](https://communities.intel.com/docs/DOC-23200)
 * [Intel IoTKit sample code](https://github.com/enableiot/iotkit-samples)
 * [Intel IoT Developer Zone](https://software.intel.com/en-us/iot)
 * [Met Office API reference](http://www.metoffice.gov.uk/datapoint/support/api-reference)
 * [Edison community forum](https://communities.intel.com/community/makers/edison)

And if you want to skip the Arduino IDE and play around with Node.js or Python and the Edison then these links might help

 * [mraa](https://github.com/intel-iot-devkit/mraa) is the low-level API to access the GPIO pins, etc. from languages like Node.js
 * [upm](https://github.com/intel-iot-devkit/upm) sits on top of mraa and provides a set of libraries for common sensors, IO chips, etc.
 * [Getting started with Intel DevKit and XDK](https://software.intel.com/en-us/html5/documentation/getting-started-with-intel-xdk-iot-edition) - if you want to try out the Intel XDK development environment


