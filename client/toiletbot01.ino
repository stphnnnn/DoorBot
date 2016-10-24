#define BUTTON 5
#include <ESP8266WiFi.h>

int reading;
int previous = LOW;
int value;
const char* ssid     = "TwentyNine";
const char* password = "TryListening";
const char* host = "192.168.1.102";

void setup() {
  pinMode(BUTTON, INPUT_PULLUP);
  Serial.begin(115200);
  delay(10);  
  WiFi.begin(ssid, password); 
}

void loop() {
  reading = digitalRead(BUTTON);
  if (reading == LOW && previous == HIGH) {
    Serial.println("engaged");
    req();
    delay(300);
  }
  if (reading == HIGH && previous == LOW) {
    Serial.println("vacant");
    req();
    delay(300);
  }
  previous = reading;
}

void req() {
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 3000;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "192.168.1.102";
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
}


