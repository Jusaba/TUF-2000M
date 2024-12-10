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

#include <TUF2000M.h>

/****************************************************************************
 * Funciones auxiliares
 * 
 * void EnterKey (int nKey)
 * void WindowMenu (int nMenu)
 * long LeeRegistrosLong ( int nRegistro )
 * float LeeRegistrosFloat ( int nRegistro )
 */

/**
* @brief Simula la pulsacion de una tecla en el teclado fronatal
* @param nKey.- Valor de la tecla que se quiere pulsar segun tabla Key Value del apartado 7.4 de especificaciones de TUF-2000M
*/
void EnterKey (int nKey)
{
  uint8_t  result;
  result = TUF.writeSingleRegister (Registro_Tecla-1,nKey);                 //Ponemos en registro de pulsación la tecla que se desea simular
  if (result == TUF.ku8MBSuccess)
  {
    #ifdef DebugTuf
      Serial.println ("EnterKey -> Tecla aceptada ");
    #endif  
  }else{
    #ifdef DebugTuf
      Serial.println ("EnterKey -> Tecla no aceptada ");
    #endif  
  }
}
/**
* @brief Va a una ventana de Menu
*
* @param nMenu.- Ventana del menu al que se quiere acceder
*/
void WindowMenu (int nMenu)
{
   uint8_t result;
   result = TUF.writeSingleRegister (Registro_Menu-1, nMenu);            //Ventana de menu nMenu
} 
/**
* @brief Simula la pulsación de un numero digito a digito
*
* A esta función se le pasa un numero y el numero de decimales que tiene el numero y simula la pulsación digito a digito incluida la coma 
*
* @param nNumero.- Numero a simular, maximo 4 digitos
* @param nDecimales.- Numero de decimales del numero a simular
*/
void WriteNumber ( int nNumero, int nDecimales )
{
    int nDigito;
    int nNumeroTmp = nNumero;
    int nDecimal = 0;
    #ifdef DebugTuf
      Serial.println ("WriteNumber -> Inicio ");
    #endif  
    nDigito = int(nNumeroTmp/1000);
    EnterKey(nDigito);
    delay(20);

    if ( nDecimales == 3 )
    {
        EnterKey(0x3A);
        delay(20);
    }

    nNumeroTmp = nNumeroTmp - ( nDigito * 1000);
    nDigito = int(nNumeroTmp/100);
    EnterKey(nDigito);
    delay(20);

    if ( nDecimales == 2 )
    {
        EnterKey(0x3A);
        delay(20);
    }
    nNumeroTmp = nNumeroTmp - ( nDigito * 100);
    nDigito = int(nNumeroTmp/10);
    EnterKey(nDigito);
    delay(20);

    if ( nDecimales == 1 )
    {
        EnterKey(0x3A);
        delay(20);
    }
    nNumeroTmp = nNumeroTmp - ( nDigito * 10);
    EnterKey(nNumeroTmp);
    delay(20);
    #ifdef DebugTuf
      Serial.println ("WriteNumber -> Final ");
    #endif  

}
/**
* @brief Lee el dato int de un registro
* 
* @param nRegistro.- Registro a leer
* 
* @return Devuelve el int almacenado en el registro
*/
  int LeeRegistrosInt ( int nRegistro )
  {
    uint16_t buf[INT_DATA_SIZE];
    uint8_t  result;
    int DatoSalida = 0;
    result = TUF.readHoldingRegisters(nRegistro, INT_DATA_SIZE);
    if (result == TUF.ku8MBSuccess)
    {
      buf[0] = TUF.getResponseBuffer(0);
      memcpy(&DatoSalida, &buf, sizeof(int));       
      #ifdef DebugTuf
        Serial.print ("LeeRegistroInt -> Dato Leido:  ");
        Serial.println (DatoSalida);
      #endif  
    }else{
      #ifdef DebugTuf
        Serial.println ("LeeRegistrosInt -> Error de lectura  ");
      #endif  
    }
    return (DatoSalida);
  }
