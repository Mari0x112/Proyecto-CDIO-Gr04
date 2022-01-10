// ---------------------------------------------------
// LIBRERIA: Sensor Temperatura
// ---------------------------------------------------

#ifndef TEMPERATURA_YA_INCLUIDO
#define TEMPERATURA_YA_INCLUIDO

// ---------------------------------------------------
// ---------------------------------------------------
class sensorTemperatura {
	
	private: 
		int adc; // Valor analogico del sensor
		int PIN; // Pin respecto el ADS1115
		double V0; // Valor digital del sensor - En funcion de la temperatura
		double m; // Parametro de calibracion del sensor
		double n; // Parametro de calibracion del sensor
		double temperature; // Temperatura obtenida por el sensor
	
	public:
		// Constructor del sensor
		sensorTemperatura();
		
		// Determinar el Pin para obtener el valor analogico (ADS1115)
		void setADS_PIN(int ADS_pin);
		
		// Leer el valor analogico del sensor
		int readValue();
		
		// Mostrar la temperatura que recibe el sensor
		double getTemperature();
		
		// Determinar los parametros de calibracion del sensor
		void setValue_m(double value);
		void setValue_n(double value);
		
		// Obtener los valores de calibracion 
		double getValue_m();
		double getValue_n();
};
#endif
