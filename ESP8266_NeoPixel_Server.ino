#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <Adafruit_NeoPixel.h>

#include "wifipass.h"

int LED_PIN = 5;
int NUMPIXELS = 24;
String HOSTNAME = "chroma";

ESP8266WebServer server(80);

Adafruit_NeoPixel *mPixels;

String mColorString = "000000";
uint8_t mBrightness = 250;

void handleNotFound() {
  String message = "404";
  server.send(404, "text/plain", message);
}

void handleRoot() {
  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i).indexOf("color") != -1) {
      mColorString = server.arg(i);
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

  uint32_t mColor = strtol(mColorString.c_str(), NULL, 16);

  for (int i = 0; i < NUMPIXELS; i++) {
    mPixels->setPixelColor(i, mColor);
  }
  mPixels->setBrightness(mBrightness);

  mPixels->show();
  delay(10);
  delete mPixels;
}

void setup(void) {
  Serial.begin(115200);

  IPAddress ip(192, 168, 0, 200);
  IPAddress gateway(192, 168, 0, 1);
  IPAddress subnet(255, 255, 255, 0);

  WiFi.config(ip, gateway, subnet);
  WiFi.hostname("chroma");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI, PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started\n");

  setupAndStartOTA();
  setPixels();
}

void loop(void) {
  server.handleClient();
  ArduinoOTA.handle();
}

void setupAndStartOTA() {
  ArduinoOTA.setHostname(HOSTNAME.c_str());
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("OTA Error[%u]: ", error);
  });
  ArduinoOTA.begin();
}

