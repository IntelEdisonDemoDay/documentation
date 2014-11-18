/* 
 * TimePosixLCD.ino
 * example code illustrating Time library set through a Linux date command.
 * Time is displayed on the Grove I2C LCD
 *
 * This works on the Intel Edison (and Galileo board?)
 */ 
 
#include <Time.h> 
#include <Wire.h>
#include <rgb_lcd.h>

rgb_lcd lcd;
const int colorR = 64;
const int colorG = 64;
const int colorB = 64;

char buf[12];
time_t getPosixTime()
{
  system("date '+%s' > /home/root/time.txt");  //get current epoch time
                                               //and save in text file time.txt located in /home/root
  FILE *fp;
  fp = fopen("/home/root/time.txt", "r");
  fgets(buf, 11, fp);
  fclose(fp);  
  return atol(buf);
}

void setup()  {
  Serial.begin(9600);
  while (!Serial) ; // Needed for Leonardo only
  pinMode(13, OUTPUT);
  setSyncProvider( getPosixTime);  //set function to call when sync required
      // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
    
  lcd.setRGB(colorR, colorG, colorB);
  lcd.print("Waiting for sync");
  Serial.println("Waiting for sync message");
}

void loop(){    
  if (timeStatus()!= timeNotSet) {
    digitalClockDisplay();  
    lcdClockDisplay();
  }
  if (timeStatus() == timeSet) {
    digitalWrite(13, HIGH); // LED on if synced
  } else {
    digitalWrite(13, LOW);  // LED off if needs refresh
  }
  delay(1000);
}

void lcdClockDisplay(){
  // digital clock display of the time
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(hour());
  lcdPrintDigits(minute());
  lcdPrintDigits(second());
  lcd.setCursor(0, 1);
  lcd.print(day());
  lcd.print(" ");
  lcd.print(month());
  lcd.print(" ");
  lcd.print(year()); 
}

void lcdPrintDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  lcd.print(":");
  if(digits < 10)
    lcd.print('0');
  lcd.print(digits);
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

