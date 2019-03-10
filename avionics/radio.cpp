/* @file    radio.cpp
 * @author  UBC Rocket Avionics 2018/2019
 * @description  Implements radio send functions and prioritizing
 * sensor data to send over the radio
 *
 * @section LICENSE
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * Distributed as-is; no warranty is given.
 */

/*Includes------------------------------------------------------------*/
#include "radio.h"

#include "sensors.h"
#include "battery.h"
#include "SparkFun_LIS331.h"        //accelerometer
#include "MS5803_01.h"              //barometer
#include "SparkFunTMP102.h"         //temp sensor
#include "Adafruit_BNO055.h"        //IMU
#include "Venus638FLPx.h"           //GPS

#include <Arduino.h>
#include <HardwareSerial.h>

//send pressure, gps, state, and altitude over radio
//FILLER FUNCTIONS
void  sendTierOne(unsigned long *timestamp, float* GPS_data, float bar_data[], FlightStates state, float altitude){
    float time = *timestamp;
    sendRadioData(time, 't');
    sendRadioData(bar_data[0], UID_bar_pres);
    sendRadioData(GPS_data[0], UID_GPS_lat);
    sendRadioData(GPS_data[1], UID_GPS_long);
    sendRadioData(GPS_data[2], UID_GPS_alt);
    sendRadioData( altitude, UID_altitude);
    sendRadioData((float) state, UID_state);
}

//Send Acceleration, IMU data, and temperature
//FILLER FUNCTION
void sendTierTwo(float acc_data[], float bar_data[], float *temp_sensor_data, float IMU_data[]){
    sendRadioData(bar_data[1], UID_bar_temp);
    sendRadioData(*temp_sensor_data, UID_temp_temp);
    sendRadioData(IMU_data[0], UID_IMU_yaw);
    sendRadioData(IMU_data[1], UID_IMU_roll);
    sendRadioData(IMU_data[2], UID_IMU_pitch);
    sendRadioData(acc_data[0], UID_acc_acc_x);
    sendRadioData(acc_data[1], UID_acc_acc_y);
    sendRadioData(acc_data[2], UID_acc_acc_z);
}
//Send data that only needs to be sent once, battery voltage, Ground Altitude
//FILLER FUNCTION
void sendTierThree(float* battery_voltage, float* ground_altitude){
    sendRadioData(*ground_altitude, UID_ground_altitude);
    sendRadioData(*battery_voltage, UID_batt);
}
/**
  * @brief  Send more essential data from the body over radio, pressure, state, altitude, timestamp
  * @param  float bar_data - baramoeter data array
  * @param  FlightStates state - current state
  * @param  float altitude - current calculated altitude
  * @param  unsigned long *timestamp - address for the timestamp
  * @return void
  */
void bodyTierOne(float bar_data[], FlightStates state, float altitude, unsigned long *timestamp){
    float time = *timestamp;
    sendRadioData(time, 't');
    sendRadioData(bar_data[0], UID_bar_pres);
    sendRadioData( altitude, UID_altitude);
    sendRadioData((float) state, UID_state);
}
/**
  * @brief  Sends more essential data over radio from the nosecone, timestamp(WHICH SHOULD BE REMOVED) and GPS data
  * @param  float GPS_data - gps data array
  * @param  unsigned long *timestamp - address for the timestamp
  * @return void
  */
void noseconeTierOne(float* GPS_data){
    sendRadioData(GPS_data[0], UID_GPS_lat);
    sendRadioData(GPS_data[1], UID_GPS_long);
    sendRadioData(GPS_data[2], UID_GPS_alt);
}
/**
  * @brief  Function to send the less essential data from the nosecone over radio,
  * barameter temperature, accelerometer, tmeperature, and IMU data
  * @param  float bar_data - barmoeter data array for temperature
  * @param  float acc_data - accelerometer data array
  * @param  float *temp_sensor_data - pointer for the temperature data
  * @param  float IMU_data - IMU data array
  * @return void
  */
void noseconeTierTwo(float bar_data[], float acc_data[], float *temp_sensor_data, float IMU_data[]){
    sendRadioData(bar_data[1], UID_bar_temp);
    sendRadioData(*temp_sensor_data, UID_temp_temp);
    sendRadioData(IMU_data[0], UID_IMU_yaw);
    sendRadioData(IMU_data[1], UID_IMU_roll);
    sendRadioData(IMU_data[2], UID_IMU_pitch);
    sendRadioData(acc_data[0], UID_acc_acc_x);
    sendRadioData(acc_data[1], UID_acc_acc_y);
    sendRadioData(acc_data[2], UID_acc_acc_z);
}

/**
  * @brief  Takes a 4 byte float and sends each byte sequentially over the radio
  * @param  float data - one float worth of data to be sent via radio
  * @param  char id - data identifier
  * @return void
  */
void sendRadioData(float data, char id){
    //teensy should be little endian, which means least significant is stored first, make sure ground station decodes accordingly
     u_int8_t b[4];
      *(float*) b = data;
      SerialRadio.write(id);
      for(int i=0; i<4; i++)
      {
          SerialRadio.write(b[i]);
      }
}