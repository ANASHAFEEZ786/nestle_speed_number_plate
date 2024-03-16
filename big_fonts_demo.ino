#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "Font_Data.h"

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 16
#define MAX_ZONES 2
#define ZONE_SIZE (MAX_DEVICES/MAX_ZONES)
#define CLK_PIN 18
#define DATA_PIN 23
#define CS_PIN 15

MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

const char* ssid = "itecknologi";
const char* password = "Iteck#@#7831";
const char* url = "http://192.168.21.14:8080/parola.php"; // Change this to your URL

void setup() {
  Serial.begin(9600);
  delay(2000);
  P.begin(MAX_ZONES);
  P.setInvert(false);
  P.setIntensity(0);
  
  P.setZone(0, 0, ZONE_SIZE - 1);
  P.setZone(1, ZONE_SIZE, MAX_DEVICES - 1);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // Check if WiFi is connected
  if (WiFi.status() == WL_CONNECTED) {
    // Create HTTP client object
    HTTPClient http;
    // Make GET request to the URL
    http.begin(url);
    int httpCode = http.GET();
    
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString(); // Get the response payload
      Serial.println(payload);
      // Display the fetched string on the MAX7219 display
      P.setFont(0, BigFontBottom);
      P.setFont(1, BigFontUp);
      P.displayZoneText(0, payload.c_str(), PA_CENTER, 30, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
      P.displayZoneText(1, payload.c_str(), PA_CENTER, 30, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
      P.synchZoneStart();
      while (!P.getZoneStatus(0) || !P.getZoneStatus(1))
        P.displayAnimate(); // Animate until both zones finish displaying
    } else {
      Serial.printf("HTTP GET request failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
  delay(1000); // Delay before making the next request
}
