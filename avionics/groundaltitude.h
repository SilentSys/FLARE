/*Ground Altitude Calculation Header*/
#ifndef GROUNDALTITUDE_H
#define GROUNDALTITUDE_H

/*Includes------------------------------------------------------------*/
#include <stdint.h>

/*Constants------------------------------------------------------------*/
#define GROUND_ALT_SIZE 60  //array size for moving average

/*Functions------------------------------------------------------------*/
float groundAlt_arr_sum(float*);
float groundAlt_update(float*, float*);

#endif
