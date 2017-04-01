#ifndef _IR_H_
#define _IR_H_


#include <IRremoteESP8266.h>

struct zmote_command {
  String command;
  String data;
};

IRsend irsend(5);

#endif
