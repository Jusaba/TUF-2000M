    //Aruino & ESP32
    #include "Arduino.h"
      
	

    //#include <StringArray.h>

    //Temporizacion
	#include <ESP32Time.h>

    //Comunicaciones
	#include <SoftwareSerial.h>
	#include <ModbusMaster.h>

	//TUF2000M
	#include <TUF2000M.h>

    /**************************
    * Declaracion de variables
    ***************************/

    #define MODBUS_DEVICE_ID 1              //Direccion del TUF2000M en el bus

    //Configuracion TUF-2000M
	int nIdioma = Ingles;
	int nUnidades = Litros;
	int nUnidaddeTiempo = Hora;
	int nFluido = Agua;
	
	//Datos tuberia
	int nOuterDiameter = 180;                    //18.0
	int nthickness = 1500;                      //1.500
	int nInnerDiameter = 0;
	int nMaterial = Cobre;    
	int nTransducerType =  ClampOnTS2;    
	int nTransducerMounting = WMethod;               
	
	ModbusMaster TUF;                           //Declarada como externa en TUF2000M.h
    int RX_PIN = 17; //                         //Rx para MODBUS
    int TX_PIN = 23; //                         //Tx para MODBUS    


    void Configura(void)
    {
        ConfiguraIdioma (nIdioma);
		ConfiguraUnidades (nUnidades, nUnidaddeTiempo);
  		ConfiguraOuterDiameterPipe (nOuterDiameter, 1);
  		ConfiguraThicknessPipe (nthickness, 3);
  		ConfiguraMaterialPipe (nMaterial);
  		ConfiguraFluidType(nFluido);
  		ConfiguraTransducerType(nTransducerType);
  		ConfiguraTransducerMounting(nTransducerMounting);
  		WindowMenu(MenuTransducerSpacing);                                             //Pantalla Transducer distance mounting
        ConfiguracionSave();

    }

    void setup() {

		Serial.begin(9600);												//Si no debugamos quedan libres los pines Tx, Rx para set urilizados
		Serial.println("Iniciando........");

  	    Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  	    TUF.begin(MODBUS_DEVICE_ID, Serial2);                           //Inicializamos TUF2000M con la direccion 1
        Configura();
    }

    void loop () {
        Serial.print ("Flujo: ");
        Serial.println ( ReadFlow() );
        delay(100);
        Serial.print ("Acumulado: ");
        Serial.println (ReadPositiveAcumulator());
        delay(1000);
    }

