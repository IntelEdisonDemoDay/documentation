//#########################################################################
//########################    WeatherWatch    #############################
//#########################################################################
//
// (c) Copyright 2013-2014 MCQN Ltd.
// Released under Apache License, version 2.0
//
// WeatherWatch
//
// A sketch to get the latest weather forecast from the Met Office
// and display some of the key information from it

//################# LIBRARIES ################
#include <SPI.h>
#include <WiFi.h>
#include <HttpClient.h>
#include <Servo.h>
#include "api_key.h"

//################# WIFI DETAILS #################
char ssid[] = "DoESDinky"; //  your network SSID (name) 
char pass[] = "decafbad00";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

//################ PINS USED ######################
const int kServoPin = 3;

//################ VARIABLES ################

//------ NETWORK VARIABLES---------
// Name of the server we want to connect to
const char kHostname[] = "datapoint.metoffice.gov.uk";
// Path to download (this is the bit after the hostname in the URL
// that you want to download
//--MAKE CHANGE HERE--MAKE CHANGE HERE--MAKE CHANGE HERE--MAKE CHANGE HERE--MAKE CHANGE HERE--    
// At present it will get the weather forecast for central Liverpool.  You can comment out the next
// line and uncomment one of the others to get the forecast for Bodmin in Cornwall or Prabost on
// the Isle of Skye.  Or you could find out the right station ID for the forecast nearest to you!
const char kPath[] = "/public/data/val/wxfcs/all/json/310012?res=3hourly&key=" APIKEY;  // Central Liverpool
//const char kPath[] = "/public/data/val/wxfcs/all/json/322041?res=3hourly&key=" APIKEY;  // Bodmin, Cornwall
//const char kPath[] = "/public/data/val/wxfcs/all/json/371633?res=3hourly&key=" APIKEY;  // Prabost, Skye
/* To find the right weather station ID for your nearest weather station, go to
 * http://www.metoffice.gov.uk/public/weather/forecast/ and find the right forecast there
 * Then find the "view source" option in your web browser, and search for "locationId"
 * You should find something like this (this example is for Liverpool):
<script type="text/javascript">
var staticPage = { locationId: 310012};
</script>
 * The number after the locationId part is the station ID for your weather station
 */
//const char kPath[] = "/public/data/val/wxfcs/all/json/INSERT_WEATHER_STATION_ID_HERE?res=3hourly&key=" APIKEY;

// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30*1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;

//------ OUTPUT VARIABLES---------
Servo myServo;

//------ WEATHER VARIABLES---------
int windSpeed = 0;
int temperature = 0;
int chanceOfRain = 0;
int weatherType = 0;


/*
 * setup() - this function runs once when you turn your Arduino on
 */
void setup()
{
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  Serial.println("Hello!");
  Serial.println("Getting ready to watch the weather.");

  setupCircuit();
  testCircuit();
  setupNetwork();
}


/*
* loop() - this function will start after setup finishes and then repeat
*/
void loop()
{
  // Check the forecast
  downloadForecast();
  
  // Update the weather display using the information we got from the forecast
  //temperatureToServo();
  // At the moment it will display the % chance of rain on the LEDs.  If you'd
  // rather it displays what sort of weather (sunny/overcast/rainy/snowy/etc.)
  // then comment out the next line and uncomment the one after
  weatherTypeToServo();
  Serial.println();

  // And wait for a bit before checking the forecast again
  delay(10000);
}
  

/*
 * setupCircuit() - this function sets all the pins to their correct mode
 */  
void setupCircuit()
{
  // Set up the servo
  myServo.attach(kServoPin);
}


/*
 * testCircuit() - this function tests your circuit, turning the LEDs on 
 * in turn and outputting which one it should be to the serial monitor
 * and check the range of the servo
 */
void testCircuit()
{
  // Test the servo
  Serial.println("Testing servo over full range");
  for (int i =0; i <= 140; i++)
  {
    Serial.print(".");
    myServo.write(i);
    delay(20);
  }
  myServo.write(0);
  Serial.println();
}


/*
 * setupNetwork() - Sets up your connection to the network
 */
void setupNetwork()
{ 
  Serial.print("Configuring Network...");
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 

  String fv = WiFi.firmwareVersion();
  if( fv != "1.1.0" )
    Serial.println("Please upgrade the firmware");
  
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);
  
    // wait 10 seconds for connection:
    delay(10000);
  } 
  Serial.println("Connected to wifi");
}


/*
 * downloadForecast() - Connect to the Met Office website, and
 * retrieve the latest 3-hour forecast
 */
