#include <Wire.h> // Librería BUS I2C
#include <Adafruit_ADS1X15.h> // Librería del ADS1115
#include <EEPROM.h> // Libreria EEPROM (Electrically Erasable Programmable Read-Only Memory)
#include <Sensor_Humedad.h> // Librería del SEN0193 (PROPIA)
#include <Sensor_Salinidad.h> // Librería sensor SALINIDAD (PROPIA)
#include <Sensor_Temperatura.h> // Librería sensor TEMPERATURA (PROPIA)
#include <Sensor_Luminosidad.h> // Librería sensor LUMINOSIDAD (PROPIA)
#include <ESP8266WiFi.h> // Librería para conectarse a internet - ThingSpeak

// CONSTRUCTORES:
Adafruit_ADS1115 ads1115; // Constructor del ads1115
sensorHumedad HUMEDAD; // Constructor sen0193
sensorSalinidad SALINIDAD; // Constructor salinidad
sensorTemperatura TEMPERATURA; // Constructor temperatura
sensorLuminosidad LUMINOSIDAD; // Constructor luminosidad

// VARIABLES ------------------------------------------------------------------------------------

   //------- GENERALES -------
   int Cont=0; // Contador - EEPROM(0)
   const int sleep = 15; // Tiempo dormido (s)


   //------- SENSOR HUMEDAD -------
   int AirValue_addr = 1; // Direccion de memoria donde guardar - EEPROM(1)
   int WaterValue_addr = 2; // Direccion de memoria donde guardar - EEPROM(2)
   int ADS_PIN_humedad = 0; // Pin respecto el ads1115 al que esta conectado el sensor


   //------- SENSOR SALINIDAD -------
   int NoSaltValue_addr = 3; // Direccion de memoria donde guardar - EEPROM(3)
   int SaltValue_addr = 4; // Direccion de memoria donde guardar - EEPROM(4)
   int ADS_PIN_salinidad = 1; // Pin respecto el ads1115 al que esta conectado el sensor

   //------- SENSOR TEMPERATURA -------
   double Valor_m = 0.0348; // Valor de calibracion m de la recta
   double Valor_n = 0.786; // Valor de calibracion n de la recta
   int ADS_PIN_temperatura = 2; // Pin respecto el ads1115 al que esta conectado el sensor

   //------- SENSOR LUMINOSIDAD -------
   double Valor_Saturacion = 3.58; // Valor de saturacion para el sensor de luminosidad (V)
   int ADS_PIN_luminosidad = 3; // Pin respecto el ads1115 al que esta conectado el sensor
   
// ----------------------------------------------------------------------------------------------


// WIFI -----------------------------------------------------------------------------------------

  // #define WiFi_CONNECTION_UPV // Comentar para conexiones fuera de la UPV
  #define REST_SERVER_THINGSPEAK // Canal para ver los datos en la web - ThingSpeak

  //------- WiFi Definitions -------
  #ifdef WiFi_CONNECTION_UPV //Conexion UPV
    const char WiFiSSID[] = "GTI1";
    const char WiFiPSK[] = "1PV.arduino.Toledo";
  #else //Conexion fuera de la UPV
    const char WiFiSSID[] = "ALCALA - MAS BLANCO";
    const char WiFiPSK[] = "bloque3puerta5";
  #endif

  //------- SERVER Definitions -------
  #if defined(WiFi_CONNECTION_UPV) //Conexion UPV
    const char Server_Host[] = "proxy.upv.es";
    const int Server_HttpPort = 8080;
  #else //Conexion fuera de la UPV - ThingSpeak
    const char Server_Host[] = "api.thingspeak.com";
    const int Server_HttpPort = 80;
  #endif

  WiFiClient client;

  //------- HTTP REST Connection -------
  #ifdef REST_SERVER_THINGSPEAK 
    const char Rest_Host[] = "api.thingspeak.com";
    String MyWriteAPIKey="S8W2GN5PPQT4JLMW"; // Clave del canal de ThingSpeak
  #endif

  #define NUM_FIELDS_TO_SEND 4 // Numero de medidas a enviar al servidor REST (Entre 1 y 8)

  //------- PIN Definitions -------
  const int LED_PIN = 5; // Led propio del ESP8266
  
// ----------------------------------------------------------------------------------------------