/**
* @brief Lee el dato long de un par de registros
* 
* @param nRegistro.- Primer registro a leer
* 
* @return Devuelve el long almacenado en el par de registros
*/
  long LeeRegistrosLong ( int nRegistro )
  {
      uint16_t buf[LONG_DATA_SIZE];
      uint8_t j, result;
      uint16_t temp;
      long DatoSalida = -1;
      result = TUF.readHoldingRegisters(nRegistro, LONG_DATA_SIZE);
      if (result == TUF.ku8MBSuccess)
      {
        for (j = 0; j < LONG_DATA_SIZE; j++)
        {
          buf[j] = TUF.getResponseBuffer(j);
        }    
        temp = buf[1];
        buf[1]=buf[0];
        buf[0]=temp;
        memcpy(&DatoSalida, &buf, sizeof(long));       
        #ifdef DebugTuf
          Serial.print ("LeeRegistrosLong -> Dato Leido:  ");
          Serial.println (DatoSalida);
        #endif  
      }else{
        #ifdef DebugTuf
          Serial.println ("LeeRegistrosLong -> Error de lectura  ");
        #endif  
      }
      return (DatoSalida);
    }


/**
* @brief Lee el dato float de un par de registyros
* 
* @param nRegistro.- Primer registro a leer
* 
* @return Devuelve el float almacenado en el par de registros
*/
  float LeeRegistrosFloat ( int nRegistro )
  {
      uint16_t buf[FLOAT_DATA_SIZE];
      uint8_t j, result;
      uint16_t temp;
      float DatoSalida = -1;
      result = TUF.readHoldingRegisters(nRegistro, FLOW_DATA_SIZE);
      if (result == TUF.ku8MBSuccess)
      {
        for (j = 0; j < FLOAT_DATA_SIZE; j++)
        {
          buf[j] = TUF.getResponseBuffer(j);
        }    
        temp = buf[1];
        buf[1]=buf[0];
        buf[0]=temp;
        memcpy(&DatoSalida, &buf, sizeof(float));       
        #ifdef DebugTuf
          Serial.print ("LeeRegistrosFloat -> Dato Leido:  ");
          Serial.println (DatoSalida);
        #endif  
      }else{
        #ifdef DebugTuf
          Serial.println ("LeeRegistrosFloat -> Error de lectura  ");
        #endif  
      }
      return (DatoSalida);
  }



/************************************************
* Funciones de configuracion
*   ConfiguraIdioma()     
*   ConfiguraHoraFecha (  int nSg, int nMinutos, int nHora, int nDia, int nMes, int nAno )
*   Configura Unidades    
*   ConfiguraOuterDiameterPipe ()  
*   ConfiguraThicknessPipe () 
*   ConfiguraMaterialPipe () 
*   ConfiguraFluidType () 
*   ConfiguraTransducerType () 
*   ConfiguraTransducerMounting () 
*   ConfigurcionSave() 
*/

