#ifndef _NETWORKING_H_
#define _NETWORKING_H_

#include "settings.h"

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

#include <WiFiUdp.h>
#include <ArduinoJson.h>

//#include <ArduinoOTA.h>

#define HTTP_BODYSIZE  512


IPAddress discover_mcast_ip(239, 255, 250, 250);
#define UDP_QUERY_PORT 9130
#define UDP_RESPONSE_PORT 9131

#define WEB_PORT 80

//const char *ssid;
//const char *password;

String ssid;
String password;

const char* s = "foobar";
const char* p = AP_PSK;

WiFiUDP udp;
MDNSResponder mdns;
ESP8266WebServer webserver(WEB_PORT);
ESP8266HTTPUpdateServer httpUpdater;


char incomingPacket[HTTP_BODYSIZE];

void setup_networking();
void handleNotFound();

#endif //_NETWORKING_H_