// SETUP ----------------------------------------------------------------------------------------
void setup()
{
  Serial.begin(9600);
  ads1115.begin();
  ads1115.setGain(GAIN_ONE);
  EEPROM.begin(512);
  Cont = EEPROM.read(0);
  
  HUMEDAD.setADS_PIN(ADS_PIN_humedad);
  HUMEDAD.loadAirValue_EEPROM(AirValue_addr);
  HUMEDAD.loadWaterValue_EEPROM(WaterValue_addr);
  SALINIDAD.setADS_PIN(ADS_PIN_salinidad);
  SALINIDAD.loadNoSaltValue_EEPROM(NoSaltValue_addr);
  SALINIDAD.loadSaltValue_EEPROM(SaltValue_addr);
  TEMPERATURA.setADS_PIN(ADS_PIN_temperatura);
  TEMPERATURA.setValue_m(Valor_m);
  TEMPERATURA.setValue_n(Valor_n);
  LUMINOSIDAD.setADS_PIN(ADS_PIN_luminosidad);
  LUMINOSIDAD.setSaturationValue(Valor_Saturacion);

  Serial.println(" ");
  Serial.println(" ");
  Serial.println("Despertando ESP8266...");
  Serial.println(" ");
  Serial.println("MEMORIA:");
  Serial.print("AirValue: ");
  Serial.println(HUMEDAD.getAirValue());
  Serial.print("WaterValue: ");
  Serial.println(HUMEDAD.getWaterValue());
  Serial.print("SinSal: ");
  Serial.println(SALINIDAD.getNoSaltValue());
  Serial.print("ConSal: ");
  Serial.println(SALINIDAD.getSaltValue());
  Serial.print("Contador: ");
  Serial.println(Cont);
  delay(5000);
  SetupWiFi();
}
// ----------------------------------------------------------------------------------------------


// LOOP -----------------------------------------------------------------------------------------
void loop()
{
  if(Cont!=1)
  {
    calibracion();
    Cont = 1;
    EEPROM.write(0,Cont);
    EEPROM.commit();
  }
  else
  {
    Serial.println(" ");
    Serial.println("VALORES OBTENIDOS: ");
    funcion_humedad();
    funcion_salinidad();
    funcion_temperatura();
    funcion_luminosidad();
    enviar_datos();
    delay(5000);
    Serial.println("Durmiendo...");
    Serial.println(" ");
    ESP.deepSleep(sleep*1000000);
  }
}
// ----------------------------------------------------------------------------------------------


// HUMEDAD --------------------------------------------------------------------------------------
  void funcion_humedad()
  {
    Serial.print("Humedad: ");
    Serial.println(HUMEDAD.readValue());
    Serial.print("Humedad relativa (%): ");
    Serial.print(HUMEDAD.percentage());
    Serial.print("%");
      if(HUMEDAD.percentage()==100)
    Serial.print("* SATURADO");  
      if(HUMEDAD.percentage()==0)
    Serial.print("* BAJO MINIMOS");
    Serial.println(" ");
  }
// ----------------------------------------------------------------------------------------------


// SALINIDAD ------------------------------------------------------------------------------------
  void funcion_salinidad()
  {
    Serial.print("Salinidad: ");
    Serial.println(SALINIDAD.readValue());
    Serial.print("Porcentaje de sal en el agua (%): ");
    Serial.print(SALINIDAD.percentage());
    Serial.print("%");
      if(SALINIDAD.percentage()==100)
    Serial.print("* SATURADO");  
      if(SALINIDAD.percentage()==0)
    Serial.print("* BAJO MINIMOS");
    Serial.println(" ");
  }
// ----------------------------------------------------------------------------------------------


// TEMPERATURA ----------------------------------------------------------------------------------
  void funcion_temperatura()
  {
    TEMPERATURA.readValue();
    Serial.print("Temperatura (ºC): ");
    Serial.print(TEMPERATURA.getTemperature());
    Serial.println("ºC"); 
  }
// ----------------------------------------------------------------------------------------------


// LUMINOSIDAD ----------------------------------------------------------------------------------
  void funcion_luminosidad()
  {
    LUMINOSIDAD.readValue();
    Serial.print("Luz recibida (%): ");
    Serial.print(LUMINOSIDAD.getBrightness());
    Serial.println("%");
  }
// ----------------------------------------------------------------------------------------------


// CALIBRACION ----------------------------------------------------------------------------------
  void calibracion()
  {
    delay(1000);
    Serial.println("Iniciando calibracion...");
    Serial.println(" ");
    delay(2500);
    calibracion_humedad();
    calibracion_salinidad();
    Serial.println(" ");
    Serial.print("AirValue: ");
    Serial.println(HUMEDAD.getAirValue());
    Serial.print("WaterValuel: ");
    Serial.println(HUMEDAD.getWaterValue());
    Serial.print("Sin Sal: ");
    Serial.println(SALINIDAD.getNoSaltValue());
    Serial.print("Con Sal: ");
    Serial.println(SALINIDAD.getSaltValue());
    delay(6000);
    
    Serial.println(" ");
    Serial.println("      INICIANDO PROGRAMA");
    Serial.println(" ");
    delay(3000);
  }
// ----------------------------------------------------------------------------------------------


