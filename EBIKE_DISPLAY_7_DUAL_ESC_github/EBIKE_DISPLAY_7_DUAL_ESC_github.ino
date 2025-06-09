#include <FlickerFreePrint.h>  // Flicker-free printing to TFT
#include <ComEVesc.h>          // Communication with VESC controller
#include <TFT_eSPI.h>          // Hardware-specific library for TFT display
#include <SPI.h>
#include "EEPROMAnything.h"  // Easy access to EEPROM storage

#include <TFT_eWidget.h>     

#include <ArduinoOTA.h>
#include "esp_task_wdt.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <ESPping.h>
#include <chrono>
#include <NetworkUdp.h>
#include <ESP32Time.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TinyGPSPlus.h>
#include <bms2.h>


#include <ESP32Time.h>
#include <time.h>





// WiFi settings
const char ssid1[] = "2.4";
const char password1[] = "password";
const char ssid2[] = "2.4";
const char password2[] = "password";
const char* login = "xxxxxxxxxxxxxxxxx";
const char* domoticz_server = "xxxxx.xxxxx.org";  //Domoticz port
int port = 8080;                                          //D

//int status = WL_IDLE_STATUS;     // the Wifi radio's status
WiFiClient client;  // or WiFiClientSecure for HTTPS
HTTPClient http;


#define WDT_TIMEOUT 120000
//if 1 core doesn't work, try with 2
#define CONFIG_FREERTOS_NUMBER_OF_CORES 0
int lastt = millis();

esp_task_wdt_config_t twdt_config = {
  .timeout_ms = WDT_TIMEOUT,
  .idle_core_mask = (1 << CONFIG_FREERTOS_NUMBER_OF_CORES) - 1,  // Bitmask of all cores
  .trigger_panic = true,
};

int sensorValue;


//ESP32Time rtc;
ESP32Time rtc(-3600);  // offset in seconds GMT+1



RTC_DATA_ATTR int bootCount = 0;
int wakereason = 0;


int pingok = 0;
int CELLS[13];
//int TEMP;
int VOLTAGE;
float CELLc;
int CURRENT;
int SOC;
int CAPACITY;


RTC_DATA_ATTR long GPSLAT = 0.1234567;
RTC_DATA_ATTR long GPSLON = 0.1234567;


const char* RAINFALL;
const char* RAINRATE;
int WINDD;
const char* WINDS;
const char* WINDG;
const char* WINDSS;
const char* WINDGG;
float TEMP;
float CHILL;
const char* VISIBILITY;
const char* VISIBILITYY;
const char* CLOUD;
float rain;
float rainrate;
float winds;
float windg;
float visi;
float cloud;
RTC_DATA_ATTR int SUNUP;
const char* DAYLEGNTH;
const char* SUNRISE;
const char* SUNSET;

float DayLength;
int dom_thr;
float elevation;
float azimuth;
int sun_azimuth;
int sun_elevation;


String time_str, current_hour, current_minute, current_day, current_month, current_year;




// The TinyGPSPlus object
TinyGPSPlus gps;
unsigned long last = 0UL;

String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

/* Coming from util.h */
#define htonl(x) (((x) << 24 & 0xFF000000UL) | ((x) << 8 & 0x00FF0000UL) | ((x) >> 8 & 0x0000FF00UL) | ((x) >> 24 & 0x000000FFUL))
#define htons(x) (((x) << 8) | (((x) >> 8) & 0xFF))




esp_sleep_wakeup_cause_t wakeup_reason;



unsigned long previousMillis1 = 0;
long LOOP1 = (1 * 500);


unsigned long previousMillis2 = 0;
long LOOP2 = (1 * 10000);



unsigned long wifimillis = 0;
unsigned long wifiinterval = 5000;


OverkillSolarBms2 bms = OverkillSolarBms2();

unsigned long timer = millis();


// Global variables for tracking speed, distance, and motor data
float trip;
float startup_total_km;
float last_total_km_stored;
float total_km;


float tacho;
float rpm;
float speed;
float watts;
float amps;
float wheel_diameter;
float calvolts;
float mottemp;
float esctemp;

float current = 0; /** The current in amps */
float current1 = 0; /** The current in amps */

int maxspeed;
int brightness = 255;
int brightnessold = 255;
char fmt[10];


float trip1;
float tacho1;
float rpm1;
float speed1;
float watts1;
float amps1;
float wheel_diameter1;
float calvolts1;
float mottemp1;
float esctemp1;
float BMSvoltage = 0;


const char* apiURL = "http://api.open-notify.org/iss-now.json";

// Your location's latitude and longitude
const float myLatitude = 5.635; 
const float myLongitude = -0.785;
const float alertRadiusMiles = 500;


float distanceMiles;


const char* apitideURL = "https://environment.data.gov.uk/flood-monitoring/id/stations/E74023";
float tide;








































