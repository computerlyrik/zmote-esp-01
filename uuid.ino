#include "uuid.h"


void get_uuid(char* uuid) {


  uint8_t MAC_array[6];
  WiFi.macAddress(MAC_array);
  //Serial.print("Mac Address values: ");
  for (int i = 0; i < sizeof(MAC_array); ++i){
    //Serial.print(MAC_array[i]);
    sprintf(uuid,"%s%02x",uuid,MAC_array[i]);
  }
  //Serial.println();
}