int downloadForecast()
{
  int err =0;
  
  WiFiClient c;
  HttpClient http(c);
  
  err = http.get(kHostname, kPath);
  if (err == 0)
  {
    Serial.println("startedRequest ok");

    err = http.responseStatusCode();
    if (err >= 200 && err < 300)
    {
      // It's a 2xx response code, which is a success
      err = http.skipResponseHeaders();
      
      if (err >= 0)
      {
        // Now we're into the data being returned.  We need to search
        // through it for the bits of the forecast we're interested in
        if (http.find("Day"))
        {
          // We've skipped past all the stuff at the start we don't care about

          // Now look for the first part we want to check - the chance of rain.
          // That's marked as "Pp" in the forecast information
          if (http.find("Pp"))
          {
            // We've found the first "% chance of rain" reading
            // Read in the value for it
            chanceOfRain = http.parseInt();
            Serial.print("Chance of rain: ");
            Serial.println(chanceOfRain);
            // Now look for the wind speed, that's marked as "S"
            if (http.find("S"))
            {
              // Found it
              // Read in the value for it
              windSpeed = http.parseInt();
              Serial.print("    Wind speed: ");
              Serial.println(windSpeed);
              // And now the temperature, which is marked as "T"
              if (http.find("T"))
              {
                // Got it
                // Read in the temperature, which will be the next Integer (whole number)
                temperature = http.parseInt();
                Serial.print("   Temperature: ");
                Serial.println(temperature);
                // Finally look for the weather type
                if (http.find("W"))
                {
                  // And we have it
                  // Again read in the value
                  weatherType = http.parseInt();
                  Serial.print("  Weather Type: ");
                  Serial.println(weatherType);
                }
              }
            }
          }
        }
      }
      else
      {
        Serial.print("Failed to skip response headers: ");
        Serial.println(err);
      }
    }
    else {
      Serial.print("Problem reading page.  Got status code: ");
      Serial.println(err);

      // Usually you'd check that the response code is 200 or a
      // similar "success" code (200-299) before carrying on,
      // but we'll print out whatever response we get

      err = http.skipResponseHeaders();
      if (err >= 0)
      {
        int bodyLen = http.contentLength();
        Serial.print("Content length is: ");
        Serial.println(bodyLen);
        Serial.println();
        Serial.println("Body returned follows:");
      
        // Now we've got to the body, so we can print it out
        unsigned long timeoutStart = millis();
        char c;
        // Whilst we haven't timed out & haven't reached the end of the body
        while ( (http.connected() || http.available()) &&
               ((millis() - timeoutStart) < kNetworkTimeout) )
        {
            if (http.available())
            {
                c = http.read();
                // Print out this character
                Serial.print(c);
               
                bodyLen--;
                // We read something, reset the timeout counter
                timeoutStart = millis();
            }
            else
            {
                // We haven't got any data, so let's pause to allow some to
                // arrive
                delay(kNetworkDelay);
            }
        }
      }
      else
      {
        Serial.print("Failed to skip response headers: ");
        Serial.println(err);
      }
    }
  }
  else
  {
    Serial.print("Connect failed: ");
    Serial.println(err);
  }
  http.stop();
  
  // Let the code that called us know whether things went okay or not
  return err;
}


/*
 * temperatureToServo() - takes the temperature value retrieved from the
 * Met Office, and works out where to position the servo to display it
 */
void temperatureToServo()
{
  // Servo runs between 0 and 140 degrees
  // temperature ranges from -10 to 35
  // Map the temperature onto the right servo position
  int pos = map(temperature, -10, 35, 0, 140);
  // Now we know what position the servo should be in, move it there
  myServo.write(pos);
}


/*
 * weatherTypeToServo() - take the weather type retrieved from the Met
 * Office, and move the servo to a position to indicate what the weather is
 * forecast to be
 */
void weatherTypeToServo()
{
  // Weather types are defined at http://www.metoffice.gov.uk/datapoint/support/code-definitions

  int pos =0;
  if ( (weatherType >=0) && (weatherType <=1) )
  {
    // Clear and sunny
    pos = 23;
  }
  else if ( (weatherType >= 2) && (weatherType <= 4) )
  {
    // Some cloud, but dry
    pos = 46;
  }
  else if ( (weatherType >= 5) && (weatherType <= 8) )
  {
    // Misty or overcast, but dry
    pos = 69;
  }
  else if ( (weatherType >= 9) && (weatherType <= 15) )
  {
    // Rain
    pos = 92;
  }
  else if ( (weatherType >= 16) && (weatherType <= 27) )
  {
    // Hail, sleet or snow
    pos = 115;
  }
  else if ( (weatherType >= 28) && (weatherType <= 30) )
  {
    // Thunder - treat it the same as rain
    pos = 138;
  }
  // Now we know what position the servo should be in, move it there
  myServo.write(pos);
}