// Font settings for various display elements
#define SPEEDFONT &JerseyM54_82pt7b          // Large font for displaying speed
#define DATAFONTSMALL2 &JerseyM54_14pt7b     // Small font for other data values
#define DATAFONTSMALL &JerseyM54_18pt7b      // Alternative small font
#define DATAFONTSMALLTEXT &Blockletter8pt7b  // Font for smaller text labels

// Motor and wheel parameters for speed calculation
#define MOTOR_POLES 30         // Number of motor poles (30 for typical E-scooters)
#define WHEEL_DIAMETER_MM 224  // Diameter of the wheel in millimeters
#define GEAR_RAITO 1.0         // Motor pulley teeth count (1:1 gearing)
#define PI 3.141592            // Pi constant
#define SCONST 0.12            // Conversion factor from RPM to speed (km/h)

// Pin configs for the hardware

#define RXD2 22  // DISPLAY RX 22 TO VESC TX
#define TXD2 27  // DISPLAY TX 27 TO VESC RX

#define LDR_PIN 34  // Light Detecting Resistor that sits on top right of the display
#define LCD_BACK_LIGHT_PIN 21

// User-configurable settings for warnings and thresholds
int EEPROM_MAGIC_VALUE = 0;        // EEPROM magic value to track saved data
#define EEPROM_UPDATE_EACH_KM 0.1  // Frequency of EEPROM updates (every 0.1 km)

int COLOR_WARNING_SPEED = TFT_RED;  // Color for speed warning display
#define HIGH_SPEED_WARNING 15       // Speed threshold for warnings (60 km/h)

int COLOR_WARNING_TEMP_VESC = TFT_WHITE;  // Color for VESC temperature warnings
#define VESC_TEMP_WARNING1 50             // First temperature warning threshold (50°C)
#define VESC_TEMP_WARNING2 80             // Second temperature warning threshold (80°C)

int COLOR_WARNING_TEMP_MOTOR = TFT_WHITE;  // Color for motor temperature warnings
#define MOTOR_TEMP_WARNING1 80             // First motor temperature warning threshold (80°C)
#define MOTOR_TEMP_WARNING2 120            // Second motor temperature warning threshold (120°C)

int BATTERY_WARNING_COLOR = TFT_WHITE;  // Color for battery voltage warnings
#define BATTERY_WARNING_HIGH 55.4       // High voltage warning threshold (67.2V)
#define BATTERY_WARNING_LOW 47          // Low voltage warning threshold (54.4V)
#define BATTERY_WARNING_0 44            // Voltage Cut warning treshold(48V)

int ERROR_WARNING_COLOR = TFT_WHITE;  // Color for error warnings

#define DO_LOGO_DRAW  // Uncomment if you want enable startup logo and background logo [Currently disbaled version doesn't work so don't disable!]
#define DEBUG_MODE

#ifdef DO_LOGO_DRAW
#include <PNGdec.h>            // PNG decoder library
#include "startup_image.h"     // PNG data for startup logo
#include "background_image.h"  // PNG data for background image
PNG png;                       // PNG decoder instance
int16_t xpos = 0;              // X position for image drawing
int16_t ypos = 0;              // Y position for image drawing
#define MAX_IMAGE_WDITH 320    // Maximum image width for display
#endif

// Other settings
int Screen_refresh_delay = 50;  // Delay between screen refreshes (ms)
ComEVesc UART;                  // VESC UART instance
HardwareSerial VescSerial(1);





TFT_eSPI tft = TFT_eSPI();
FlickerFreePrint<TFT_eSPI> Data1(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data2(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data3(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data4(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data5(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data6(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data7(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data8(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data9(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data10(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data11(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data12(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data13(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data14(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data15(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data16(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data17(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data18(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data19(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data20(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data21(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data1t(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data2t(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data3t(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data4t(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data5t(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data6t(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data7t(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data8t(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data9t(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data10t(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data11t(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data22(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data23(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data24(&tft, TFT_WHITE, TFT_BLACK);
FlickerFreePrint<TFT_eSPI> Data25(&tft, TFT_WHITE, TFT_BLACK);

void pngDraw(PNGDRAW* pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WDITH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  tft.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}

void checkvalues() {
  total_km = startup_total_km + trip;
  bool traveled_enough_distance = (total_km - last_total_km_stored >= EEPROM_UPDATE_EACH_KM);
  if (traveled_enough_distance) {
    last_total_km_stored = total_km;
    EEPROM_writeAnything(EEPROM_MAGIC_VALUE, total_km);
  }
}






void loop() {

  VESCF();
  VESCR();
  TFT();

  ArduinoOTA.handle();


  unsigned long currentMillis1 = millis();
  if (currentMillis1 - previousMillis1 >= LOOP1) {
    previousMillis1 = currentMillis1;
    Serial.println("loop1...................................................");
    BACKLIGHT();
  }


  unsigned long currentMillis2 = millis();
  if (currentMillis2 - previousMillis2 >= LOOP2) {
    previousMillis2 = currentMillis2;
    Serial.println("loop2...................................................");
    // BMS();
    sendvoltage();
   // sendspeed();
   // weather();
    //ISS();
   // fetchTideData();
  
    //wificheck();
  }
}
