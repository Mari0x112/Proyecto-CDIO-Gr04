// ---------------------------------------------------
// LIBRERIA: Sensor Luminosidad
// ---------------------------------------------------

#include "Sensor_Luminosidad.h" // Libreria propia del sensor de luminosidad
#include "Adafruit_ADS1X15.h" // Libreria del ADS1115
Adafruit_ADS1115 ADS4;

// ---------------------------------------------------
// ---------------------------------------------------
sensorLuminosidad::sensorLuminosidad()
{
	ADS4.begin();
	ADS4.setGain(GAIN_ONE);
	adc = 0;
	PIN = 0;
	SaturationValue = 0;
	Brightness = 0;
}


// ---------------------------------------------------
// ---------------------------------------------------
void sensorLuminosidad::setADS_PIN(int ADS_pin)
{
	PIN = ADS_pin;
}


// ---------------------------------------------------
// ---------------------------------------------------
int sensorLuminosidad::readValue()
{
  adc = ADS4.readADC_SingleEnded((*this).PIN);
  return adc;
}


// ---------------------------------------------------
// ---------------------------------------------------
double sensorLuminosidad::getBrightness()
{
	Brightness = (adc/10000)*100/SaturationValue;
	return Brightness;
}


// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------
void sensorLuminosidad::setSaturationValue(double value)
{
	SaturationValue = value;
}


// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------
double sensorLuminosidad::getSaturationValue()
{
	return (*this).SaturationValue;
}
