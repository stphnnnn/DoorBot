#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

extern "C" {
#include "gpio.h"
}
extern "C" {
#include "user_interface.h"
}

String SLACK_SSL_FINGERPRINT = "AB F0 5B A9 1A E0 AE 5F CE 32 2E 7C 66 67 49 EC DD 6D 6A 38";
String SLACK_BOT_TOKEN = "your-bot-token-here";

const char* ssid = "your-ssid-password-here";
const char* password = "your-wifi-password-here";

const int buttonPin = 12; 
const int ledPin = 5;  
int reading = HIGH;
int previous = HIGH;

void setup() 
{
  gpio_init();
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(115200);
  delay(100);
   
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  delay(1000);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
 
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() 
{
  reading = digitalRead(buttonPin);
  digitalWrite(ledPin, reading);
  if (reading == LOW && previous == HIGH) {
    request("auto");
    delay(300);
    Serial.println("Ready to go into light sleep...");
    delay(1000);
    sleep();
  }
  if (reading == HIGH && previous == LOW) {
    request("away");
    delay(300);
  }
  previous = reading;
}

void request(String url) {
  HTTPClient http;
  Serial.println("https://slack.com/api/users.setPresence?token=" + SLACK_BOT_TOKEN + "&presence=" + url);
  http.begin("https://slack.com/api/users.setPresence?token=" + SLACK_BOT_TOKEN + "&presence=" + url, SLACK_SSL_FINGERPRINT);
  int httpCode = http.GET();

  if (httpCode != HTTP_CODE_OK) {
    Serial.printf("HTTP GET failed with code %d\n", httpCode);
    delay(300);
    request(url);
    return;
  }
  else {
    Serial.printf("HTTP GET success with code %d\n", httpCode);
  } 
}

void wake(void) 
{
  wifi_fpm_close;
  wifi_set_opmode(STATION_MODE);
  wifi_station_connect();
  Serial.println("Woke up from sleep");
  reading == HIGH; 
}


void sleep() 
{
  Serial.println("Going to light sleep...");
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
