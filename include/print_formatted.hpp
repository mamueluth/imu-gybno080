#ifndef _PRINT_FORMATTED_H_
#define _PRINT_FORMATTED_H_

#include "HardwareSerial.h"

void printFormattedFloat(HardwareSerial& Serial, float val, uint8_t decimals, uint8_t leading)
{
  float aval = abs(val);
  if (val < 0)
  {
    Serial.print("-");
  }
  else
  {
    Serial.print(" ");
  }
  for (uint8_t indi = 0; indi < leading; indi++)
  {
    uint32_t tenpow = 0;
    if (indi < (leading - 1))
    {
      tenpow = 1;
    }
    for (uint8_t c = 0; c < (leading - 1 - indi); c++)
    {
      tenpow *= 10;
    }
    if (aval < tenpow)
    {
      Serial.print("0");
    }
    else
    {
      break;
    }
  }
  if (val < 0)
  {
    Serial.print(-val, decimals);
  }
  else
  {
    Serial.print(val, decimals);
  }
}



template <typename CommunicationProtocol> void printScaledAGMT(HardwareSerial& Serial, CommunicationProtocol *sensor, const uint8_t float_precision = 4, const uint8_t float_alignment = 5)
{
  Serial.print("Scaled. Acc (mg) [ ");
  printFormattedFloat(Serial, sensor->accX(), float_precision, float_alignment);
  Serial.print(", ");
  printFormattedFloat(Serial, sensor->accY(), float_precision, float_alignment);
  Serial.print(", ");
  printFormattedFloat(Serial, sensor->accZ(), float_precision, float_alignment);
  Serial.print(" ], Gyr (DPS) [ ");
  printFormattedFloat(Serial, sensor->gyrX(), float_precision, float_alignment);
  Serial.print(", ");
  printFormattedFloat(Serial, sensor->gyrY(), float_precision, float_alignment);
  Serial.print(", ");
  printFormattedFloat(Serial, sensor->gyrZ(), float_precision, float_alignment);
  Serial.print(" ], Mag (uT) [ ");
  printFormattedFloat(Serial, sensor->magX(), float_precision, float_alignment);
  Serial.print(", ");
  printFormattedFloat(Serial, sensor->magY(), float_precision, float_alignment);
  Serial.print(", ");
  printFormattedFloat(Serial, sensor->magZ(), float_precision, float_alignment);
  Serial.print(" ], Tmp (C) [ ");
  printFormattedFloat(Serial, sensor->temp(), float_precision, float_alignment);
  Serial.print(" ]");
  Serial.println();
}

#endif // _PRINT_FORMATTED_H_