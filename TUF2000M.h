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

#define Registro_Flow                          1
#define Registro_PositiveAcumulatorDecimal    11
#define Registro_Menu	                        60
#define Registro_Tecla	                      59
#define Registro_DateTime                     53
#define RegistroFlowForTodayDecimal          139
#define RegistroFlowForMonthDecimal          143
#define RegistroFlowForYearDecimal           147

#define Menu_0                            0x0000                    //Menu 0
#define MenuOuterDiameterPipe             0x0011                    //Menu 11
#define MenuThicknessPipe                 0x0012                    //Menu 12
#define MenuMaterialPipe                  0x0014                    //Menu 14
#define MenuFluidType                     0x0020                    //Menu 20
#define MenuTransducerType                0x0023                    //Menu 23
#define MenuTransducerMounting            0x0024                    //Menu 24
#define MenuTransducerSpacing             0x0025                    //Menu 25
#define MenuSave                          0x0026                    //Menu 26
#define Menu_Idioma		                    0x0039                    //Menu Idioma
#define Menu_DateTime                     0x0060                    //Menu Fecha/Hora
#define Menu_Unidades                     0x0031                    //Menu Unidades

#define Metros3                             0x30                    //Mas Opciones descritas en M31
#define Litros                              0x31                    //Mas Opciones descritas en M31
#define Ingles		                          0x33	   		             //Mas Opciones descritas en M39
#define Cobre                               0x34                    //Mas Opciones descritas en M14    
#define Agua                                0x30                    //Mas Opciones descritas en M20
#define ClampOnTS2                            19                    //Mas Opciones descritas en M23  
#define VMethod                             0x30                    //Mas Opciones descritas en M24  

#define Hora                                0x30
#define Dia                                 0x31
#define Segundo                             0x32
#define Minuto                              0x33




#define Key_Enter 		                    0x003D                    //Enter
#define Key_Incrementar                   0x003F                    //Incrementar
#define Key_Decrementar                   0x003E                    //Decrementar

//Configuracion
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


#define DebugTuf

ModbusMaster TUF;


void EnterKey (int nKey);
void WindowMenu (int nMenu);

void ConfiguraIdioma (void);
void ConfiguraHoraFecha (  int nSg, int nMinutos, int nHora, int nDia, int nMes, int nAno );
void ConfiguraUnidades (void);
void ConfiguraPipe (void);
void ConfiguraOuterDiameterPipe (void);
void ConfiguraThicknessPipe (void);
void ConfiguraMaterialPipe (void);
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