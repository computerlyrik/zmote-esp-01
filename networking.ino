#include "networking.h"

void setup_networking() 
{
  
  // inital connect
  WiFi.mode(WIFI_STA);
  // Connect to WiFi network
  
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

  //udp.begin(UDP_PORT);
  if (!udp.beginMulticast(WiFi.localIP(), discover_mcast_ip, discover_mcast_query_port)) {
    Serial.println("Starting of discovery server failed"); 
  } else {
    Serial.println("Discovery server started");
  }

  if (mdns.begin(MDNS_NAME, WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  
  Serial.println(WiFi.localIP());

  delay(100);


}
