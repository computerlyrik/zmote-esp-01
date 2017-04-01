#include "uuid.h"


void get_uuid(char* uuid) {
  Serial.println("Testing UUID Startbyte from EEPROM");

  byte startbyte = EEPROM.read(0);
  if (startbyte == 0xFF) {
    Serial.println("Found 0xFF - generating UUID");
    set_uuid_by_mac();
  }
  
  for (int i = 0; i<UUID_SIZE; i++) {
    sprintf(uuid,"%s%02x:",uuid,EEPROM.read(i));
  }
  Serial.print("Read UUID from EEPROM: ");
  Serial.println(uuid);

}


void set_uuid_by_mac() {

    uint8_t MAC_array[6];
    WiFi.macAddress(MAC_array);
    
    for (int i = 0; i < sizeof(MAC_array); ++i){
      Serial.print("Writing to eeprom: ");
      EEPROM.write(i, MAC_array[i]);
    }
}




