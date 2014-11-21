//#########################################################################
//######################## Web Server Local   #############################
//#########################################################################
/*
 "The Internet" is a big, wide, complicated place. To make sure everything with 
 the Arduino Ethernet board and your local network are working well we'll start 
 with an example that relies on only these. We'll create a simple web server 
 that will deliver a page with sensor readings (light and temperature) and 
 change the background color of the page based on the light intensity. To do 
 this simply wire up the IOTM-IN circuit (page 6), upload the code, plug your 
 board into your local network and open up the page in a web browser. 

 Based on WebServer Example Code
 created 18 Dec 2009 by David A. Mellis -- modified 4 Sep 2010 by 
 Tom Igoe -- extended Feb 2011 by Adrian McEwen
*/

//################# LIBRARIES ################

#include <SPI.h>        //Used to communicate with the ethernet chip
#include <WiFi.h>   //Used to interface with the ethernet portion

//char ssid[] = "yourNetwork"; //  your network SSID (name) 
//char pass[] = "secretPassword";    // your network password (use for WPA, or use as key for WEP)
char ssid[] = "BellaMacchina"; //  your network SSID (name) 
char pass[] = "D61445FB99";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

//################ PINS USED ######################
const int kLightSensorPin = A2;  //Light Sensor is connected to 
                                 //analog pin ...
const int kTempSensorPin = A3;   //Temperature Sensor is connected to 
                                 //analog pin ...

//################ VARIABLES ################

//------ NETWORK VARIABLES---------
//--MAKE CHANGE HERE--MAKE CHANGE HERE--MAKE CHANGE HERE--MAKE CHANGE HERE--MAKE CHANGE HERE--							
//IPAddress ip(***,***,***,***); //A free ip address on your network 
                             //(for more help http://iotm.org/NETW)
int port = 8080;  //Port your server will listen to 
                  //(80 is default for HTTP but the Edison already has one running there)
WiFiServer server(port);

//------ SKETCH VARIABLES -------------
// Variables used to hold the current color reading
int red = 0;   //current Red reading (between 0 and 255)
int green = 0; //current Green reading (between 0 and 255)
int blue = 0;  //current Blue reading (between 0 and 255)

/*
 * setup() - this function runs once when you turn your Arduino on 
 */
void setup()
{
  Serial.begin(9600);
  Serial.println("Setting Up IOTC-01");
  Serial.println(".:Web Server:. -- (local)");
  
  setupCircuit();  //sets up the connected circuit
  testCircuit();   //tests the connected circuit
  setupNetwork();  //sets up the boards WiFi connection
  
  Serial.println("IOTC-01 Code Running");
  Serial.print("    Visit http://");
  Serial.print(WiFi.localIP());
  Serial.print(":");
  Serial.print(port);
  Serial.println(" in a web browser to see the result.");  
}

/*
 * loop() - this function will start after setup finishes and then repeat
 */
void loop()
{
  takeColorReadingLight();     //take a color reading based on the light intensity (stored in red,green,blue)
//  takeColorReadingTemp();    //take a color reading based on the temperature intensity (stored in red,green,blue)
  setColor(red, green, blue);  //sets the on board LED to the current color (stored in red, green,blue)
  checkEthernet();             //checks to see if there has been a ethernet request
}

/*
 * setupCircuit() - this function sets all the pins to their correct mode
 */
void setupCircuit()
{
  Serial.print("    setupCircuit() Started..");
  pinMode(kTempSensorPin, INPUT);   //sets the temperature sensor pin as an input
  pinMode(kLightSensorPin, INPUT);  //sets the light sensor pin as an input
                                    //(Arduino pins are by default inputs so the above two lines 
                                    //are not neccesary but are good practice)
  Serial.println("..Finished..");
}

/*
 * testCircuit() - this function tests your circuit, turning the LED various 
 * colors and outputting the color it should be to the serial monitor
 */
