


void ISS() {
  if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;
    http.begin(apiURL);
    int httpResponseCode = http.GET();

    if (httpResponseCode == 200) {
      String payload = http.getString();
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      float issLatitude = doc["iss_position"]["latitude"];
      float issLongitude = doc["iss_position"]["longitude"];

      Serial.print("ISS Latitude: ");
      Serial.println(issLatitude);
      Serial.print("ISS Longitude: ");
      Serial.println(issLongitude);

      distanceMiles = calculateDistance(myLatitude, myLongitude, issLatitude, issLongitude);
      Serial.print("Distance to ISS: ");
      Serial.println(distanceMiles);

      if (distanceMiles <= alertRadiusMiles) {
        Serial.println("Alert! ISS is within 500 miles!");
        // Add buzzer or LED notification code here
      }
    } else {
      Serial.print("Error getting ISS data: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
}

float calculateDistance(float lat1, float lon1, float lat2, float lon2) {
  const float earthRadiusMiles = 3958.8;
  float dLat = radians(lat2 - lat1);
  float dLon = radians(lon2 - lon1);
  float a = sin(dLat / 2) * sin(dLat / 2) + cos(radians(lat1)) * cos(radians(lat2)) * sin(dLon / 2) * sin(dLon / 2);
  float c = 2 * atan2(sqrt(a), sqrt(1 - a));
  return earthRadiusMiles * c;
}






void fetchTideData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(apitideURL);
    int httpCode = http.GET();

    if (httpCode > 0) {  // Check for successful response
      String payload = http.getString();
      // Serial.println("Response: ");
      //Serial.println(payload);

      // Parse JSON response
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);
      JsonObject obj = doc.as<JsonObject>();


      tide = doc["items"]["measures"]["latestReading"]["value"];
      Serial.print("tide=================================: ");
      Serial.println(tide);
      // Example: Print tide station names
    }
  } else {
    Serial.println("Error fetching data!");
  }
  http.end();
}
