/*

 Simple IoTKit Client
 
 Written 2014 by Adrian McEwen
 
 This code is in the public domain.
 
 */

#include <SPI.h>         
#include <WiFi.h>
#include <WiFiUdp.h>

int status = WL_IDLE_STATUS;
char ssid[] = "mynetwork";  //  your network SSID (name)
char pass[] = "mypassword";       // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)

unsigned int localPort = 1234;      // local port to listen for UDP packets

const int PACKET_BUFFER_SIZE = 255;

char packetBuffer[PACKET_BUFFER_SIZE]; //buffer to hold incoming and outgoing packets 

const int pinTemp = A0;      // pin of temperature sensor

float temperature;
int B=3975;                  // B value of the thermistor
float resistance;

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;

void setup() 
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

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
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  Serial.println("Connected to wifi");
  printWifiStatus();

  Udp.begin(localPort);
  
  // First off we need to register the type of sensor
  Serial.println("Registering sensor");
  sprintf(packetBuffer, "\{\"n\": \"temperature sensor\", \"t\":\"temperature.v1.0\"}");
  sendPacket(packetBuffer);
}

void loop()
{
  // Take a temperature reading
  int val = analogRead(pinTemp);                               // get analog value
  resistance=(float)(1023-val)*10000/val;                      // get resistance
  temperature=1/(log(resistance/10000)/B+1/298.15)-273.15;     // calc temperature
  Serial.print("Temperature is ");
  Serial.println(temperature);

  // And send it to the agent
  sprintf(packetBuffer, "\{\"n\": \"temperature sensor\", \"v\":\"%f\"}", temperature);
  sendPacket(packetBuffer);

  // wait ten seconds before checking again
  delay(10000); 
}

// send a request to the IoTKit agent, which will be running on our Edison
unsigned long sendPacket(char* aPacket)
{
  IPAddress agent(WiFi.localIP());
  Udp.beginPacket(agent, 41234); //IoTKit agent listens on 41234

  Serial.println("Sending packet...");
  Serial.println(aPacket);
  Udp.write((byte*)aPacket, strlen(aPacket));
  Udp.endPacket(); 
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}