void testCircuit()
{
  Serial.println("    testCircuit() Started..");
  
  Serial.println("      Testing LED");  //Test the LED by cycling through its colors
  Serial.print("        Red,");
  setColor(255, 0, 0);     //Set LED to red
  delay(750);              //wait just under a seconed
  Serial.print("Green,");
  setColor(0, 255, 0);     //Set LED to green
  delay(750);              //wait just under a seconed
  Serial.print("Blue,");
  setColor(0, 0, 255);     //Set LED to blue
  delay(750);              //wait just under a seconed
  Serial.println("White");
  setColor(255, 255, 255); //Set the LED to white
  delay(750);              //wait just under a seconed
  setColor(0, 0, 0);       //turn the LED off
  Serial.println("    testCircuit() Finished..");  
}  

/*
 * setupNetwork() - Sets up your WiFi connection
 */
void setupNetwork()
{
  Serial.print("    setupNetwork() Started..");
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
  server.begin();
  Serial.println("..Connected to wifi..");
}

/*
 * takeColorReadingLight() - Takes a color reading using the light sensors 
 * current value
 */
void takeColorReadingLight()
{
  const int readingMax = 650;  //largest reading (received when in 
                               //full darkness)
  const int readingMin = 90;   //smallest reading (received in full light)
                               //(these two values can be adjusted for your environment)

  int lightLevel = analogRead(kLightSensorPin);
                               //read the current light level and store it in a variable
  lightLevel = map(lightLevel,readingMin,readingMax,0,1023); 
                             //scale the reading to the full range of the LED
  red = 255 * hueToRGB(lightLevel/1000.0+(1.0/3));     // Work out how that maps onto the RGB color-space
  green = 255 * hueToRGB(lightLevel/1000.0);
  blue = 255 * hueToRGB(lightLevel/1000.0-(1.0/3));
}

/*
 * takeColorReadingTemperature() - Takes a color reading using the 
 * temperature sensors current value
 */
void takeColorReadingTemp()
{
  const int readingMax = 650;  //Higest temperature you want to display
  const int readingMin = 90;   //Lowest temperature you want to display
                               //(these two values can be adjusted for your environment)
  int tempLevel = analogRead(kTempSensorPin);
                               //read the current temperature value into a variable
  tempLevel = map(tempLevel,readingMin,readingMax,0,1023); 
                             //scale the reading to the full range of the LED
  red = 255 * hueToRGB(tempLevel/1000.0+(1.0/3));      // Work out how that maps onto the RGB color-space
  green = 255 * hueToRGB(tempLevel/1000.0);
  blue = 255 * hueToRGB(tempLevel/1000.0-(1.0/3));
}

/*
 * checkEthernet() - Checks for an ethernet connection attempt, if 
 * one is found a webpage is served up.
 */
void checkEthernet()
{
  WiFiClient client = server.available(); //listen for incoming clients
  if (client) {                               //If a request has been received
  
    boolean currentLineIsBlank = true; //variable for checking if end of request found.
    
    while (client.connected()) {       //this is a simple implementation which does not
                                       //capture any information from the request, it reads the entire
                                       //request and once the end is found (all http requests end with a blank line)
                                       //this is checked for by finding two new line '\n' in a row      
      if (client.available()) {        //if there is still an http request waiting
      
        char c = client.read();        //read the next charachter of the request
        
        if (c == '\n' && currentLineIsBlank) {  //if the last and current charachter are new lines '\n' the end of the
                                                //http request has been found, so return a web page
          
          Serial.print("      Serving Webpage at: "); //Send request timestamp to Serial Monitor
          Serial.println(millis());

          sendWebpage(client);  // Send the web page in response
          
          break;                                                            //exit this sub routine as the page is finished sending
        }
        
        //only reached if not returning a web page
        
        if (c == '\n') {               //if current charachter is a new line '\n'
          currentLineIsBlank = true;   //set a variable to mark the current charachter
                                       //as a new line, done because if the next charachter is
                                       //a new line the end of the request has been reached and
                                       //a web page needs to be returned
        } 
        else if (c != '\r') {          //if the current charachter isn't a new line
          currentLineIsBlank = false;  //set the variable telling the loop the last charachter wasn't
                                       //a new line
        }
      }
    }
    delay(1);       //give the web browser time to receive the data
    client.stop();  //close the connection:
  }
}

/*
 * sendWebpage(aClient) - Serves the webpage to the given network client
 */ 
