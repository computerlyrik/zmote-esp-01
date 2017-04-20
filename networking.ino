#include "networking.h"

void setup_networking() 
{
  setup_settings();


  String hostString(DEVICE_PREFIX);
  hostString += "-";
  hostString += String(ESP.getChipId(), HEX);
  WiFi.hostname(hostString);
  Serial.println("Hostname set to "+hostString);

  if (!loadWifiSettings(&ssid, &password)) {
    startAP();
  } 
  else {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.print("...");

    WiFi.begin(ssid.c_str(), password.c_str());

    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("Connection Failed! Starting AP");
      WiFi.disconnect();
      startAP();
    } else { 
      Serial.println(WiFi.localIP());
    }
  }
  Serial.println("WiFi connected");

/* MDNS Service */

  if (!MDNS.begin((const char *)hostString.c_str())) {
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.println("mDNS responder started");
  MDNS.addService(hostString, "tcp", WEB_PORT);
  //if (mdns.begin(hostString, WiFi.localIP())) {
  //  Serial.println("MDNS responder started");
  //}
  


/* Discovery Service */
  if (!udp.beginMulticast(WiFi.localIP(), discover_mcast_ip, UDP_QUERY_PORT)) {
    Serial.println("Starting of discovery server failed"); 
  } else {
    Serial.println("Discovery server started");
  }

/* OTA Serivice */
  //ArduinoOTA.setHostname((const char *)hostString.c_str());
  //ArduinoOTA.begin();


/* HTTP OTA Service */
 httpUpdater.setup(&webserver, UPDATE_PATH, UPDATE_USER, UPDATE_PASSWORD);
  
/* Webserver */

  webserver.onNotFound(handleNotFound);

  webserver.begin();
  Serial.println("Webserver started");

  
  delay(100);


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
  Serial.println(message);
  webserver.send(404, "text/plain", message);
}

void startAP() {

  ssid = AP_SSID_PREFIX;
  ssid += String(ESP.getChipId(), HEX);
  
  s = ssid.c_str(); 

  Serial.print("Configuring access point...");
  WiFi.softAP(s,p);

}


