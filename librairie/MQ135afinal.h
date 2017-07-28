/**************************************************************************/
/*!
@file     MQ135.h
@author   G.Krocker (Mad Frog Labs)
@license  GNU GPLv3

First version of an Arduino Library for the MQ135 gas sensor
TODO: Review the correction factor calculation. This currently relies on
the datasheet but the information there seems to be wrong.

@section  HISTORY

v1.0 - First release
*/
/**************************************************************************/
#ifndef MQ135_H
#define MQ135_a_H
#define MQ135_b_H
#define MQ135_c_H
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

/// The load resistance on the board
#define RLOAD 10.0
/// Calibration resistance at atmospheric CO2 level
#define RZERO0 101.19
/// Calibration resistance at atmospheric CO level
#define RZERO1 235.28
/// Calibration resistance at atmospheric NH3 level
#define RZERO2 260.04
/// Parameters for calculating ppm of CO2 from sensor resistance
#define PARA 116.6020682
#define PARB 2.769034857

/// Parameters for calculating ppm of CO from sensor resistance
#define PARA1 602.20
#define PARB1 -3.82

/// Parameters for calculating ppm of NH3 from sensor resistance
#define PARA2 116.089
#define PARB2 -2.267

/// Parameters to model temperature and humidity dependence
#define CORA 0.00035
#define CORB 0.02718
#define CORC 1.39538
#define CORD 0.0018

/// Atmospheric CO2 level for calibration purposes
#define ATMOCO2 397.13

/// Atmospheric CO level for calibration purposes
#define ATMOCO 3.5

/// Atmospheric CO level for calibration purposes
#define ATMONH3 5.5

//CO2
class MQ135_a {
 private:
  uint8_t _pin_a;

 public:
  MQ135_a(uint8_t pin_a);
  float getCorrectionFactor(float t, float h);
  float getResistance();
  float getCorrectedResistance(float t, float h);
  float getPPM();
  float getCorrectedPPM(float t, float h);
  float getRZero();
  float getCorrectedRZero(float t, float h);
};

///CO
class MQ135_b {
 private:
  uint8_t _pin_b;

 public:
  MQ135_b(uint8_t pin_b);
  float getCorrectionFactor(float t, float h);
  float getResistance();
  float getCorrectedResistance(float t, float h);
  float getPPM();
  float getCorrectedPPM(float t, float h);
  float getRZero();
  float getCorrectedRZero(float t, float h);
};

///NH3
class MQ135_c {
 private:
  uint8_t _pin_c;

 public:
  MQ135_c(uint8_t pin_c);
  float getCorrectionFactor(float t, float h);
  float getResistance();
  float getCorrectedResistance(float t, float h);
  float getPPM();
  float getCorrectedPPM(float t, float h);
  float getRZero();
  float getCorrectedRZero(float t, float h);
};
#endif
