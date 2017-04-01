#ifndef _NETWORKING_H_
#define _NETWORKING_H_

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#include <WiFiUdp.h>
#include <ArduinoJson.h>


#define HTTP_BODYSIZE  512

const char* ssid = "**********";
const char* password = "********";

#define WEB_PORT 80
#define UDP_PORT 4333

ESP8266WebServer webserver(WEB_PORT);
WiFiUDP udp;
MDNSResponder mdns;


char incomingPacket[HTTP_BODYSIZE];


void setup_networking();




#endif //_NETWORKING_H_
