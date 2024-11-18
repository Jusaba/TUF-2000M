	//Librerias LoRa 32
    //Aruino & ESP32
    #include <WebServer.h>
    #include "esp_camera.h"
    #include <WiFi.h>
    #include "img_converters.h"
    #include "Arduino.h"
    #include "soc/soc.h"           
    #include "soc/rtc_cntl_reg.h"  
    #include "driver/rtc_io.h"
    #include <WebServer.h>
    
	

    //#include <StringArray.h>
    #include <SPIFFS.h>
    #include <FS.h>
    #include <WiFiUdp.h>
    #include <ArduinoOTA.h>
    #include <Wire.h>
    #include <base64.h>
    #include <libb64/cencode.h>

    // Lora
    #include <LoRa.h>
    #include <SPI.h>
    #include <Wire.h>

    //Pantalla
    #include <Adafruit_GFX.h>
    #include <Adafruit_SSD1306.h>

    //Temporizacion
	#include <ESP32Time.h>

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
	int nOuterDiameter = 380;                    //38.0
	int nthickness = 3175;                      //3.175
	int nInnerDiameter = 0;
	int nMaterial = Cobre;    
	int nTransducerType =  ClampOnTS2;    
	int nTransducerMounting = VMethod;               
	
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
        Serial.println ( readFlow() );
        delay(100);
        Serial.print ("Acumulado: ");
        Serial.println (ReadPositiveAcumulator());
        delay(1000);
    }

