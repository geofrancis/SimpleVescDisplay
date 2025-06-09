void TFT_SETUP() {


  tft.begin();
  tft.setRotation(3);
   tft.invertDisplay( true ); 
  tft.fillScreen(TFT_BLACK);
  UART.getVescValues();
  EEPROM_readAnything(EEPROM_MAGIC_VALUE, startup_total_km);
  if (isnan(startup_total_km)) {
    tft.setCursor(40, 160);
    tft.setTextFont(4);
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("SETUP EPROM...");
    for (int i = 0; i < 100; i++) {
      EEPROM_writeAnything(i, 0);
    }
    EEPROM_writeAnything(EEPROM_MAGIC_VALUE, 0.0);
    delay(1500);
    ESP.restart();
  }
}


void BACKLIGHT() {


  sensorValue = analogRead(LDR_PIN);

  if (sensorValue > 200) { sensorValue = 200; }
  brightness = map(sensorValue, 0, 200, 255, 1);


  if (brightnessold > 253) { brightnessold = 253; }

  if (brightnessold < brightness) {
    if (brightnessold <= 255) {
      analogWrite(LCD_BACK_LIGHT_PIN, (brightnessold + 1));
      brightnessold = (brightnessold + 1);
    }
  }
  if (brightnessold < 2) { brightnessold = 2; }

  if (brightnessold > brightness) {
    if (brightnessold >= 0) {
      analogWrite(LCD_BACK_LIGHT_PIN, (brightnessold - 1));
      brightnessold = (brightnessold - 1);
    }
  }


  /*
int sensorValue = analogRead(LDR_PIN);
  if(sensorValue <= 200){sensorValue = 200;}
  brightness = map(sensorValue, 200, 1000, 255, 25);
  if(brightness <= 25){brightness=25;} //stupid map function gets destroyed when input value goes over specified 
  analogWrite(LCD_BACK_LIGHT_PIN, brightness);
*/
}



