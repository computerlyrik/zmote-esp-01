#include "networking.h"
#include "ir.h"
#include "uuid.h"

String local_ip;
String mac;

char uuid[12];

void setup() 
{
    
  // start serial
  Serial.begin(9600);
  delay(100);

  get_uuid(uuid);
  Serial.print("Setup got uuid: ");
  Serial.println(uuid);

  webserver.on("/", []() {
     webserver.send(200, "text/html", "<html><head> <title>zmote-esp-01</title></head><body><h1>ZMote API v2 Endpoint</h1></body></html>");
  });
  
  webserver.on("/uuid", HTTPMethod::HTTP_GET, []() {
    String message = "uuid,";
    message += uuid;
    webserver.send(200, "text/html", message );
  });
  
  char command_uri[17] = "/v2/";
  strcat(command_uri,uuid);
  Serial.print("Listening on command uri: ");
  Serial.println(command_uri);
  webserver.on(command_uri, HTTPMethod::HTTP_POST, handleRequestCommand);

  webserver.onNotFound(handleNotFound);
  
  setup_networking(); 

  //mac = WiFi.macAddress();
  //local_ip = WiFi.localIP().toString();

  //setup_ir();

}

void loop() {
  webserver.handleClient();
  //check_discoveryserver();
  //check_webserver();
   
} 

// ################ WEB SERVER PARSER

void check_webserver() {
}

void check_discoveryserver() {
  int packetSize = udp.parsePacket();
  if (packetSize)
  {
    Serial.printf("Discovery Received %d bytes from %s, port %d\n", packetSize, udp.remoteIP().toString().c_str(), udp.remotePort());
    int len = udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);
    if (incomingPacket[0] == 0xbe && incomingPacket[1] == 0xef)
    {
      Serial.print("Answering to discoverer ip ");
      Serial.print(udp.remoteIP());
      Serial.print(" port ");
      Serial.println(udp.remotePort());
      udp.beginPacket(udp.remoteIP(), udp.remotePort());
      char buffer[HTTP_BODYSIZE];
      info_object(buffer, sizeof(buffer));
      Serial.print("Discovery Answer: ");
      Serial.println(buffer);
      udp.write(buffer);
      udp.endPacket();
    }


  }
}




void info_object(char* json, size_t maxSize) {
  StaticJsonBuffer<HTTP_BODYSIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["name"] = "LedPipe";
  root["id"] = mac;
  root["ip"] = local_ip;
  

  root.printTo(json, maxSize);
  
}




void process_settings(const char* body, WiFiClient client) {
  StaticJsonBuffer<HTTP_BODYSIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(body);

  set_settings(root);

}



int BRIGHTNESS = 0;
void set_settings(JsonObject& root) {
  Serial.println("-- Set Settings");
  

  if (root.containsKey("dim")) {
    BRIGHTNESS = root["dim"];
    Serial.print(" Brightness/Dim: ");
    Serial.println(BRIGHTNESS); 
  }

}

void handleRequestCommand() {
  String body = webserver.arg("plain");
  String parameters = "";
  Serial.print("processing body request: ");
  Serial.print(body);
  webserver.send(200, "text/html", body);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += webserver.uri();
  message += "\nMethod: ";
  message += (webserver.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += webserver.args();
  message += "\n";
  for (uint8_t i=0; i<webserver.args(); i++){
    message += " " + webserver.argName(i) + ": " + webserver.arg(i) + "\n";
  }
  webserver.send(404, "text/plain", message);
}
