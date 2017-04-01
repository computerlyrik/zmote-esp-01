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

  webserver.on("/discover", HTTPMethod::HTTP_GET, []() {
    char buffer[HTTP_BODYSIZE];
    info_object(buffer, sizeof(buffer));
    webserver.send(200, "text/html", buffer );
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

  setup_ir();

}

void loop() {
  webserver.handleClient();
  handle_discoveryserver();   
} 


void handle_discoveryserver() {
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

void info_object(char* buf, size_t maxSize) {
  String buffer = "<-Model=zmote-esp-01><-Type=ZMT2><-Revision=1><-Make=zmote.io>";
  buffer += "<-Config-URL=";
  buffer += "http://";
  buffer += WiFi.localIP().toString();
  buffer += ">";
  buffer += "<-UUID=";
  buffer += uuid;
  buffer += ">";
  buffer.toCharArray(buf, maxSize);
}

void handleRequestCommand() {
  String body = webserver.arg("plain");
  String parameters = "";
  Serial.print("processing body request: ");
  Serial.println(body);

  int khz = 38;
  unsigned int irSignal[] = {9000, 4500, 560, 560, 560, 560, 560, 1690, 560, 560, 560, 560, 560, 560, 560, 560, 560, 560, 560, 1690, 560, 1690, 560, 560, 560, 1690, 560, 1690, 560, 1690, 560, 1690, 560, 1690, 560, 560, 560, 560, 560, 560, 560, 1690, 560, 560, 560, 560, 560, 560, 560, 560, 560, 1690, 560, 1690, 560, 1690, 560, 560, 560, 1690, 560, 1690, 560, 1690, 560, 1690, 560, 39416, 9000, 2210, 560}; //AnalysIR Batch Export (IRremote) - RAW
  
  irsend.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), khz); //Note the approach used to automatically calculate the size of the array.

  if (body.substring(0,6) == "sendir") {
    Serial.println("Processing sendir");
    String ircode = body.substring(13);  //sendir,1:1,0,%s"
    Serial.print("Ircode: ");
    Serial.println(ircode);


    unsigned int buffer[HTTP_BODYSIZE];
    char ircode_char[ircode.length()];
    ircode.toCharArray(ircode_char, ircode.length());
  
    char * pch;
    pch = strtok (ircode_char, ", ");
    uint8_t msg_size = 0;
    while (pch != NULL)
    {
      buffer[msg_size] = atoi(pch);
      msg_size++;
      pch = strtok (NULL, ", ");
      
    }
    //Serial.println(buffer);
    Serial.println(msg_size);
    irsend.sendRaw(buffer, msg_size, khz);
    webserver.send(200, "text/html", "completeir,1:1,0");
  }
  
  
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
