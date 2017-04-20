#include "networking.h"
#include "ir.h"
#include "uuid.h"

char uuid[48]; //8+1+(6*2+5)];

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

  char command_uri[65] = "/v2/";
  strcat(command_uri,uuid);
  Serial.print("Listening on command uri: ");
  Serial.println(command_uri);
  webserver.on(command_uri, HTTPMethod::HTTP_POST, handleRequestCommand);
  
  setup_networking(); 

  setup_ir();

}

void loop() {
  webserver.handleClient();
  discovery_server();   
} 


void discovery_server() {
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
    String pack = incomingPacket;
    Serial.printf("UDP Server received: %s\n", pack.c_str());
    if (pack.length() <= 8 && pack.substring(0,8).equalsIgnoreCase("SENDAMXB"))
    {
      
      Serial.printf("Answering to discoverer ip %s port %s\n", udp.remoteIP().toString().c_str(), udp.remotePort());

      udp.beginPacketMulticast(discover_mcast_ip, UDP_RESPONSE_PORT, WiFi.localIP());

//      udp.beginPacket(udp.remoteIP(), udp.remotePort());
  
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
  String buffer = "AMXB<-Model=zmote-esp-01><-Type=ZMT2><-Revision=1><-Make=zmote.io>";
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
  //Serial.print("processing body request: ");
  //Serial.println(body);

  int khz;
  
  if (body.substring(0,6) == "sendir") {
    //Serial.println("Processing sendir");
    String ircode = body.substring(13);  //sendir,1:1,0,%s"
    //Serial.print("Ircode: ");
    //Serial.println(ircode);


    unsigned int buffer[HTTP_BODYSIZE];
    char ircode_char[ircode.length()+1];
    ircode.toCharArray(ircode_char, ircode.length()+1);
  
    char * pch;
    pch = strtok(ircode_char, " ,");
    uint8_t msg_size = 0;
    uint8_t pos = 0;
    uint8_t times = 0;
    while (pch != NULL)
    {
      if (pos == 0) {
        khz = atoi(pch) / 1000;
      }
      else if (pos == 1) {
        times = atoi(pch);
      }
      else if (pos == 2) 
      {
        //garbage
      }
      else {
        buffer[msg_size] = atoi(pch);
        msg_size++;
      }

      pch = strtok(NULL, " ,");
      pos++;      
    }
    
    for (int i = 0; i < msg_size; i++) {
      Serial.print(buffer[i]);
      Serial.print(", ");
    }
    Serial.println();
    Serial.println(msg_size);
    
    for (int t = 0; t < times; t++) {
      irsend.sendRaw(buffer, msg_size, khz);
      delay(200);
    }
    webserver.send(200, "text/html", "completeir,1:1,0");

  }
  
  
  webserver.send(200, "text/html", body);
}
