// ---------------------------------------------------
// LIBRERIA: Sensor Luminosidad
// ---------------------------------------------------

#ifndef LUMINOSIDAD_YA_INCLUIDO
#define LUMINOSIDAD_YA_INCLUIDO

// ---------------------------------------------------
// ---------------------------------------------------
class sensorLuminosidad {
	
	private: 
		int adc; // Valor analogico del sensor
		int PIN; // Pin respecto el ADS1115
		double SaturationValue; // Valor de saturacion del sensor - En funcion de la resistencia usada
		double Brightness; // Luminosidad expuesta al sensor
	
	public:
		// Constructor del sensor
		sensorLuminosidad();
		
		// Determinar el Pin para obtener el valor analogico (ADS1115)
		void setADS_PIN(int ADS_pin);
		
		// Leer el valor analogico del sensor
		int readValue();
		
		// Mostrar la temperatura que recibe el sensor
		double getBrightness();
		
		// Determinar los parametros de calibracion del sensor
		void setSaturationValue(double value);
		
		// Obtener los valores de calibracion 
		double getSaturationValue();
};
#endif