/**
* @brief Configura el idioma a Ingles
*
* @parameter Idioma.- Numero de idioma deseado
*/
void ConfiguraIdioma (int Idioma)
{
	uint8_t result;
  
  WindowMenu(Menu_Idioma);                                                   //Pantalla idioma
  delay(20);  
  if (result == TUF.ku8MBSuccess)
  {
      EnterKey(Key_Enter);                                                  //<Ent>
      delay(20);    
      EnterKey(Idioma);                                                     //Ingles
      delay(20);
      EnterKey(Key_Enter);
      delay(20);
      WindowMenu (Menu_0);
      delay(20);
   	  #ifdef DebugTuf
        Serial.println("ConfiguraIdioma -> Cambiado idioma");
      #endif  
	}else{
      #ifdef DebugTuf
		    Serial.println("ConfiguraIdioma -> No ha sido posible cambiar el idioma");
      #endif  
	}	
}
/**
******************************************************
* @brief Pone Hora, Minutos, Segundos, dia, mes y año en RTC
*
* @param nSg.- Segundos
* @param nMinutos.- Minutos
* @param nHora.- Horas
* @param nDia.- Dia
* @param nMes.- Mes
* @param nAno.- Año
*/
void ConfiguraHoraFecha (  int nSg, int nMinutos, int nHora, int nDia, int nMes, int nAno )
{
    int Temp = 0;
    int nYear = nAno % 100;

    Temp =  IntToBcd(nMinutos) << 8;                                    //Minutos en formato bcd en byte alto de registro 0
    Temp =  Temp + IntToBcd(nSg);                                       //Segundos en formato bcd en byte bajo de registro 0                              
    TUF.setTransmitBuffer(0, Temp);         
    Temp = 0;
    Temp = IntToBcd(nDia) << 8;                                         //Dia en formato bcd en byte alto de registro 1
    Temp = Temp + IntToBcd(nHora);                                      //Hora en formato bcd en byte bajo de registro 1
    TUF.setTransmitBuffer(1, Temp);                       
    Temp = 0;
    Temp = IntToBcd(nYear) << 8;                                        //Año en formato bcd en byte alto de registro 2
    Temp = Temp + IntToBcd(nMes);                                       //Mes en formato bcd en byte bajo de registro 2
    TUF.setTransmitBuffer(2, Temp);
    TUF.writeMultipleRegisters (Registro_DateTime-1, 3);                //Se graban en TUF-2000M los tres registros

    TUF.writeSingleRegister (Registro_Menu-1, Menu_DateTime);           //Pantalla Fecha/Hora. Se podia acceder tambien con la funcion WindowMenu (Menu_DateTime)

}
/**
******************************************************
* @brief Configura las unidades de medida caudal con las unidades almacenadas e Unidades y UnidaddeTiempo
*
* Los seleccionados serán l/h o m3/h
*
* @param UnidadesFluido.- Nuemro de la opción de las unidades de fluido segun menu 31
* @param UnidaddeTiempo.- Numero de la opcion de unidad de tiempo segun menu 31 
*/
void ConfiguraUnidades (int UnidadesFluido, int UnidaddeTiempo)
{
  uint8_t result;
  WindowMenu(Menu_Unidades);                                                   //Pantalla idioma
  delay(20);  
  if (result == TUF.ku8MBSuccess)
  {
      EnterKey(Key_Enter);                                                  //<Ent>
      delay(20);    
      EnterKey(UnidadesFluido);
      delay(20);
      EnterKey(Key_Enter);                                                  //<Ent>
      delay(20);    
      EnterKey(UnidaddeTiempo);
      delay(20);
      EnterKey(Key_Enter);                                                  //<Ent>
      delay(20);    
     	#ifdef DebugTuf
        Serial.println("ConfiguraUnidades -> Se han registrado las Unidades");
      #endif  
	}else{
      #ifdef DebugTuf
		    Serial.println("ConfiguraUnidades -> No ha sido posible cambiar las unidades");
      #endif  
	}	
}  
/**
******************************************************
* @brief Configura el diametro exterior de la tuberia
* El diametro se encuentra en la variable global OuterDiameter
*
* @param OuterDiameter.- Diametro exterior de la tuberia
* @param nDecimales.- Numero de decimales en el dato pasado como OuterDiameter
*
* Ejemplo, un diametro de 28.5 mm se registraria  con  ConfiguraOuterDiameterPipe (285, 1);
*/
void ConfiguraOuterDiameterPipe (int OuterDiameter, int nDecimales)
{
    uint8_t result;
    WindowMenu(MenuOuterDiameterPipe);                                                   //Pantalla Outer Diameter Pipe
    delay(20);  
    if (result == TUF.ku8MBSuccess)
    {
      EnterKey(Key_Enter);                                                              //<Ent>
      delay(20);

      WriteNumber(OuterDiameter, nDecimales);

      EnterKey(Key_Enter);                                                  //<Ent>
      delay(20);
      EnterKey(Key_Enter);                                                  //<Ent>
      delay(20);
     	#ifdef DebugTuf
        Serial.println("ConfiguraOuterDiameterPipe -> Registrado diametro exterior");
      #endif  
    }else{
     	#ifdef DebugTuf
        Serial.println("ConfiguraOuterDiameterPipe -> No se ha podido Registrar el diametro exterior");
      #endif  
    } 
}
/**
******************************************************
* @brief Configura el grosor de la pared de la tuberia
* El dato se encuentra en la variable global thickness
*
* @param thickness.- Grosor del cobre de la tuberia
* @param nDecimales.- Numero de decimales en el dato pasado como OuterDiameter
*
* Ejemplo, un grosor de 3.175 mm se registraria  con  ConfiguraThicknessPipe (3175, 3);
*
*/
void ConfiguraThicknessPipe (int thickness, int nDecimales)
{

    uint8_t result;
    WindowMenu(MenuThicknessPipe);                                                   //Pantalla Thickness Pipe
    delay(20);  
    if (result == TUF.ku8MBSuccess)
    {
      EnterKey(Key_Enter);                                                              //<Ent>
      delay(20);

      WriteNumber(thickness, nDecimales);

      EnterKey(Key_Enter);                                                  //<Ent>
      delay(20);
      EnterKey(Key_Enter);                                                  //<Ent>
      delay(20);
      EnterKey(Key_Enter);                                                  //<Ent>
      delay(20);
     	#ifdef DebugTuf
        Serial.println("ConfiguraThicknessPipe -> Se ha Registrado el grosor de las paredes");
      #endif      
    }else{
     	#ifdef DebugTuf
        Serial.println("ConfiguraThicknessPipe -> No se ha podido Registrar el grosor de las paredes");
      #endif
    }    
}
/**
******************************************************
* @brief Configura el material de la tuberia
* El material se encuentra en la variable global Material
*
* @param Material.- Numero para opcion de material deseado segun Menu 14
*/
void ConfiguraMaterialPipe (int Material)
{
    uint8_t result;
    WindowMenu(MenuMaterialPipe);                                                      //Pantalla Material Pipe
    delay(20);  
    if (result == TUF.ku8MBSuccess)
    {
      EnterKey(Key_Enter);                                                              //<Ent>
      delay(20);

      EnterKey(Material);                                                               //Cobre
      delay(20);

      EnterKey(Key_Enter);                                                              //<Ent>
      delay(20);
     	#ifdef DebugTuf
        Serial.println("ConfiguraMaterialPipe ->Se  Registrado el material de la tuberia");
      #endif
    }else{
     	#ifdef DebugTuf
        Serial.println("ConfiguraMaterialPipe -> No se ha podido Registrar el material de la tuberia");
      #endif
    }      
}
/**
******************************************************
* @brief Configura el fluido que circula por la tuberia
* El tipo de fluido se encuentra en la variable global Fluido
*
*@param Fluido.- Numero de opción para el fluido seleccionado en Menu 20
*/
void ConfiguraFluidType (int Fluido)
{
    uint8_t result;
    WindowMenu(MenuFluidType);                                                          //Pantalla Fluid Type
    delay(20);  
    if (result == TUF.ku8MBSuccess)
    {
      EnterKey(Key_Enter);                                                              //<Ent>
      delay(20);

      EnterKey(Fluido);                                                               //Cobre
      delay(20);

      EnterKey(Key_Enter);                                                              //<Ent>
      delay(20);
     	#ifdef DebugTuf
        Serial.println("ConfiguraFluidType ->Se ha Registrado el fluido");
      #endif
    }else{
     	#ifdef DebugTuf
        Serial.println("ConfiguraFluidType -> No se ha podido Registrar el fluido");
      #endif
    }        
}
/**
******************************************************
* @brief Configura el tipo de transductor que utiliza
* El modelo de transductor se encuentra en la variable global TransducerType
*
* @param TransducerType.- Tipo de transductor empleado segun descripción en Menu 23
*/
void ConfiguraTransducerType (int TransducerType)
{
    uint8_t result;
    int nOpcion;
    WindowMenu(MenuTransducerType);                                                          //Pantalla Fluid Type
    delay(20);  
    if (result == TUF.ku8MBSuccess)
    {
      EnterKey(Key_Enter);                                                              //<Ent>
      delay(20);

      if ( TransducerType < 10 )                                                          //Si el tipo de transductro es menor de 10
      {
        EnterKey(TransducerType);                                                         //Introducimos el tipo directamente
        delay(20);
      }else{                                                                              //Si es superior a 10
        EnterKey(10);                                                                     //Seleccionamos la opcion 10
        delay(20);
        nOpcion = TransducerType;                                                         //Simulamos la tecla incrementar hasta llegar a la opcion deseada
        while (nOpcion > 10)
        {
          EnterKey(Key_Incrementar);                                                      //<Incrementar>
          delay(100);
          nOpcion--;
        }  
      } 
      EnterKey(Key_Enter);                                                                //<Ent>
      delay(20);
     	#ifdef DebugTuf
        Serial.println("ConfiguraTransducerType -> Se ha Registrado el tipo de transductor");
      #endif      
    }else{
     	#ifdef DebugTuf
        Serial.println("ConfiguraTransducerType -> No se ha podido Registrar el tipo de transductor");
      #endif
    }       
}    
/**
******************************************************
* @brief Configura el metodo de montaje de los transductores
* El tipo de metodp se encuentra en la variable global TransducerMounting
*
* @param TransducerMounting.- Tipo de montaje de los transductores segun descripcion en Menu 24
*/
void ConfiguraTransducerMounting (int TransducerMounting)
{
    uint8_t result;
    WindowMenu(MenuTransducerMounting);                                             //Pantalla Transducer Mounting
    delay(20);  
    if (result == TUF.ku8MBSuccess)
    {
      EnterKey(Key_Enter);                                                          //<Ent>
      delay(20);

      EnterKey(TransducerMounting);                                                 //VMethod
      delay(20);

      EnterKey(Key_Enter);                                                          //<Ent>
      delay(20);
     	#ifdef DebugTuf
        Serial.println("ConfiguraTransducerMounting -> Se ha Registrado el tipo de montaje transductor");
      #endif      
    }else{
     	#ifdef DebugTuf
        Serial.println("ConfiguraTransducerMounting -> No se ha podido Registrar el tipo de montaje transductor");
      #endif
    }        
}
/**
******************************************************
* @brief Salva la configuración en Flash
*/
void ConfiguracionSave (void)
{
	uint8_t result;
  
  WindowMenu(MenuSave);                                                   //Pantalla Save parameter to flash
  delay(20); 
  if (result == TUF.ku8MBSuccess)
  {
      EnterKey(Key_Enter);                                                //<Ent>
      delay(20);    

      EnterKey(FLASH);                                                   //Opción salvar en Flash
      delay(20);    

      EnterKey(Key_Enter);                                               //<Ent>
      delay(20);    

  }      
}
/************************************************
* Lectura de Registros/Datos

*/

