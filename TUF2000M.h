/**
******************************************************
* @file TUF2000.h
* @brief Libreria para gestrion de medidor ultrasonico TUF2000M
* @author Julian Salas Bartolome
* @version 1.0
* @date 21/10/2024
*
* Necesita las librerias:
*
*    ModbusMaster   en  https://github.com/4-20ma/ModbusMaster
*    SoftwareSerial en h ttps://github.com/junhuanchen/Esp32-SoftwareSerial/tree/master
*
* Docuemntación de interés
*
*   https://images-na.ssl-images-amazon.com/images/I/91CvZHsNYBL.pdf
*   https://www.libe.net/en/flowmeter
*******************************************************/

#include <SoftwareSerial.h>
#include <ModbusMaster.h>



#ifndef TUF2000M_H
	#define TUF2000M_H


#define FLOW_DATA_SIZE 2
#define LONG_DATA_SIZE 2

#define FLOAT_DATA_SIZE  2

// Registros de datos
#define Registro_Flow                          1
#define Registro_PositiveAcumulatorDecimal    11
#define Registro_Menu	                      60
#define Registro_Tecla	                      59
#define Registro_DateTime                     53
#define RegistroFlowForTodayDecimal          139
#define RegistroFlowForMonthDecimal          143
#define RegistroFlowForYearDecimal           147

//Pantallas de Menu
#define Menu_0                            0x0000                    //Menu 0
#define MenuOuterDiameterPipe             0x0011                    //Menu 11
#define MenuThicknessPipe                 0x0012                    //Menu 12
#define MenuMaterialPipe                  0x0014                    //Menu 14
#define MenuFluidType                     0x0020                    //Menu 20
#define MenuTransducerType                0x0023                    //Menu 23
#define MenuTransducerMounting            0x0024                    //Menu 24
#define MenuTransducerSpacing             0x0025                    //Menu 25
#define MenuSave                          0x0026                    //Menu 26
#define Menu_Idioma		                  0x0039                    //Menu Idioma
#define Menu_DateTime                     0x0060                    //Menu Fecha/Hora
#define Menu_Unidades                     0x0031                    //Menu Unidades

/*------------------------------------------------
             OPCIONES EN MENUS
Se han contemplado unicamente las opciones utilizadas
en la aplicación que ha generado esta libreria.
Hay más opciones para cada Menu. 
En la documentacion	del modulo se detallan las opciones
para cada Menu
https://images-na.ssl-images-amazon.com/images/I/91CvZHsNYBL.pdf		 
--------------------------------------------------*/
//Opciones en Menu 14
#define Cobre                               0x34                    //Material de la tuberia Cobre(4). Mas Opciones descritas en M14    
//Opciones en Menu 20
#define Agua                                0x30                    //Fluido por a tuberia Agua(0). Mas Opciones descritas en M20
//Opciones en Menu 23
#define ClampOnTS2                            19                    //Sensores utilizados TS2 (19). Mas Opciones descritas en M23  
//Opciones en Menu 24
#define VMethod                             0x30                    //Metodo de instalacion de los sensores V (0). Mas Opciones descritas en M24  
//Opciones en Menu 31
#define Metros3                             0x30                    //Unidades empleadas m3 (0). Mas Opciones descritas en M31 
#define Litros                              0x31                    //Unidades empleadas litros (1). Mas Opciones descritas en M31 
#define Hora                                0x30					//Unidades de tiempo Hora (0)
#define Dia                                 0x31					//Unidades de tiempo Dia  (1)
#define Segundo                             0x32					//Unidades de tiempo Segundo (2)
#define Minuto                              0x33					//Unidades de tiempo Minuto (3)
//Opciones en Menu 39
#define Ingles		                        0x33	   		        //Idioma utilizado Ingles. Mas Opciones descritas en M39






/*------------------------------------
    Teclas del frontal del dispositivo
--------------------------------------*/
#define Key_Enter 		                  0x003D                    //Enter
#define Key_Incrementar                   0x003F                    //Incrementar
#define Key_Decrementar                   0x003E                    //Decrementar

/*------------------------------------
    Configuracion del sistema
--------------------------------------*/
//Configuracion TUF-2000M
int Idioma = Ingles;
int Unidades = Litros;
int UnidaddeTiempo = Hora;
int Fluido = Agua;

//Datos tuberia
int OuterDiameter = 380;
int thickness = 3175;
int InnerDiameter = 0;
int Material = Cobre;    
int TransducerType =  ClampOnTS2;    
int TransducerMounting = VMethod;               


#define DebugTuf																				//Flag para Debug

ModbusMaster TUF;																				//Asignamos a la variable TUF el bus ModBus


void EnterKey (int nKey);																		//Funcion para simular la pulsacion de una tecla o la entrada de un digito
void WindowMenu (int nMenu);																	//Funcion para ir a un Menu determinado

void ConfiguraIdioma (void);																	//Funcion que configura el idioma
void ConfiguraHoraFecha (  int nSg, int nMinutos, int nHora, int nDia, int nMes, int nAno );	//Funcion para configurar la fecha y hora del TUF-2000M
void ConfiguraUnidades (void);																	//Funcion que configura las unidades del fluido y fe tiempo
void ConfiguraPipe (void);																		//Configura las caracteristicas de la tuberia									
	void ConfiguraOuterDiameterPipe (void);														//Configura el diametro exterior de la tuberia
	void ConfiguraThicknessPipe (void);															//Configura el grosor del cobre
	void ConfiguraMaterialPipe (void);															//Configura el material de la tuberia
void ConfiguraFluidType (void);
void ConfiguraTransducerType (void);
void ConfiguraTransducerMounting (void);
void ConfiguracionSave(void);
void Configura(void);

int IntToBcd (int nDato );
void WriteNumber ( int nNumero );
float LeeRegistrosFloat ( int nRegistro );
long LeeRegistrosLong ( int nRegistro );

float readFlow(void);
float ReadPositiveAcumulator (void);
float FlowForTodayDecimal (void);
float FlowForMonthDecimal (void);
float FlowForYearDecimal (void);



#endif