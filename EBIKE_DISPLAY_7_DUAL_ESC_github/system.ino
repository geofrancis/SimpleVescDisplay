/*
void SLEEPWAKE() {
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));
  print_wakeup_reason();
  //TIME_TO_SLEEP = map(MAINBATT,480,420,1000,60);
  //if (TIME_TO_SLEEP < 60){TIME_TO_SLEEP =59;}
  //if (TIME_TO_SLEEP > 1000){TIME_TO_SLEEP =1001;}
  //if (MAINBATTV <= 490) { TIME_TO_SLEEP = TIME_TO_SLEEPshort; }
  //if (MAINBATTV <= 470) { TIME_TO_SLEEP = TIME_TO_SLEEPlong; }
  //esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 1);  //1 = High, 0 = Low
  //esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  // Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");
  // Serial.println(esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH));
}


*/


void WIFICONNECT() {

  // Connecting to a WiFi AP
  WiFi.begin(ssid1, password1);
  Serial.println("Connecting to WiFi...");
  delay(2000);

  if (WiFi.isConnected()) {
  }

  if (!WiFi.isConnected()) {
    Serial.print("WIFI 1 FAIL: ");
    WiFi.disconnect();
    WiFi.begin(ssid2, password2);
    // Waiting until we connect to WiFi
    delay(5000);
    if (WiFi.isConnected()) {
    }
  }
  // Print the IP address in the local network
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void PINGSERVER() {
  // Ping Host
  const char* remote_host = "google.co.uk";
  Serial.print(remote_host);
  if (Ping.ping(remote_host) > 0) {
    Serial.printf(" response time : %d/%.2f/%d ms\n", Ping.minTime(), Ping.averageTime(), Ping.maxTime());
    pingok = 1;
  } else {
    Serial.println(" Ping Error !");
    pingok = 0;
  }
}

void OTASETUP() {

  ArduinoOTA
    .onStart([]() {
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

  ArduinoOTA.setHostname("ebike");
  ArduinoOTA.setPassword("password");
}








void wificheck() {
  Serial.println("WIFI CHECK");
  unsigned long wificurrent = millis();
  // if WiFi is down, try reconnecting

  if (!WiFi.isConnected()) {
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");

    WiFi.begin(ssid1, password1);
    Serial.println("Connecting to WiFi1...");
   // delay(2000);
    if (WiFi.isConnected()) {
      Serial.print("WIFI 1 Connect: ");
    }

    if (!WiFi.isConnected()) {
      Serial.print("WIFI 1 FAIL: ");
      WiFi.disconnect();

      WiFi.begin(ssid2, password2);
      Serial.println("Connecting to WiFi2...");

     // delay(5000);
      if (WiFi.isConnected()) {
        Serial.print("WIFI 2 Connect: ");
      }
    }
    // Print the IP address in the local network
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());



    wifimillis = wificurrent;
  }
}



/*


void sleepcheck() {

  Serial.println("sleepcheck............................................");
  Serial.println(SLEEPMODE);
 // if (SLEEPMODE == 60) {

    // Sleeptime();
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 1);  //1 = High, 0 = Low
    esp_sleep_enable_timer_wakeup(1000000ULL * TIME_TO_SLEEP);
    Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP / 60) + " Minutes");
    Serial.println("Going to sleep now");
    Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");
    Serial.println(MAINBATTV);

    // talkiesleep();

    delay(500);
    Serial.flush();
    esp_deep_sleep_start();
 // }
}

*/
/*
void print_wakeup_reason() {


  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case 1:
      messaged = "Wakeup caused by Omni RADAR";
      Serial.println("Wakeup caused by Omni RADAR");
      wakereason = 40;
      break;
    case 2:
      messaged = "Wakeup caused by RADAR";
      Serial.println("Wakeup caused by RADAR");
      wakereason = 10;
      break;
    case 3:
      messaged = "Wakeup caused by timer";
      Serial.println("Wakeup caused by timer");
      wakereason = 30;
      break;
    case 4:
      Serial.println("Wakeup caused by touchpad");
      wakereason = 20;
      break;

    case 5:

      messaged = "Wakeup caused by ULP program";
      Serial.println("Wakeup caused by ULP program");

      break;
    default:
      Serial.println("Solar Rover Reboot");
      messaged = "Solar Rover Reboot";
      wakereason = 50;
      if (bootCount = 1) { bootCount = 0; };
      talkierstartup();
      break;
  }
}


*/

void ESPWATCHSETUP() {

  Serial.println("Configuring WDT...");
  esp_task_wdt_deinit();            //wdt is enabled by default, so we need to deinit it first
  esp_task_wdt_init(&twdt_config);  //enable panic so ESP32 restarts
  esp_task_wdt_add(NULL);           //add current thread to WDT watch

  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 1);  //enable brownout detector
}


void ESPWATCH() {
  //Serial.println("Resetting Watchdog...");
  esp_task_wdt_reset();
}
