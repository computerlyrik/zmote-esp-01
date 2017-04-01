#include "uuid.h"


void get_uuid(char* uuid) {


  uint8_t MAC_array[6];
  WiFi.macAddress(MAC_array);
  Serial.print("Mac Address values: ");
  for (int i = 0; i < sizeof(MAC_array); ++i){
    Serial.print(MAC_array[i]);
    sprintf(uuid,"%s%02x",uuid,MAC_array[i]);
  }
  Serial.println();
}


void get_uuid_via_eeprom(char* uuid) {
  Serial.println("Testing UUID Startbyte from EEPROM");

  byte startbyte = EEPROM.read(0);
  if (startbyte == 0xFF || startbyte == 0x00) {
    Serial.println("Found ");
    Serial.print(startbyte);
    Serial.println(" - generating UUID");
    set_uuid_by_mac();
  }
  
  for (int i = 0; i<UUID_SIZE; i++) {
    sprintf(uuid,"%s%02x",uuid,EEPROM.read(i));
  }
  Serial.print("Read UUID from EEPROM: ");
  Serial.println(uuid);

}


void set_uuid_by_mac() {

    uint8_t MAC_array[6];
    WiFi.macAddress(MAC_array);
    Serial.print("Writing to eeprom: ");
    
    for (int i = 0; i < sizeof(MAC_array); ++i){
      Serial.print(MAC_array[i]);
      EEPROM.write(i, MAC_array[i]);
    }
    Serial.println();
}