void sendWebpage(WiFiClient aClient)
{
//###########################
//####### HTTP HEADER #######
//###########################
  aClient.println("HTTP/1.1 200 OK");          //send a standard http response header
  aClient.println("Content-Type: text/html");
  aClient.println();

//###########################
//#######  WEB PAGE   #######  //to customize change the page details below
//###########################
  aClient.println("<html><head>");                              //send html opening
  aClient.println("  <title>IOTC-01 Web Server</title>");       //send page title
  aClient.println("  <meta http-equiv='refresh' content='2'>"); //set the page to auto-refresh every 2 seconds
  aClient.println("</head>");                                   //close the head
  aClient.print("<body bgcolor='#");                            //opens the body and sets the background color to be the current LED
                                                               //color (for HTML conversion to HEX is required
                                                               //also a leading zero is required if less than 0x10
  if (red < 0x10)
    aClient.print("0");
  aClient.print(red, HEX);
  if (green < 0x10)
    aClient.print("0");
  aClient.print(green, HEX);
  if (blue < 0x10)
    aClient.print("0");
  aClient.print(blue, HEX);

  aClient.println("'>");                                          //end of setting background color
       
  aClient.println("<br>");                                        //the page contents change to customize
  aClient.println("<center>");
  aClient.println("<h2>IOTC-01 Web Server Page</h2>");
  aClient.println("<br>");
  aClient.println("<br>");
  aClient.print("Current Color: ");
  aClient.print(red, HEX);
  aClient.print(":");
  aClient.print(green, HEX);
  aClient.print(":");
  aClient.print(blue, HEX);
  aClient.println("<br>");
  aClient.print("Current Light Level: ");
  aClient.print(analogRead(kLightSensorPin));                       //send the current light level         
  aClient.println(" (0-1024)<br>");
  aClient.print("Current Temperature: ");
  float temperature = 
           (((analogRead(kTempSensorPin) *  .004882814))-0.5)*100;  
  aClient.print(temperature);                                        //send the current temperature 
  aClient.println(" degrees<br>");
  aClient.println("</center>");        
  aClient.println("</body></html>");                                 //close the body and html page
}

/*
 * setColor(aRed, aGreen, aBlue) - Sets the LED to the supplied RGB
 * color. 
 * aRed,aGreen,aBlue Range: 0-255
 */
void setColor(int aRed, int aGreen, int aBlue)
{
#if defined USE_RGB_LED  
  analogWrite(kRedPin, 255-aRed);     //write the current red value to the LED
                                      //(we use a common anode LED so full on is analogWrite(0)
                                      //and full off analogWrite(255) this is why 255-ared)
  analogWrite(kGreenPin, 255-aGreen); //write the current green value to the LED
  analogWrite(kBluePin, 255-aBlue);   //write the current blue value to the LED
#endif  
}

/*
 * hueToRGB(aHue) Convert from HSL color space into RGB
 * See http://en.wikipedia.org/wiki/HSL_and_HSV for a good explanation
 * color. 
 * aHue color value, 0.333 is green, 0.666 blue, 0 or 1 is red
 */
float hueToRGB(float aHue)
{
  const float aSaturation = 0.9;  //Sets the overall saturation of the LED
  const float aLightness = 0.5;  //Sets the overall lightness of the LED
  float ret;
  float v1;
  float v2;
  
  if (aSaturation == 0)
  {
    return aLightness;
  }
  
  if (aLightness < 0.5)
    v2 = aLightness * (aLightness+aSaturation);
  else
    v2 = (aLightness+aSaturation)-(aSaturation*aLightness);
  v1 = 2*aLightness-v2;
  
  aHue = constrain(aHue, -1.0, 2.0);
  if (aHue < 0)
    aHue += 1;
  if (aHue > 1)
    aHue -= 1;

  if ((6*aHue) < 1)
  {
    return v1 + (v2-v1)*6*aHue;
  }
  if ((2*aHue) < 1)
  {
    return v2;
  }
  if ((3*aHue) < 2)
  {
    return (v1 + (v2-v1) * ((2.0/3) - aHue)*6);
  }
  return v1;
}
