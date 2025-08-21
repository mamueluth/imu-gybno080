
#ifndef _IMU_DATA_SERIALIZATION_H_
#define _IMU_DATA_SERIALIZATION_H_

#include <stdint.h>
#include <stdio.h>

#include "HardwareSerial.h"
#include "ICM_20948.h"

class IMUDataSerialization
{
public:
  IMUDataSerialization(
    const float accX, const float accY, const float accZ, const float gyrX, const float gyrY,
    const float gyrZ, const float magX, const float magY, const float magZ, const float temp)
  : accX_(accX),
    accY_(accY),
    accZ_(accZ),
    gyrX_(gyrX),
    gyrY_(gyrY),
    gyrZ_(gyrZ),
    magX_(magX),
    magY_(magY),
    magZ_(magZ),
    temp_(temp)
  {
  }

  IMUDataSerialization(ICM_20948 & agmt)
  : accX_(agmt.accX()),
    accY_(agmt.accY()),
    accZ_(agmt.accZ()),
    gyrX_(agmt.gyrX()),
    gyrY_(agmt.gyrY()),
    gyrZ_(agmt.gyrZ()),
    magX_(agmt.magX()),
    magY_(agmt.magY()),
    magZ_(agmt.magZ()),
    temp_(agmt.temp())
  {}


  // Serialize to JSON into a caller-provided buffer
  // buffer: char array to store JSON
  // size: size of the buffer
  // Returns: true if serialization succeeded, false if buffer too small
  bool serialize_to_json(char* buffer, size_t size) const {
      int n = snprintf(buffer, size,
          JSON_FORMAT,
          accX_, accY_, accZ_,
          gyrX_, gyrY_, gyrZ_,
          magX_, magY_, magZ_,
          temp_);

      return n > 0 && static_cast<size_t>(n) < size; // true if no truncation
  }

  // Calculate the size needed for the JSON string
  size_t json_buffer_size() const {
      // we add 1 for the null termination character
      return static_cast<size_t> (snprintf(nullptr, 0,
          JSON_FORMAT,
          accX_, accY_, accZ_,
          gyrX_, gyrY_, gyrZ_,
          magX_, magY_, magZ_,
          temp_) + 1);
  }

protected:
  float accX_, accY_, accZ_;  // Accelerometer 
  float gyrX_, gyrY_, gyrZ_;  // Gyroscope data 
  float magX_, magY_, magZ_;  // Magnetometer data 
  float temp_;                // Temperature 

  const char* JSON_FORMAT =
    "{\"accX\":%.4f,\"accY\":%.4f,\"accZ\":%.4f,"
    "\"gyrX\":%.4f,\"gyrY\":%.4f,\"gyrZ\":%.4f,"
    "\"magX\":%.4f,\"magY\":%.4f,\"magZ\":%.4f,"
    "\"temp\":%.4f}";
};

#endif  // _IMU_DATA_SERIALIZATION_H_