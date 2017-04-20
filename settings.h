#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#define DEBUG


#include <FS.h>

#define VERSION "0.0.1"
#define DEVICE_PREFIX "ZMote_ESP"

#define AP_SSID_PREFIX "ZMote_ESP_"
#define AP_PSK "ZMote_ESP"
#define WIFI_CONFIG_FILE "/wifi_settings"

#define UPDATE_USER "zmote"
#define UPDATE_PASSWORD "zmote"
#define UPDATE_PATH "/update"

void setup_settings();
bool saveWifiSettings(String *ssid, String *password);
bool loadWifiSettings(String *ssid, String *password);


#endif
