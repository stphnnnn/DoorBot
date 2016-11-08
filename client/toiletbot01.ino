#include <ESP8266WiFi.h>

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

const char* ssid = SSID;
const char* password = PASSWORD;

const int httpPort = 3000;
const char* host = URL;

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
  if (reading == LOW && previous == HIGH) {
    request("/open");
    delay(300);
    Serial.println("Ready to go into light sleep...");
    delay(1000);
    sleep();
  }
  if (reading == HIGH && previous == LOW) {
    request("/close");
    delay(300);
  }
  previous = reading;
}

void request(String url) 
{  
  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    delay(300);
    request(url);
    return;
  }
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
  
  Serial.println();
  Serial.println("closing connection");
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
