void setup_settings() {
  // Initialize file system.
  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount file system");
    return;
  }
}

bool saveWifiSettings(String *ssid, String *password)
{
  // Open config file for writing.
  File file = SPIFFS.open(WIFI_CONFIG_FILE, "w");
  if (!file)
  {
    Serial.println("Failed to open file for writing");
    return false;
  }
  file.println(*ssid);
  file.println(*password);
  file.close();
  
  return true;
}

bool loadWifiSettings(String *ssid, String *password)
{
  File file = SPIFFS.open(WIFI_CONFIG_FILE, "r");
  if (!file)
  {
    Serial.println("Failed to open config file");
    return false;
  }

  String content = file.readString();
  file.close();
  
  content.trim();

  int8_t pos = content.indexOf("\r\n");
  if (pos == -1)
  {
    Serial.println("No second line found, could not parse file");
    Serial.println(content);
    return false;
  }

  *ssid = content.substring(0, pos);
  *password = content.substring(pos + 2);

  ssid->trim();
  password->trim();


  return true;
}
