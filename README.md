# Intel Edison Hackday

This repository holds all the information for the [Intel Edison Demo Day](http://www.eventbrite.co.uk/e/intel-edison-demo-day-tickets-13546962329) run by [designswarm](http://designswarm.com/), Intel&reg; Corporation and [MCQN Ltd](http://www.mcqn.com) on November 28th.

It collects together all the code, etc. that we'll be using on the day and includes the steps that we'll be running through as we explore the Edison board.

FIXME Explain the different folders, etc.

## Respository contents

  * Arduino: Example code
  * Downloads: Arduino programming environment for Edison, relevant drivers, etc.

## Order of the day

 * 9:00 Registration opens (Breakfast is served)
 * 10:00 Introduction to the day by Ana Bradley of designswarm
 * 10:15 What is the Intel® Edison
 * 10:25 Beginners tutorial for Intel® Edison by Adrian McEwen
   - Install the Edison Arduino environmnet
   - Get blink working on the Edison
   - Copy the day's examples, etc. into the right place
   - Hooking up sensors
   - Now control a servo
   - Using the LCD display (Michael's time example?)
 
 * 11:30 Break
 * 11:45 Continued Tutorial
   - Talking to the Internet
   - Simple web client
     - Setting your WiFi username/password
   - Being a web server
     - Background colour based on sensor input
   - Scraping web pages and talking to APIs
     - Getting the weather forecast from the Met Office
   - Pushing data to the cloud?  Which cloud?
 * 13:00 Lunch
 * 14:00 Build a demo time
   - Anyone particularly interested in one of the sensors?
   - Is there a web service people would like to use?
   - 
 * 17:00 Team Demos (Drinks are served)
 * 18:00 End of the day and move to a nearby pub.


## Getting set up

 1. If you haven't already, download the [zip file of all the things you'll need]() FIXME  Once it's downloaded, unzip it (it doesn't matter where to, but somewhere you can find it again - for example, to your Desktop)
 1. While that downloads we'll unbox the goodies.
 1. Next we'll follow the steps to connect your Edison board and install the software.
    Follow the relevant link for your operating system:
   * [Windows](https://communities.intel.com/docs/DOC-23147)
   * [Mac](https://communities.intel.com/docs/DOC-23148)
   * [Linux](https://communities.intel.com/docs/DOC-23149)

FIXME John, do we need any drivers for Mac?
FIXME John, does the Mac come with ability to open zip files by default?
FIXME Do we need an install for something on Windows to let us open .7z files?
    Your Edison boards have already been flashed with the latest image, so you don't need to worry about that.

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

# Links

FIXME Link to Grove kit datasheets?

 * [Arduino language reference](http://arduino.cc/en/Reference/HomePage)
 * [The Grove Starter Kit Plus](http://www.seeedstudio.com/wiki/Grove_-_Starter_Kit_V2.0) website, and [the official location for the example code](https://github.com/Seeed-Studio/Sketchbook_Starter_Kit_V2.0)
 * [Edison Arduino expansion board hardware guide](https://communities.intel.com/docs/DOC-23161) - includes details of the jumper settings, pin-out diagram, etc. for the expansion board
 * If you do need to flash your Edison board with a new image, follow the instructions for your operating system below:
   - [Windows](https://communities.intel.com/docs/DOC-23192)
   - [Mac](https://communities.intel.com/docs/DOC-23193)
   - [Linux](https://communities.intel.com/docs/DOC-23200)
 * [Intel IoT Developer Zone](https://software.intel.com/en-us/iot)