// CALIBRACIÓN HUMEDAD---------------------------------------------------------------------------
  void calibracion_humedad()
  {
    Serial.println("HUMEDAD:");
    delay(3500);
    Serial.println(" ");
    Serial.print("Mantenga el sensor de humedad en el aire");
    dots();
    HUMEDAD.setAirValue(AirValue_addr);
    Serial.println("CALIBRADO");
    delay(4000);
    
    Serial.println(" ");
    Serial.print("Introduzca el sensor en agua");
    dots();
    HUMEDAD.setWaterValue(WaterValue_addr);
    Serial.println("CALIBRADO");
    Serial.println(" ");
    delay(4000);
  }
// ----------------------------------------------------------------------------------------------


// CALIBRACION SALINIDAD ------------------------------------------------------------------------
  void calibracion_salinidad()
  {
    Serial.println("SALINIDAD:");
    delay(3500);
    Serial.println(" ");
    Serial.print("Introduzca el sensor de salinidad en agua destilada");
    dots();
    SALINIDAD.setNoSaltValue(NoSaltValue_addr);
    Serial.println("CALIBRADO");
    delay(4000);
    
    Serial.println(" ");
    Serial.print("Introduzca el sensor de salinidad en agua salada");
    dots();
    SALINIDAD.setSaltValue(SaltValue_addr);
    Serial.println("CALIBRADO");
    Serial.println(" ");
    delay(4000);
  }
// ----------------------------------------------------------------------------------------------


// WiFi CONNECtION ------------------------------------------------------------------------------
void connectWiFi()
{
  byte ledStatus = LOW;
  
  Serial.println();
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  
  WiFi.begin(WiFiSSID, WiFiPSK);

  while (WiFi.status() != WL_CONNECTED)
  {
    // Blink the LED
    digitalWrite(LED_PIN, ledStatus); // Alternar LED high/low
    ledStatus = (ledStatus == HIGH) ? LOW : HIGH;
    Serial.println(".");
    delay(500);
  }
  Serial.println( "WiFi Connected" );
  Serial.println(WiFi.localIP()); // Mostrar la dirección IP
}
// ----------------------------------------------------------------------------------------------


// HTTP GET -------------------------------------------------------------------------------------
void HTTPGet(String fieldData[], int numFields)
{
// Esta funcion construye el string de datos a enviar a ThingSpeak mediante el metodo HTTP GET
// La funcion envia "numFields" datos, del array fieldData.
  
  if (client.connect( Server_Host , Server_HttpPort )){
    #ifdef REST_SERVER_THINGSPEAK 
      String PostData= "GET https://api.thingspeak.com/update?api_key=";
      PostData= PostData + MyWriteAPIKey ;
    #else 
      String PostData= "GET http://dweet.io/dweet/for/";
      PostData= PostData + MyWriteAPIKey +"?" ;
    #endif
           
    for( int field = 1; field < (numFields + 1); field++ ){
      PostData += "&field" + String( field ) + "=" + fieldData[ field ];
    }
    
    client.print(PostData);         
    client.println(" HTTP/1.1");
    client.println("Host: " + String(Rest_Host)); 
    client.println("Connection: close");
    client.println();
     
    Serial.println();                
    Serial.println("Datos enviados al servidor.");
    Serial.println();
  }
}
// ----------------------------------------------------------------------------------------------


// SETUP WiFi------------------------------------------------------------------------------------
void SetupWiFi()
{
  connectWiFi();
  
  digitalWrite(LED_PIN, HIGH);
  
  Serial.print("Server_Host: ");
  Serial.println(Server_Host);
  Serial.print("Port: ");
  Serial.println(String( Server_HttpPort ));
  Serial.print("Server_Rest: ");
  Serial.println(Rest_Host);
}
// ----------------------------------------------------------------------------------------------


// ENVIAR DATOS ---------------------------------------------------------------------------------
void enviar_datos()
{
  String data[ NUM_FIELDS_TO_SEND + 1];
  
  data[ 1 ] = String(HUMEDAD.percentage()); // Dato HUMEDAD - Porcentaje (%)

  data[ 2 ] = String(SALINIDAD.percentage()); // Dato SALINIDAD - Porcentaje (%)

  data[ 3 ] = String(TEMPERATURA.getTemperature()); // Dato TEMPERATURA - Grado Celsius (ºC)

  data[ 4 ] = String(LUMINOSIDAD.getBrightness()); // Dato LUMINOSIDAD - Porcentaje (%)
  
  HTTPGet(data, NUM_FIELDS_TO_SEND);  
}
// ----------------------------------------------------------------------------------------------


// PUNTOS -------------------
void dots()
{
  for(int i=0; i<=7; i++)
    {
       delay(1500);
       Serial.print(".");
    }
}
// --------------------------
