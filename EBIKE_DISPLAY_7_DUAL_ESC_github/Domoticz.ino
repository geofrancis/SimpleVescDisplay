




void weather(){
Rain();
Wind();
Cloud();
Sun();
Visibility();
}




void domoticzoutconnect() {

  if (client.connect(domoticz_server, port)) {
    client.print("GET /json.htm?type=command&param=udevice&idx=");
  }
}


void domoticzoutauthenticate() {
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.print(domoticz_server);
  client.print(":");
  client.println(port);
  client.print("Authorization: Basic ");
  client.println(login);
  client.println("User-Agent: Arduino-ethernet");
  client.println("Connection: close");
  client.println();
  client.stop();
}



void sendvoltage() {

  domoticzoutconnect();
  client.print(577);
  client.print("&nvalue=0&svalue=");
  client.print(calvolts);
  domoticzoutauthenticate();
}



void sendspeed() {

  domoticzoutconnect();
  client.print(578);
  client.print("&nvalue=0&svalue=");
  client.print(speed);
  domoticzoutauthenticate();
}




void Rain() {

  //Serial.print("Rain1 ");
  JsonDocument doc;
  //Serial.print("Rain2 ");
  client.setTimeout(1000);
  http.setTimeout(1000);
  if (!client.connect(domoticz_server, (port))) {
    //Serial.println(F("DOMOTICZ_CONNECT failed"));
    return;
  }
  // Send HTTP request
  // //Serial.println("RAIN Get");
  client.println(F("GET /json.htm?type=command&param=getdevices&rid=447 HTTP/1.0"));
  client.print("Host: ");
  client.print(domoticz_server);
  client.print(":");
  client.println(port);
  client.print("Authorization: Basic ");
  client.println(login);
  client.println(F("Connection: close"));
  if (client.println() == 0) {
    //Serial.println(F("Failed to send request1"));
    // doc2.clear();
    client.stop();
    return;
  }

  char status[32] = { 0 };
  client.readBytesUntil('\r', status, sizeof(status));
  // It should be "HTTP/1.0 200 OK" or "HTTP/1.1 200 OK"
  if (strcmp(status + 9, "200 OK") != 0) {
    //Serial.print(F("Unexpected response1: "));
    //Serial.println(status);
    client.stop();
    return;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    //Serial.println(F("Invalid response1"));
    client.stop();
    return;
  }
  //client.setTimeout(1000);
  // Parse JSON object
  DeserializationError error = deserializeJson(doc, client);
  if (error) {
    //Serial.print(F("deserializeJson() failed: 1"));
    //Serial.println(error.f_str());
    client.stop();
    return;
    //Serial.println("DC Get");
  }
  RAINFALL = doc["result"][0]["Rain"];
  RAINRATE = doc["result"][0]["RainRate"];
  if (RAINFALL) {
    //Serial.print("RAINFALL ");
    //Serial.println(RAINFALL);
    rain = atof(RAINFALL);
    //Serial.print(rain);
  }
  if (RAINRATE) {
    //Serial.print("RAINRATE ");
    //Serial.println(RAINRATE);
    rainrate = atof(RAINRATE);
    //Serial.println(rainrate);
  }
  http.end();
}






void Visibility() {
  JsonDocument doc;
  // //Serial.println("----------------------------------------------------------------1");
  client.setTimeout(10000);
  if (!client.connect(domoticz_server, (port))) {
    //Serial.println(F("Connection failed4"));
    return;
  }
  //Serial.println("----------------------------------------------------------------2");
  // Send HTTP request
  client.println(F("GET /json.htm?type=command&param=getdevices&rid=448 HTTP/1.0"));
  client.print("Host: ");
  client.print(domoticz_server);
  client.print(":");
  client.println(port);
  client.print("Authorization: Basic ");
  client.println(login);
  client.println(F("Connection: close"));
  if (client.println() == 0) {
    //Serial.println(F("Failed to send request"));
    client.stop();
    return;
  }
  // //Serial.println("----------------------------------------------------------------3");
  // Check HTTP status
  char status[32] = { 0 };
  client.readBytesUntil('\r', status, sizeof(status));
  // It should be "HTTP/1.0 200 OK" or "HTTP/1.1 200 OK"
  if (strcmp(status + 9, "200 OK") != 0) {
    //Serial.print(F("Unexpected response: "));
    //Serial.println(status);
    client.stop();
    return;
  }
  //Serial.println("----------------------------------------------------------------4");
  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    //Serial.println(F("Invalid response"));
    client.stop();
    return;
  }
  // //Serial.println("----------------------------------------------------------------5");
  // Parse JSON object
  DeserializationError error = deserializeJson(doc, client);
  if (error) {
    //Serial.print(F("deserializeJson() failed: "));
    //Serial.println(error.f_str());
    client.stop();
    return;
  }

  // //Serial.println("----------------------------------------------------------------6");
  VISIBILITY = doc["result"][0]["Data"];
  //Serial.print("Visibility ");

  if (VISIBILITY) {
    //Serial.println(VISIBILITY);
    visi = atof(VISIBILITY);
    //Serial.println(visi);
    //Serial.print("Visibility OK");
  }
  http.end();
}