void TFT() {

 tft.invertDisplay( true ); 

  /*

  //RAIN --------------------------------------------------------------------------

  tft.setFreeFont(DATAFONTSMALL2);
  Data13.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setCursor(275, 25);
  dtostrf(rain, 3, 1, fmt);
  Data13.print(fmt);

  tft.setCursor(285, 30);
  tft.setTextFont(1);
  Data4t.setTextColor(TFT_WHITE, TFT_BLACK);
  Data4t.print("Rain");






  //Wind Gust --------------------------------------------------------------------------

  tft.setFreeFont(DATAFONTSMALL2);
  Data14.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setCursor(275, 60);
  dtostrf((windg * 2.23), 3, 1, fmt);
  Data14.print(fmt);

  tft.setCursor(285, 65);
  tft.setTextFont(1);
  Data4t.setTextColor(TFT_WHITE, TFT_BLACK);
  Data4t.print("Gust");




  //Wind Speed --------------------------------------------------------------------------

  tft.setFreeFont(DATAFONTSMALL2);
  Data15.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setCursor(275, 98);
  dtostrf((winds * 2.23), 3, 1, fmt);
  Data15.print(fmt);

  tft.setCursor(285, 103);
  tft.setTextFont(1);
  Data4t.setTextColor(TFT_WHITE, TFT_BLACK);
  Data4t.print("Wind");



  //Wind Direction --------------------------------------------------------------------------

  tft.setFreeFont(DATAFONTSMALL2);
  Data16.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setCursor(275, 133);
  dtostrf(WINDD, 3, 0, fmt);
  Data16.print(fmt);

  tft.setCursor(285, 137);
  tft.setTextFont(1);
  Data4t.setTextColor(TFT_WHITE, TFT_BLACK);
  Data4t.print("Dir");






  tft.setCursor(9, 70);
  tft.setTextFont(1);
  Data17.setTextColor(TFT_WHITE, TFT_BLACK);
  Data17.print("TIDE");


  tft.setCursor(5, 79);
  tft.setTextFont(1);
  Data17.setTextColor(TFT_WHITE, TFT_BLACK);
  Data17.print(tide);

  tft.setCursor(9, 50);
  tft.setTextFont(1);
  Data17.setTextColor(TFT_WHITE, TFT_BLACK);
  Data17.print("ISS");


  tft.setCursor(5, 59);
  tft.setTextFont(1);
  Data17.setTextColor(TFT_WHITE, TFT_BLACK);
  Data17.print(distanceMiles);


  if (!WiFi.isConnected()) {
    tft.setCursor(5, 180);
    tft.setTextFont(1);
    Data17.setTextColor(TFT_RED, TFT_BLACK);
    Data17.print("Disconnected");
  }

  if (WiFi.isConnected()) {
    tft.setCursor(5, 180);
    tft.setTextFont(1);
    Data17.setTextColor(TFT_GREEN, TFT_BLACK);
    Data17.print("Connected");
  }




  if (BMSvoltage > 0) {
    // BMS

    tft.setCursor(0, 50);
    tft.setTextFont(1);
    Data4t.setTextColor(TFT_WHITE, TFT_BLACK);
    Data4t.print(CELLS[0]);

    tft.setCursor(0, 60);
    tft.setTextFont(1);
    Data4t.setTextColor(TFT_WHITE, TFT_BLACK);
    Data4t.print(CELLS[1]);

    tft.setCursor(0, 70);
    tft.setTextFont(1);
    Data4t.setTextColor(TFT_WHITE, TFT_BLACK);
    Data4t.print(CELLS[2]);

    tft.setCursor(0, 80);
    tft.setTextFont(1);
    Data4t.setTextColor(TFT_WHITE, TFT_BLACK);
    Data4t.print(CELLS[3]);

    tft.setCursor(0, 90);
    tft.setTextFont(1);
    Data4t.setTextColor(TFT_WHITE, TFT_BLACK);
    Data4t.print(CELLS[4]);

    tft.setCursor(0, 100);
    tft.setTextFont(1);
    Data4t.setTextColor(TFT_WHITE, TFT_BLACK);
    Data4t.print(CELLS[5]);

    tft.setCursor(0, 110);
    tft.setTextFont(1);
    Data4t.setTextColor(TFT_WHITE, TFT_BLACK);
    Data4t.print(CELLS[6]);

    tft.setCursor(0, 120);
    tft.setTextFont(1);
    Data4t.setTextColor(TFT_WHITE, TFT_BLACK);
    Data4t.print(CELLS[7]);

    tft.setCursor(0, 130);
    tft.setTextFont(1);
    Data4t.setTextColor(TFT_WHITE, TFT_BLACK);
    Data4t.print(CELLS[8]);

    tft.setCursor(0, 140);
    tft.setTextFont(1);
    Data4t.setTextColor(TFT_WHITE, TFT_BLACK);
    Data4t.print(CELLS[9]);

    tft.setCursor(0, 150);
    tft.setTextFont(1);
    Data4t.setTextColor(TFT_WHITE, TFT_BLACK);
    Data4t.print(CELLS[10]);

    tft.setCursor(0, 160);
    tft.setTextFont(1);
    Data4t.setTextColor(TFT_WHITE, TFT_BLACK);
    Data4t.print(CELLS[11]);

    tft.setCursor(0, 170);
    tft.setTextFont(1);
    Data4t.setTextColor(TFT_WHITE, TFT_BLACK);
    Data4t.print(CELLS[12]);
  }


*/







  //Main Speed --------------------------------------------------------------------------

  int speedINT = _max(speed, 0);
  if (speedINT > HIGH_SPEED_WARNING) {
    COLOR_WARNING_SPEED = TFT_RED;
  } else {
    COLOR_WARNING_SPEED = TFT_WHITE;
  }
  Data1.setTextColor(TFT_BLACK, TFT_BLACK);

  tft.setFreeFont(SPEEDFONT);
  tft.setCursor(83, 162);
  Data1.setTextColor(COLOR_WARNING_SPEED, TFT_BLACK);
  Data1.print(speedINT);






  //Vesc Temp --------------------------------------------------------------------------

  if (esctemp > VESC_TEMP_WARNING1) {
    COLOR_WARNING_TEMP_VESC = TFT_YELLOW;
  }
  if (esctemp > VESC_TEMP_WARNING2) {
    COLOR_WARNING_TEMP_VESC = TFT_RED;
  } else {
    COLOR_WARNING_TEMP_VESC = TFT_GREEN;
  }

  tft.setCursor(65, 220);
  tft.setFreeFont(DATAFONTSMALL);
  Data2.setTextColor(COLOR_WARNING_TEMP_VESC, TFT_BLACK);
  dtostrf(esctemp, 3, 0, fmt);
  Data2.print(fmt);


  if (esctemp1 > VESC_TEMP_WARNING1) {
    COLOR_WARNING_TEMP_VESC = TFT_YELLOW;
  }
  if (esctemp1 > VESC_TEMP_WARNING2) {
    COLOR_WARNING_TEMP_VESC = TFT_RED;
  } else {
    COLOR_WARNING_TEMP_VESC = TFT_GREEN;
  }

  tft.setCursor(5, 220);
  tft.setFreeFont(DATAFONTSMALL);
  Data22.setTextColor(COLOR_WARNING_TEMP_VESC, TFT_BLACK);
  dtostrf(esctemp1, 3, 0, fmt);
  Data22.print(fmt);


  tft.setCursor(15, 235);
  tft.setFreeFont(DATAFONTSMALLTEXT);
  Data2t.setTextColor(TFT_WHITE, TFT_BLACK);
  Data2t.print("RT");



  tft.setCursor(65, 235);
  tft.setFreeFont(DATAFONTSMALLTEXT);
  Data11t.setTextColor(TFT_WHITE, TFT_BLACK);
  Data11t.print("FT");



  /*

  //Motor Temp --------------------------------------------------------------------------

  if (mottemp > MOTOR_TEMP_WARNING1) {
    COLOR_WARNING_TEMP_MOTOR = TFT_YELLOW;
  }
  if (mottemp > MOTOR_TEMP_WARNING2) {
    COLOR_WARNING_TEMP_MOTOR = TFT_RED;
  } else {
    COLOR_WARNING_TEMP_MOTOR = TFT_GREEN;
  }

  tft.setCursor(65, 180);
  tft.setFreeFont(DATAFONTSMALL);
  Data11.setTextColor(COLOR_WARNING_TEMP_MOTOR, TFT_BLACK);
  dtostrf(mottemp, 3, 0, fmt);
  Data11.print(fmt);

  if (mottemp1 > MOTOR_TEMP_WARNING1) {
    COLOR_WARNING_TEMP_MOTOR = TFT_YELLOW;
  }
  if (mottemp1> MOTOR_TEMP_WARNING2) {
    COLOR_WARNING_TEMP_MOTOR = TFT_RED;
  } else {
    COLOR_WARNING_TEMP_MOTOR = TFT_GREEN;
  }

  tft.setCursor(65, 220);
  tft.setFreeFont(DATAFONTSMALL);
  Data22.setTextColor(COLOR_WARNING_TEMP_MOTOR, TFT_BLACK);
  dtostrf(mottemp1, 3, 0, fmt);
  Data22.print(fmt);



  tft.setCursor(65, 235);
  tft.setFreeFont(DATAFONTSMALLTEXT);
  Data11t.setTextColor(TFT_WHITE, TFT_BLACK);
  Data11t.print("MotorT");




*/






  //Battery Voltage --------------------------------------------------------------------------

  if (calvolts > BATTERY_WARNING_HIGH) {
    BATTERY_WARNING_COLOR = TFT_RED;
  } else if (calvolts < BATTERY_WARNING_0) {
    BATTERY_WARNING_COLOR = TFT_RED;
  } else if (calvolts < BATTERY_WARNING_LOW) {
    BATTERY_WARNING_COLOR = TFT_YELLOW;
  } else if (BATTERY_WARNING_LOW < calvolts < BATTERY_WARNING_HIGH) {
    BATTERY_WARNING_COLOR = TFT_GREEN;
  }

  tft.setFreeFont(DATAFONTSMALL2);
  Data4.setTextColor(BATTERY_WARNING_COLOR, TFT_BLACK);
  tft.setCursor(10, 25);
  dtostrf(calvolts, 3, 1, fmt);
  Data4.print(fmt);

  tft.setCursor(10, 30);
  tft.setTextFont(1);
  Data4t.setTextColor(TFT_WHITE, TFT_BLACK);
  Data4t.print("Battery");





  //Battery CELL --------------------------------------------------------------------------

  if (calvolts > 4.2) {
    BATTERY_WARNING_COLOR = TFT_RED;
  } else if (calvolts < 3.1) {
    BATTERY_WARNING_COLOR = TFT_RED;
  } else if (calvolts < 3.5) {
    BATTERY_WARNING_COLOR = TFT_YELLOW;
  } else if (3.5 < calvolts < 4.2) {
    BATTERY_WARNING_COLOR = TFT_GREEN;
  }

  tft.setFreeFont(DATAFONTSMALL2);
  Data12.setTextColor(BATTERY_WARNING_COLOR, TFT_BLACK);
  tft.setCursor(70, 25);
  dtostrf(CELLc, 3, 1, fmt);
  Data12.print(fmt);

  tft.setCursor(70, 30);
  tft.setTextFont(1);
  Data4t.setTextColor(TFT_WHITE, TFT_BLACK);
  Data4t.print("Cell");



  //Watt/Error display --------------------------------------------------------------------------

  if (UART.data.error == 0) {  // Display Watts when no error(0)
    tft.setFreeFont(DATAFONTSMALL2);
    Data10.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(160, 25);
    dtostrf(watts, 5, 0, fmt);
    Data10.print(fmt);

    tft.setFreeFont(DATAFONTSMALL2);
    Data24.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(220, 25);
    dtostrf(watts1, 5, 0, fmt);
    Data24.print(fmt);

    tft.setCursor(170, 30);
    tft.setTextFont(1);
    Data10t.setTextColor(TFT_WHITE, TFT_BLACK);
    Data10t.print("Watts");
  } else {  //	Error display when present
    if (UART.data.error > 0) {
      ERROR_WARNING_COLOR = TFT_RED;
    } else {
      ERROR_WARNING_COLOR = TFT_GREEN;
    }
    tft.setFreeFont(DATAFONTSMALL2);
    Data10.setTextColor(ERROR_WARNING_COLOR, TFT_BLACK);
    tft.setCursor(170, 25);
    dtostrf(UART.data.error, 2, 0, fmt);
    Data10.print(fmt);

    tft.setCursor(170, 30);
    tft.setTextFont(1);
    Data10t.setTextColor(TFT_WHITE, TFT_BLACK);
    Data10t.print("Error");
  }





  //Motor-Phase Current front --------------------------------------------------------------------------

  tft.setCursor(270, 180);
  tft.setFreeFont(DATAFONTSMALL);
  Data6.setTextColor(TFT_GREEN, TFT_BLACK);
  dtostrf(current1, 3, 0, fmt);
  Data6.print(fmt);


  //Motor-Phase Current --------------------------------------------------------------------------

  tft.setCursor(270, 220);
  tft.setFreeFont(DATAFONTSMALL);
  Data20.setTextColor(TFT_GREEN, TFT_BLACK);
  dtostrf(current, 3, 0, fmt);
  Data20.print(fmt);

  tft.setCursor(265, 235);
  tft.setFreeFont(DATAFONTSMALLTEXT);
  Data6t.setTextColor(TFT_WHITE, TFT_BLACK);
  Data6t.print("PHASE A");

  //Battery Current --------------------------------------------------------------------------

  tft.setCursor(220, 180);
  tft.setFreeFont(DATAFONTSMALL);
  Data7.setTextColor(TFT_GREEN, TFT_BLACK);
  dtostrf(amps1, 3, 0, fmt);
  Data7.print(fmt);

  tft.setCursor(220, 220);
  tft.setFreeFont(DATAFONTSMALL);
  Data21.setTextColor(TFT_GREEN, TFT_BLACK);
  dtostrf(amps, 3, 0, fmt);
  Data21.print(fmt);


  tft.setCursor(220, 120);
  tft.setFreeFont(DATAFONTSMALL);
  Data23.setTextColor(TFT_GREEN, TFT_BLACK);
  dtostrf((amps + amps1), 3, 0, fmt);
  Data23.print(fmt);



  tft.setCursor(215, 235);
  tft.setFreeFont(DATAFONTSMALLTEXT);
  Data7t.setTextColor(TFT_WHITE, TFT_BLACK);
  Data7t.print("BATT A");

  //Odometer Text (TRIP) --------------------------------------------------------------------------

  tft.setCursor(145, 220);
  tft.setFreeFont(DATAFONTSMALL);

  Data9.setTextColor(TFT_WHITE, TFT_BLACK);
  dtostrf(total_km, 4, 0, fmt);
  Data9.print(fmt);

  tft.setCursor(135, 235);
  tft.setFreeFont(DATAFONTSMALLTEXT);
  Data9t.setTextColor(TFT_WHITE, TFT_BLACK);
  Data9t.print("ODOMETER");

  // delay(Screen_refresh_delay);
  checkvalues();
}