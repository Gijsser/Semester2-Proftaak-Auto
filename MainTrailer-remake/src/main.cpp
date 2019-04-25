#include <Arduino.h>

#include "Trailer.h"

void setup() {
  Serial.begin(9600);
}

void select_mode(){
  switch (trailer_get_state()) {
    case 0:
    break;
    case 1:
    break;
    case 2:
    break;
  }
}


void loop() {
  select_mode();
  trailer_check_message();
}
