// ---------------------------------------------------
// LIBRERIA: Sensor Salinidad
// ---------------------------------------------------

#ifndef SALINIDAD_YA_INCLUIDO
#define SALINIDAD_YA_INCLUIDO

// ---------------------------------------------------
// ---------------------------------------------------
class sensorSalinidad {
	
	private: 
		int adc; // Valor analogico del sensor
		int PIN; // Pin respecto el ADS1115
		int PowerPIN; // Pin de alimentacion 
		int NoSaltValue; // Parametro de calibracion del sensor en agua DESTILADA
		int SaltValue; // Parametro de calibracion del sensor en agua SALADA
	
	public:
		// Constructor del sensor
		sensorSalinidad();
		
		// Determinar el Pin para obtener el valor analogico (ADS1115)
		void setADS_PIN(int ADS_pin);
		
		// Determinar el Pin de alimentacion del sensor 
		void setPower_PIN(int power_pin);
		
		// Leer el valor analogico del sensor
		int readValue();
		
		// Transformar a porcentaje de la lectura del valor analogico
		int percentage();
		
		// Determinar los parametros de calibracion del sensor
		void setNoSaltValue(int EEPROM_addr);
		void setSaltValue(int EEPROM_addr);
		
		// Guardar en la memoria EEPROM los valores de calibracion
		void loadNoSaltValue_EEPROM(int EEPROM_addr);
		void loadSaltValue_EEPROM(int EEPROM_addr);
		
		// Obtener los valores de calibracion
		int getNoSaltValue();
		int getSaltValue();	
};
#endif
