#ifndef _NETWORKING_H_
#define _NETWORKING_H_

#include "settings.h"

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#include <WiFiUdp.h>
#include <ArduinoJson.h>


#define HTTP_BODYSIZE  512


ESP8266WebServer webserver(WEB_PORT);

IPAddress discover_mcast_ip(239, 255, 250, 250);
unsigned int discover_mcast_port = 9130; 
#define UDP_PORT 9130

WiFiUDP udp;
MDNSResponder mdns;

char incomingPacket[HTTP_BODYSIZE];

void setup_networking();


#endif //_NETWORKING_H_
