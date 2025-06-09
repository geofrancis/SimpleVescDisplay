
void setup(void) {

  VescSerial.begin(115200, SERIAL_8N1, RXD2, TXD2);  // VESC RX TX
  // Wait for the Serial port to be ready
  while (!VescSerial) {
    delay(10);
  }
  UART.setSerialPort(&VescSerial);
#ifdef DEBUG_MODE
  Serial.begin(115200);  // Debug MicroUSB ?
  UART.setDebugPort(&Serial);
#endif


  //BMS.begin(115200);
  bms.begin(&Serial);
  bms.main_task(true);


  EEPROM.begin(100);
  TFT_SETUP();





  last_total_km_stored = startup_total_km;
  tacho = (UART.data.tachometerAbs / (MOTOR_POLES * 3));
  trip = tacho / 1000;

  if (startup_total_km != 0) {
    startup_total_km = startup_total_km - trip;
  }

#ifdef DO_LOGO_DRAW
  int16_t rc_bg = png.openFLASH((uint8_t*)startup_image, sizeof(startup_image), pngDraw);
  if (rc_bg == PNG_SUCCESS) {
    tft.startWrite();
    rc_bg = png.decode(NULL, 0);
    tft.endWrite();
  }



  png.close();
  delay(3000);
  tft.fillScreen(TFT_BLACK);
  int16_t rc_mainbg = png.openFLASH((uint8_t*)background_image, sizeof(background_image), pngDraw);

  if (rc_mainbg == PNG_SUCCESS) {
    tft.startWrite();
    rc_mainbg = png.decode(NULL, 0);
    tft.endWrite();
  }
  png.close();
#endif
 // UART.setCurrent(0,1);




  WIFICONNECT();
  PINGSERVER();

  OTASETUP();
  configTime(1 * 60 * 60, 3600, "uk.pool.ntp.org");
  // ESPWATCHSETUP();






  ArduinoOTA.onStart([]() {
              String type;
              if (ArduinoOTA.getCommand() == U_FLASH) {
                type = "sketch";
              } else {  // U_SPIFFS
                type = "filesystem";
              }

              // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
              Serial.println("Start updating " + type);
            })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
      } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
      } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
      } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
      }
    });

  ArduinoOTA.setHostname("Ebike_Display");
  ArduinoOTA.setPassword("password");
  ArduinoOTA.begin();
}
