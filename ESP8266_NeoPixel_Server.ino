#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Adafruit_NeoPixel.h>

#include "wifipass.h"

#define LED_PIN 5
#define NUMPIXELS 24

const char* WiFiAPPSK = "cromaterapia";
IPAddress WiFiIP(10, 10, 81, 1);
IPAddress WiFiSubnet(255, 255, 255, 0);

ESP8266WebServer server(80);

Adafruit_NeoPixel *mPixels;

uint32_t mColor = Adafruit_NeoPixel::Color(0, 0, 0);
String mColorHtml = "#000000";
uint8_t mBrightness = 250;

void handleRoot() {
  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i).indexOf("color") != -1) {
      mColor = strtol(server.arg(i).c_str(), NULL, 16);
      mColorHtml = "#" + server.arg(i);
    }
    else if (server.argName(i).indexOf("brightness") != -1) {
      mBrightness = server.arg(i).toInt();
    }
  }
  server.send(200, "text/html", createHtml());
  setPixels();
}

void setPixels() {

  mPixels = new Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
  mPixels->begin();

  for (int i = 0; i < NUMPIXELS; i++) {
    mPixels->setPixelColor(i, mColor);
  }
  mPixels->setBrightness(mBrightness);

  mPixels->show();
  delay(10);
  delete mPixels;
}

void handleNotFound() {
  String message = "404";
  server.send(404, "text/plain", message);
}

void setup(void) {
  Serial.begin(115200);
  WiFi.softAPConfig(WiFiIP, WiFiIP, WiFiSubnet);
  WiFi.softAP(WiFiIP.toString().c_str(), WiFiAPPSK);

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started\n");

  setPixels();
}

void loop(void) {
  server.handleClient();
}

