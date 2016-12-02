#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "config.h"

extern "C" {
#include "gpio.h"
}
extern "C" {
#include "user_interface.h"
}

const int buttonPin = 12;
const int ledPin = 5;
int reading = HIGH;
int previous = HIGH;

HTTPClient http;

void setup() {
  initHardware();
  delay(100);
  connectWiFi();
}

void loop() {
  reading = digitalRead(buttonPin);
  if (reading == HIGH && previous == LOW) {
    request("away");
  }
  if (reading == LOW && previous == HIGH) {
    request("auto");
    delay(300);
    sleep();
  }
  previous = reading;
}

void request(String url) {
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    digitalWrite(ledPin, !digitalRead(ledPin));
    Serial.print(".");
  }
  digitalWrite(ledPin, LOW);
  delay(300);
  http.begin("https://slack.com/api/users.setPresence?token=" + SLACK_BOT_TOKEN + "&presence=" + url, SLACK_SSL_FINGERPRINT);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    Serial.println("Success!");
    return;
  }
  else {
    Serial.printf("[HTTP] GET failed, error: %s\n", http.errorToString(httpCode).c_str());
    request(url);
    return;
  }
}

void wake(void) {
  wifi_fpm_close;
  wifi_set_opmode(STATION_MODE);
  wifi_station_connect();
  Serial.println("Waking");
  previous = LOW;
}

void sleep() {
  digitalWrite(ledPin, LOW);
  Serial.println("Sleeping");
  wifi_station_disconnect();
  wifi_set_opmode(NULL_MODE);
  wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
  gpio_pin_wakeup_enable(GPIO_ID_PIN(12), GPIO_PIN_INTR_HILEVEL);
  wifi_fpm_open();
  delay(100);
  wifi_fpm_set_wakeup_cb(wake);
  wifi_fpm_do_sleep(0xFFFFFFF);
  delay(100);
}

void connectWiFi() {
  Serial.printf("Connecting to %s\n", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.config(staticIP, gateway, subnet);
  delay(1000);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(ledPin, !digitalRead(ledPin));
  }
  digitalWrite(ledPin, LOW);
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void initHardware() {
  gpio_init();
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(115200);
  Serial.println();
}