/**
* @brief Lee el registro 'flow RATE' (0001) 
* 
*/
float ReadFlow(void) {

    float flow = LeeRegistrosFloat(Registro_Flow);

    #ifdef DebugTuf
      if ( flow != -1) 
      {
        Serial.print("ReadFlow-> El caudal es:  ");
        Serial.println(flow, 6);
      }else{       
        Serial.println("ReadFlow-> Lectura erronea ");
      }  
    #endif
    return (flow);
  
  }
/**
* @brief Lee el registro 'positive acumulator' (0009) 
* 
*/  
float ReadPositiveAcumulator (void)
{
    float Dato = LeeRegistrosFloat(Registro_PositiveAcumulatorDecimal);

    #ifdef DebugTuf
      if ( Dato != -1) 
      {
        Serial.print("ReadPositiveAcumulator-> El acumulado es:  ");
        Serial.println(Dato, 6);
      }else{       
        Serial.println("ReadPositiveAcumulator-> Lectura erronea ");
      }  
    #endif
    return (Dato);
}  
/**
* @brief Lee el registro 'flow for today decimal fraction' (0139) 
*/
float FlowForTodayDecimal (void)
{
    float nDato = LeeRegistrosFloat (RegistroFlowForTodayDecimal);
    #ifdef DebugTuf
		  Serial.print("FlowForTodayDecimal -> Dato Leido: ");
      Serial.println(nDato);
    #endif  
    return (nDato);
}
/**
* @brief Lee el registro 'flow for this month decimal fraction' (0143) 
*/
float FlowForMonthDecimal (void)
{
    float nDato = LeeRegistrosFloat (RegistroFlowForMonthDecimal);
    #ifdef DebugTuf
		  Serial.print("FlowForMonthDecimal -> Dato Leido: ");
      Serial.println(nDato);
    #endif  
    return (nDato);
}
/**
* @brief Lee el registro 'flow for this year decimal fraction' (0147) 
*/
float FlowForYearDecimal (void)
{
    float nDato = LeeRegistrosFloat (RegistroFlowForYearDecimal);
    #ifdef DebugTuf
		  Serial.print("FlowForYearDecimal -> Dato Leido: ");
      Serial.println(nDato);
    #endif  
    return (nDato);
}
/**
* @brief Lee el registro 'Q ' (0092) 
*/
int ReadQ (void)
{
    int nDato = LeeRegistrosInt (Registro_Q-1);
    nDato = nDato & 0x00FF;
    #ifdef DebugTuf
		  Serial.print("Q -> Dato Leido: ");
      Serial.println(nDato);
    #endif  
    return (nDato);  
}
/**
* @brief Lee el registro 'Upstream ' (0093) 
*/
int ReadUStrength (void)
{
    int nDato = LeeRegistrosInt (Registro_UStrngth-1);
    nDato = map(nDato,0,4096,0,1000);
    #ifdef DebugTuf
		  Serial.print("Upstremam Strength-> Dato Leido: ");
      Serial.println(nDato);
    #endif  
    return (nDato);  
}
/**
* @brief Lee el registro 'Dpstream ' (0094) 
*/
int ReadDStrength (void)
{
    int nDato = LeeRegistrosInt (Registro_DStrngth-1);
    nDato = map(nDato,0,4096,0,1000);
    #ifdef DebugTuf
		  Serial.print("Dpstremam Strength-> Dato Leido: ");
      Serial.println(nDato);
    #endif  
    return (nDato);  
}
/**
* @brief Lee el registro 'Dpstream ' (0094) 
*/
int ReadErrorCode (void)
{
    int nDato = LeeRegistrosInt (Registro_ErrorCode-1);
    #ifdef DebugTuf
		  Serial.print("Dpstremam Strength-> Dato Leido: ");
      Serial.println(nDato);
    #endif  
    return (nDato);  
}
/**
* @brief Convierte un numero decimal en el correposndiente BCD
* Dato XY ->nibble Alto X nible bajo Y
* @param Dato.- Dato a convertir
* @return Devuelve Dato convertido a BCD 
*/
int IntToBcd (int nDato )
{
    int nCociente, nResto;

    nCociente = nDato/10;
    nResto = nDato % 10;

    return ( (nCociente*16) + nResto );
}