void Sun() {
  //Serial.print("SUN1 ");
  JsonDocument doc;
  client.setTimeout(10000);
  if (!client.connect(domoticz_server, (port))) {
    //Serial.println(F("Connection failed"));
    return;
  }
  // Send HTTP request
  client.println(F("GET /json.htm?type=command&param=getdevices&rid=451 HTTP/1.0"));
  client.print("Host: ");
  client.print(domoticz_server);
  client.print(":");
  client.println(port);
  client.print("Authorization: Basic ");
  client.println(login);
  client.println(F("Connection: close"));
  if (client.println() == 0) {
    //Serial.println(F("Failed to send request"));
    client.stop();
    return;
  }

  // Check HTTP status
  char status[32] = { 0 };
  client.readBytesUntil('\r', status, sizeof(status));
  // It should be "HTTP/1.0 200 OK" or "HTTP/1.1 200 OK"
  if (strcmp(status + 9, "200 OK") != 0) {
    //Serial.print(F("Unexpected response: "));
    //Serial.println(status);
    client.stop();
    return;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    //Serial.println(F("Invalid response"));
    client.stop();
    return;
  }

  // Parse JSON object
  DeserializationError error = deserializeJson(doc, client);
  if (error) {
    //Serial.print(F("deserializeJson() failed: "));
    //Serial.println(error.f_str());
    client.stop();
    return;
  }
  //Serial.println("SUN ");
  DAYLEGNTH = doc["DayLength"];
  SUNRISE = doc["Sunrise"];
  SUNSET = doc["Sunset"];
  SUNUP = doc["result"][0]["Data"];
  //Serial.print("DAYLEGNTH ");
  //Serial.println(DAYLEGNTH);
  //Serial.print("SUNRISE ");
  //Serial.println(SUNRISE);
  //Serial.print("SUNSET ");
  //Serial.println(SUNSET);
  DayLength = atof(DAYLEGNTH);
  //Serial.println(DayLength);
  http.end();
}


void Cloud() {
  //LIDAR
  JsonDocument doc;
  client.setTimeout(10000);
  if (!client.connect(domoticz_server, (port))) {
    //Serial.println(F("Connection failed"));
    return;
  }
  // Send HTTP request
  client.println(F("GET /json.htm?type=command&param=getdevices&rid=450 HTTP/1.0"));
  client.print("Host: ");
  client.print(domoticz_server);
  client.print(":");
  client.println(port);
  client.print("Authorization: Basic ");
  client.println(login);
  client.println(F("Connection: close"));
  if (client.println() == 0) {
    //Serial.println(F("Failed to send request"));
    client.stop();
    return;
  }

  // Check HTTP status
  char status[32] = { 0 };
  client.readBytesUntil('\r', status, sizeof(status));
  // It should be "HTTP/1.0 200 OK" or "HTTP/1.1 200 OK"
  if (strcmp(status + 9, "200 OK") != 0) {
    //Serial.print(F("Unexpected response: "));
    //Serial.println(status);
    client.stop();
    return;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    //Serial.println(F("Invalid response"));
    client.stop();
    return;
  }

  // Parse JSON object
  DeserializationError error = deserializeJson(doc, client);
  if (error) {
    //Serial.print(F("deserializeJson() failed: "));
    //Serial.println(error.f_str());
    client.stop();
    return;
  }
  CLOUD = doc["result"][0]["Data"];
  if (CLOUD) {
    //Serial.print("CLOUD ");
    //Serial.println(CLOUD);
    cloud = atof(CLOUD);
    //Serial.println(cloud);
  }
  http.end();
}









void Wind() {
  //LIDAR
  JsonDocument doc;
  client.setTimeout(10000);
  if (!client.connect(domoticz_server, (port))) {
    //Serial.println(F("Connection failed4"));
    return;
  }
  // Send HTTP request
  client.println(F("GET /json.htm?type=command&param=getdevices&rid=445 HTTP/1.0"));
  client.print("Host: ");
  client.print(domoticz_server);
  client.print(":");
  client.println(port);
  client.print("Authorization: Basic ");
  client.println(login);
  client.println(F("Connection: close"));
  if (client.println() == 0) {
    //Serial.println(F("Failed to send request"));
    client.stop();
    return;
  }

  // Check HTTP status
  char status[32] = { 0 };
  client.readBytesUntil('\r', status, sizeof(status));
  // It should be "HTTP/1.0 200 OK" or "HTTP/1.1 200 OK"
  if (strcmp(status + 9, "200 OK") != 0) {
    //Serial.print(F("Unexpected response: "));
    //Serial.println(status);
    client.stop();
    return;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    //Serial.println(F("Invalid response"));
    client.stop();
    return;
  }

  // Parse JSON object
  DeserializationError error = deserializeJson(doc, client);
  if (error) {
    //Serial.print(F("deserializeJson() failed: "));
    //Serial.println(error.f_str());
    client.stop();
    return;
  }

  //Serial.println(doc["result"][0]["Direction"].as<long>());
  WINDD = doc["result"][0]["Direction"];
  WINDS = doc["result"][0]["Speed"];
  WINDG = doc["result"][0]["Gust"];

  TEMP = doc["result"][0]["Temp"];
  CHILL = doc["result"][0]["Chill"];


  if (WINDD) {
    //Serial.println("WINDD ");
    //Serial.println(WINDD);
  }


  if (WINDS) {
    //Serial.println("WINDS ");
    //Serial.println(WINDS);
    winds = atof(WINDS);
    //Serial.println(winds);
  }

  if (WINDG) {
    //Serial.println("WINDG ");
    //Serial.println(WINDG);
    windg = atof(WINDG);
    //Serial.println(windg);
  }

  //Serial.println(TEMP);
  //Serial.println(CHILL);


  http.end();
}
