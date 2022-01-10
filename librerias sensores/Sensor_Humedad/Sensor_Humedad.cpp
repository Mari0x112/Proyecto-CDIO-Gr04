// ---------------------------------------------------
// LIBRERIA: Sensor Humedad Capacitivo SEN0193
// ---------------------------------------------------

#include "Sensor_Humedad.h" // Libreria propia del sensor de humedad capacitivo
#include "Adafruit_ADS1X15.h" // Libreria del ADS1115
#include "EEPROM.h" // Libreria memoria EEPROM (Electrically Erasable Programmable Read-Only Memory)
Adafruit_ADS1115 ADS1;

// ---------------------------------------------------
// ---------------------------------------------------
sensorHumedad::sensorHumedad()
{
  ADS1.begin();
  ADS1.setGain(GAIN_ONE);
  EEPROM.begin(512);
  adc = 0;
  PIN = 0;
  AirValue = 0;
  WaterValue = 0;
}


// ---------------------------------------------------
// ---------------------------------------------------
void sensorHumedad::setADS_PIN(int ADS_pin)
{
	PIN = ADS_pin;
}


// ---------------------------------------------------
// ---------------------------------------------------
int sensorHumedad::readValue()
{
  adc = ADS1.readADC_SingleEnded((*this).PIN);
  return adc;
}


// ---------------------------------------------------
// ---------------------------------------------------
int sensorHumedad::percentage()
{
	int percentage;
	percentage = 100*AirValue/(AirValue-WaterValue)-adc*100/(AirValue-WaterValue);
	
	if(percentage>=100)
    	percentage=100;
	if(percentage<=0)
		percentage=0;
		
	return percentage; 
}


// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------
void sensorHumedad::setAirValue(int EEPROM_addr)
{
  AirValue = (*this).readValue();
  int AirValue_2 = AirValue/100/2;
  EEPROM.write(EEPROM_addr, AirValue_2);
  EEPROM.commit();
}


// ---------------------------------------------------
// ---------------------------------------------------
void sensorHumedad::setWaterValue(int EEPROM_addr)
{
  WaterValue = (*this).readValue();
  int WaterValue_2 = WaterValue/100/2;
  EEPROM.write(EEPROM_addr, WaterValue_2);
  EEPROM.commit();
}
// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------
void sensorHumedad::loadAirValue_EEPROM(int EEPROM_addr)
{
  AirValue = EEPROM.read( EEPROM_addr);
  AirValue = AirValue*2*100;
}


// ---------------------------------------------------
// ---------------------------------------------------
void sensorHumedad::loadWaterValue_EEPROM(int EEPROM_addr)
{
  WaterValue = EEPROM.read( EEPROM_addr);
  WaterValue = WaterValue*2*100;
}
// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------
int sensorHumedad::getAirValue()
{
	return (*this).AirValue;
}


// ---------------------------------------------------
// ---------------------------------------------------
int sensorHumedad::getWaterValue()
{
	return (*this).WaterValue;
}
// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------
