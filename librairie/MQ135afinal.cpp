/**************************************************************************/
/*!
@file     MQ135.cpp
@author   G.Krocker (Mad Frog Labs)
@license  GNU GPLv3

First version of an Arduino Library for the MQ135 gas sensor
TODO: Review the correction factor calculation. This currently relies on
the datasheet but the information there seems to be wrong.

@section  HISTORY

v1.0 - First release
*/
/**************************************************************************/

#include "MQ135afinal.h"

/**************************************************************************/
/*!
@brief  Default constructor

@param[in] pin  The analog input pin for the readout of the sensor
*/
/**************************************************************************/
///CO2
MQ135_a::MQ135_a(uint8_t pin_a) {
  _pin_a = pin_a;
}

///CO
MQ135_b::MQ135_b(uint8_t pin_b) {
  _pin_b = pin_b;
}

///NH3
MQ135_c::MQ135_c(uint8_t pin_c) {
  _pin_c = pin_c;
}
/**************************************************************************/
/*!
@brief  Get the correction factor to correct for temperature and humidity

@param[in] t  The ambient air temperature
@param[in] h  The relative humidity

@return The calculated correction factor
*/
/**************************************************************************/
// CO2
float MQ135_a::getCorrectionFactor(float t, float h) {
  return CORA * t * t - CORB * t + CORC - (h-33.)*CORD;
}

// CO
float MQ135_b::getCorrectionFactor(float t, float h) {
  return CORA * t * t - CORB * t + CORC - (h-33.)*CORD;
}

// NH3
float MQ135_c::getCorrectionFactor(float t, float h) {
  return CORA * t * t - CORB * t + CORC - (h-33.)*CORD;
}


/**************************************************************************/
/*!
@brief  Get the resistance of the sensor, ie. the measurement value

@return The sensor resistance in kOhm
*/
/**************************************************************************/
// CO2 
float MQ135_a::getResistance() {
  int val = analogRead(_pin_a);
  return ((1023./(float)val) * 5. - 1.)*RLOAD;
}

// CO
float MQ135_b::getResistance() {
  int val = analogRead(_pin_b);
  return ((1023./(float)val) * 5. - 1.)*RLOAD;
}

// NH3
float MQ135_c::getResistance() {
  int val = analogRead(_pin_c);
  return ((1023./(float)val) * 5. - 1.)*RLOAD;
}

/**************************************************************************/
/*!
@brief  Get the resistance of the sensor, ie. the measurement value corrected
        for temp/hum

@param[in] t  The ambient air temperature
@param[in] h  The relative humidity

@return The corrected sensor resistance kOhm
*/
/**************************************************************************/
// CO2
float MQ135_a::getCorrectedResistance(float t, float h) {
  return getResistance()/getCorrectionFactor(t, h);
}

// CO
float MQ135_b::getCorrectedResistance(float t, float h) {
  return getResistance()/getCorrectionFactor(t, h);
}

// NH3
float MQ135_c::getCorrectedResistance(float t, float h) {
  return getResistance()/getCorrectionFactor(t, h);
}

/**************************************************************************/
/*!
@brief  Get the ppm of CO2 sensed (assuming only CO2 in the air)

@return The ppm of CO2 in the air
*/
/**************************************************************************/
// CO2
float MQ135_a::getPPM() {
  return PARA * pow((getResistance()/RZERO0), -PARB);
}

// CO
float MQ135_b::getPPM() {
  return PARA1 * pow((getResistance()/RZERO1), -PARB1);
}

// NH3
float MQ135_c::getPPM() {
  return PARA2 * pow((getResistance()/RZERO2), -PARB2);
}


/**************************************************************************/
/*!
@brief  Get the ppm of CO2 sensed (assuming only CO2 in the air), corrected
        for temp/hum

@param[in] t  The ambient air temperature
@param[in] h  The relative humidity

@return The ppm of CO2 in the air
*/
/**************************************************************************/
// CO2
float MQ135_a::getCorrectedPPM(float t, float h) {
  return PARA * pow((getCorrectedResistance(t, h)/RZERO0), -PARB);
}

// CO
float MQ135_b::getCorrectedPPM(float t, float h) {
  return PARA1 * pow((getCorrectedResistance(t, h)/RZERO1), -PARB1);
}

// NH3
float MQ135_c::getCorrectedPPM(float t, float h) {
  return PARA2 * pow((getCorrectedResistance(t, h)/RZERO2), -PARB2);
}


/**************************************************************************/
/*!
@brief  Get the resistance RZero of the sensor for calibration purposes

@return The sensor resistance RZero in kOhm
*/
/**************************************************************************/
// CO2
float MQ135_a::getRZero() {
  return getResistance() * pow((ATMOCO2/PARA), (1./PARB));
}

// CO
float MQ135_b::getRZero() {
  return getResistance() * pow((ATMOCO/PARA1), (1./PARB1));
}

// NH3
float MQ135_c::getRZero() {
  return getResistance() * pow((ATMONH3/PARA2), (1./PARB2));
}
/**************************************************************************/
/*!
@brief  Get the corrected resistance RZero of the sensor for calibration
        purposes

@param[in] t  The ambient air temperature
@param[in] h  The relative humidity

@return The corrected sensor resistance RZero in kOhm
*/
/**************************************************************************/

// CO2
float MQ135_a::getCorrectedRZero(float t, float h) {
  return getCorrectedResistance(t, h) * pow((ATMOCO2/PARA), (1./PARB));
}

// CO
float MQ135_b::getCorrectedRZero(float t, float h) {
  return getCorrectedResistance(t, h) * pow((ATMOCO/PARA1), (1./PARB1));
}

// NH3
float MQ135_c::getCorrectedRZero(float t, float h) {
  return getCorrectedResistance(t, h) * pow((ATMONH3/PARA2), (1./PARB2));
}
