// ---------------------------------------------------
// LIBRERIA: Sensor Humedad Capacitivo SEN0193
// ---------------------------------------------------

#ifndef HUMEDAD_YA_INCLUIDO
#define HUMEDAD_YA_INCLUIDO

// ---------------------------------------------------
// ---------------------------------------------------
class sensorHumedad {
	
	private: 
		int adc; // Valor analogico del sensor
		int PIN; // Pin respecto el ADS1115
		int AirValue; // Parametro de calibracion del sensor en SECO 
		int WaterValue; // Parametro de calibracion del sensor en MOJADO
	
	public:
		// Constructor del sensor
		sensorHumedad();
		
		// Determinar el Pin para obtener el valor analogico (ADS1115)
		void setADS_PIN(int ADS_pin);
		
		// Leer el valor analogico del sensor
		int readValue();
		
		// Transformar a porcentaje de la lectura del valor analogico
		int percentage();
		
		// Determinar los parametros de calibracion del sensor
		void setAirValue(int EEPROM_addr);
		void setWaterValue(int EEPROM_addr);
		
		// Guardar en la memoria EEPROM los valores de calibracion
		void loadAirValue_EEPROM(int EEPROM_addr);
		void loadWaterValue_EEPROM(int EEPROM_addr);
		
		// Obtener los valores de calibracion 
		int getAirValue();
		int getWaterValue();
};
#endif
