#include "ThingSpeak.h"
#include "WiFi101.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"
#include "Fonts/FreeSans9pt7b.h"
#include "Fonts/FreeSans12pt7b.h"



//Wifi info

char ssid[] = "YOUR SSID";
char pass[] = "YOUR PASSWORD";
int status = WL_IDLE_STATUS; 
WiFiClient  client;

//ThingSpeak

unsigned long myChannelNumber = YOUR CHANNEL NUMBER;
const char * myReadAPIKey = "YOUR WRITE API KEY";

//tft setup

#define TFT_CS     6
#define TFT_RST    9  // you can also connect this to the Arduino reset
                      // in which case, set this #define pin to 0!
#define TFT_DC     5

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

#define Background ST7735_BLACK
#define Alarm_Background ST7735_BLUE

void setup() {

  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  tft.setRotation(1);

  WiFi.setPins(8,7,4,2);   // MUST ALWAYS DEFINE these pins on Feather M0 wifi

   //Initialize serial an d wait for port to open:
  Serial.begin(9600);
 

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");

}

void loop() {

ThingSpeak.begin(client);
Serial.begin(9600);

float lt = ThingSpeak.readFloatField(myChannelNumber, 1, myReadAPIKey);
float lh = ThingSpeak.readFloatField(myChannelNumber, 2, myReadAPIKey);
float lux = ThingSpeak.readFloatField(myChannelNumber, 3, myReadAPIKey);

Serial.println();
Serial.println();
Serial.print("Temperature: ");
Serial.print(lt);
Serial.print("° F");
Serial.println();
Serial.print("Humidity: ");
Serial.print(lh);
Serial.print("%");
Serial.println();
Serial.print("Lux Value: ");
Serial.print(lux);
Serial.println();
Serial.println();

tft.setCursor(0,12);
tft.fillScreen(Background);
tft.setFont(&FreeSans9pt7b);

tft.println();
tft.print("Temp: ");
tft.print(lt);
tft.print("F");
tft.println();
tft.print("Hum: ");
tft.print(lh);
tft.print("%");
tft.println();
tft.print("Lux: ");
tft.print(lux);
tft.println();
tft.println();

delay(8500);

if((lh<=60) & (lh>=35) & (lt>=65) & (lt<=80))
{
tft.setCursor(0,24);
tft.fillScreen(Background);
tft.setFont(&FreeSans12pt7b);

tft.println();
tft.println("Status OK");
tft.println();
}
else
{
tft.setCursor(0,24);
tft.fillScreen(Alarm_Background);
tft.setFont(&FreeSans12pt7b);

tft.println();
tft.println("Status");
tft.println("NOT OK");
tft.println();
}
delay(8500);


}
