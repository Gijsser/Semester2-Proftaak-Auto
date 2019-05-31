#include <Arduino.h>
#include "Communication.h"
#ifndef _SENSORS_H
#define _SENSORS_H

#define STEERING_WHEEL (A0)

void get_steeringwheel_position(int * lastPostition);

#endif
