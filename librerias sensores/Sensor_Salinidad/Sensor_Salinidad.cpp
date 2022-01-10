// ---------------------------------------------------
// LIBRERIA: Sensor Salinidad
// ---------------------------------------------------

#include "Sensor_Salinidad.h" // Libreria propia del sensor de salinidad
#include "Adafruit_ADS1X15.h" // Libreria del ADS1115
#include "EEPROM.h" // Libreria memoria EEPROM (Electrically Erasable Programmable Read-Only Memory)
Adafruit_ADS1115 ADS2;

// ---------------------------------------------------
// ---------------------------------------------------
sensorSalinidad::sensorSalinidad()
{
  ADS2.begin();
  ADS2.setGain(GAIN_ONE);
  EEPROM.begin(512);
  adc = 0;
  PIN = 0;
  PowerPIN = 0;
  NoSaltValue = 0;
  SaltValue = 0;
}


// ---------------------------------------------------
// ---------------------------------------------------
void sensorSalinidad::setADS_PIN(int ADS_pin)
{
	PIN = ADS_pin;
}


// ---------------------------------------------------
// ---------------------------------------------------
void sensorSalinidad::setPower_PIN(int power_pin)
{
	PowerPIN = power_pin;
}


// ---------------------------------------------------
// ---------------------------------------------------
int sensorSalinidad::readValue()
{
	digitalWrite(PowerPIN, HIGH);
	delay(100);
	adc = ADS2.readADC_SingleEnded((*this).PIN);
	digitalWrite(PowerPIN, LOW);
	
	return adc;
}


// ---------------------------------------------------
// ---------------------------------------------------
int sensorSalinidad::percentage()
{
	int percentage;
	percentage = 100*NoSaltValue/(NoSaltValue-SaltValue)-adc*100/(NoSaltValue-SaltValue);
	
	if(percentage>=100)
    	percentage=100;
	if(percentage<=0)
		percentage=0;
		
	return percentage; 
}


// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------
void sensorSalinidad::setNoSaltValue(int EEPROM_addr)
{
	NoSaltValue = (*this).readValue();
	int NoSaltValue_2 = NoSaltValue/100/2;
	EEPROM.write(EEPROM_addr, NoSaltValue_2);
	EEPROM.commit();
}


// ---------------------------------------------------
// ---------------------------------------------------
void sensorSalinidad::setSaltValue(int EEPROM_addr)
{
	SaltValue = (*this).readValue();
	int SaltValue_2 = SaltValue/100/2;
	EEPROM.write(EEPROM_addr, SaltValue_2);
	EEPROM.commit();
}
// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------
void sensorSalinidad::loadNoSaltValue_EEPROM(int EEPROM_addr)
{
	NoSaltValue = EEPROM.read( EEPROM_addr);
	NoSaltValue = NoSaltValue*2*100;
}


// ---------------------------------------------------
// ---------------------------------------------------
void sensorSalinidad::loadSaltValue_EEPROM(int EEPROM_addr)
{
	SaltValue = EEPROM.read( EEPROM_addr);
	SaltValue = SaltValue*2*100;
}
// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------
int sensorSalinidad::getNoSaltValue()
{
	return (*this).NoSaltValue;
}


// ---------------------------------------------------
// ---------------------------------------------------
int sensorSalinidad::getSaltValue()
{
	return (*this).SaltValue;
}
// ------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------
