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
*    SoftwareSerial en https://github.com/junhuanchen/Esp32-SoftwareSerial/tree/master
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

#define INT_DATA_SIZE   1
#define LONG_DATA_SIZE  2
#define FLOAT_DATA_SIZE 2

// Registros de datos
#define Registro_Flow                          1
#define Registro_PositiveAcumulatorDecimal    11
#define Registro_Menu	                      60
#define Registro_Tecla	                      59
#define Registro_DateTime                     53
#define Registro_ErrorCode                    72  
#define Registro_Q                            92  
#define Registro_UStrngth                     93
#define Registro_DStrngth                     94
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
#define WMethod                             0x33                    //Metodo de instalacion de los sensores W (3). Mas Opciones descritas en M24
//Opciones en Menu 26
#define RAM                                 0x30                    //Salvado en RAM (0).  
#define FLASH                               0x31                    //Salvado en FLASH (1). 

//Opciones en Menu 31
#define Metros3                             0x30                    //Unidades empleadas m3 (0). Mas Opciones descritas en M31 
#define Litros                              0x31                    //Unidades empleadas litros (1). Mas Opciones descritas en M31 
#define Hora                                0x30					//Unidades de tiempo Hora (0)
#define Dia                                 0x31					//Unidades de tiempo Dia  (1)
#define Segundo                             0x32					//Unidades de tiempo Segundo (2)
#define Minuto                              0x33					//Unidades de tiempo Minuto (3)
//Opciones en Menu 39
#define Ingles		                        0x33	   		        //Idioma utilizado Ingles(3). Mas Opciones descritas en M39






/*------------------------------------
    Teclas del frontal del dispositivo
--------------------------------------*/
#define Key_Enter 		                  0x003D                    //Enter
#define Key_Incrementar                   0x003F                    //Incrementar
#define Key_Decrementar                   0x003E                    //Decrementar


//#define DebugTuf																				//Flag para Debug

extern ModbusMaster TUF;																				//Asignamos a la variable TUF el bus ModBus

//Funciones auxiliares
void EnterKey (int nKey);																		//Funcion para simular la pulsacion de una tecla o la entrada de un digito
void WindowMenu (int nMenu);																	//Funcion para ir a un Menu determinado
int IntToBcd (int nDato );																		//Convierte un numero decimal en el correposndiente BCD
void WriteNumber ( int nNumero, int nDecimales );												//Escribe un numero en un registro digito a digito
int LeeRegistrosInt ( int nRegistro );                                                          //Lee el dato INT de un registro
float LeeRegistrosFloat ( int nRegistro );														//Lee el dato float de un par de registros
long LeeRegistrosLong ( int nRegistro );														//Lee el dato long de un par de registros

//Funciones configuracion
void ConfiguraIdioma (int Idioma);																//Funcion que configura el idioma
void ConfiguraHoraFecha (  int nSg, int nMinutos, int nHora, int nDia, int nMes, int nAno );	//Funcion para configurar la fecha y hora del TUF-2000M
void ConfiguraUnidades  (int UnidadesFluido, int UnidaddeTiempo);								//Funcion que configura las unidades del fluido y de tiempo
void ConfiguraOuterDiameterPipe (int OuterDiameter, int nDecimales);							//Configura el diametro exterior de la tuberia
void ConfiguraThicknessPipe (int thickness, int nDecimales);									//Configura el grosor del cobre
void ConfiguraMaterialPipe (int Material);														//Configura el material de la tuberia
void ConfiguraFluidType (int Fluido);															//Configura el tipo de fluido
void ConfiguraTransducerType (int TransducerType );												//Configura el tipo de transductor
void ConfiguraTransducerMounting (int TransducerMounting);										//Configura el modo de instalacion de los sensores
void ConfiguracionSave(void);																	//Salva la configuracion a memoria Flash del TUF-2000M



float ReadFlow(void);                                                                           //Obtiene el caudal instantaneo
float ReadPositiveAcumulator (void);                                                            //Obtiene el caudal positivo acumulado
float FlowForTodayDecimal (void);                                                               //Obtine el consumo del dia actual    
float FlowForMonthDecimal (void);                                                               //Obtiene el consumo del mes actual
float FlowForYearDecimal (void);                                                                //Obtiene el consumo acumulado del año actual    
int ReadQ (void);                                                                               //Obtiene el valor Q de los sensores
int ReadUStrength (void);                                                                       //Obtiene el valor de Upstream strength    
int ReadDStrength (void);                                                                       //Obtiene el valor de Downstream strength                                        
int ReadErrorCode (void);                                                                       //Obriene el codigo de error. En nota 4 se describe a que error corresponde cada bit                                                     



#endif