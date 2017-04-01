#include "networking.h"

void setup_networking() 
{
  
  // inital connect
  WiFi.mode(WIFI_STA);
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  webserver.begin();
  Serial.println("Server started");

  udp.begin(UDP_PORT);
  Serial.println("Discovery server started");


  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  
  Serial.println(WiFi.localIP());


}
