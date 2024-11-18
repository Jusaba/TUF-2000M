# TUF-2000M

TUF-2000M es un medidor de flujo ultrasonico

Ante la necesidad de controlar el consumo de agua de un recinto con jardin y piscina ha sido necesario buscar distintas soluciones para poder obtrener información digital del consumo y a la vez poderla transitir hacia internet.

Después de analizar las distintas soluciones encontradas hemos optado por utilizar el **TUF-2000M** debido a que, al contrario que en otros dispositivos, existe una buena documentacion para poder acceder a los datos que obtiene el medidor.

Para las pruebasa se ha utilizado un **ESP32** en un modulo **Lora 32 V3** y un adaptador a **RS-485** para  acceder al dispositivo. 

La localización de la instalación donde se esta utilizando el montaje no tiene acceso a internet de ahí que se utilice un modulo con **Lora**, el **ESP32** captura la información y la envia por Radio a un punto con Wifi donde otro modulo **Lora 32 V3** recibe por radio la información que debe trasladar a internet mediante Wifi.

El **ESP32** se ha programado con **Arduino CORE** aunque la librería podria utilizarse con cualquier micro basado en **Arduino**

La documentación utilizada se ha descargado de [este sitio](https://images-na.ssl-images-amazon.com/images/I/91CvZHsNYBL.pdf)

## Descripción de acceso

El **TUF-2000M** permite el acceso a la información mediente un display alfanumerico de dos lineas y unas teclas ubicadas en el panel frontal.

Para poder configurar el dispositivo y acceder a la información que proprciona existen algo más de 100 **ventanas de menús** distintas. Unos **menús** son para configurar el dispositivo y adaptarlo a la instalación donde se va a utilizar y otros son los que visualizan la información que registra. Todos loe **menús** están descritos en el manual en  apartado **4 MENU WINDOW DETAILS**

En [este sitio](https://www.libe.net/en/flowmeter) se describen gran cantidad de las **ventanas de menus** del dispositivo.

Además de los **menús** mencionados, el dispositivo dispone de una gran cantidad de **registros** donde almacena los datos de consumo, tiempo, fecha, hora....... Estos **registros** están debidamente descritos en el manual en la sección **7.1.1 MODBUS REGISTER TABLE**

Esos **registros** son de 16 bit y almacenan datos en formato pareja de BCD de 2 bytes, Integer de 16 bit, Long con signo de 32 bit y Float de 32 bit. En el manual, en cada **registro** se detalla en la columna **Formato** el tipo de datos del **registro**.

La mayor parte de la configuración debe hacerse con los **menús**, para acceder a los datos, se puede hacer por pantalla o bien leyendo con el **ESP32** por **MODBUS RS-485** los **registros** mencionados anteriormente.

## Funciones basicas de la libreria.

En este apartado se describirán las funciones que consideraremos auxiliares para poder manejar el dispositivo.

En primer lugar, como se ha dicho, la mayor parte de la configuración del dispositivo se realiza accediendo a los **menús**. Existen un par de **registros** que nos van a facillitar el acceso a los **menus**, concretamente los **registros** **59** y **60**.

El **registro 60** denominado **GO TO WINDOW** nos permite acceder al **menu** de la posición indicada por este registro, si queremos acceder al **menú 39** que es el **menú** para la configuración del idioma,  pondremos en el **registro 60** el valor **39**.

El **registro 59** denominado **KEY TO INPUT** nos permite simular la pulsación de una de las teclas del frontal o la pulsación de un número, para conseguirlo deberemos introducir en el **registro 59** el código de la tecla que queremos pulsar según la tabla detallada en el apartado **7.4 KEY VALUE TABLE** del manual.

Con estos dos **registros** podemos acceder con el **ESP32**  a los **menús** como si lo hicieramos desde el frontal del dispositivo.

Las funciones que se han dispuesto para acceso a la configuración son

    void EnterKey (int nKey);       //Funcion para simular la pulsacion de una tecla o la entrada de un digito
    void WindowMenu (int nMenu);    //Funcion para ir a un Menu determinado



A titulo de ejemplo, para configurar en el dispositivo el idioma inglés, lo podríamso hacer con el siguiente código

```C++

    #define Menu_Idioma	0x0039          //Menu Idioma
    #define Ingles      0x33            //Ingles es la opción 3
    #define Key_Enter   0x003D          //Enter
    
    uint8_t result;

    WindowMenu(Menu_Idioma);            //Vamos al Menu 30 que es el de idioma
    delay(20);  
    if (result == TUF.ku8MBSuccess)     //Si ha habido exito
    {
        EnterKey(Key_Enter);            //Simulamos la tecla <ENT>
        delay(20);    
        EnterKey(Idioma);               //Simulamos la opción 3 del Menu
        delay(20);
        EnterKey(Key_Enter);            //Volvemos a pulsar <ENT>
        delay(20);
        WindowMenu (Menu_0);            //Vamos a la pantalla de menu 0
        delay(20); 	  
    }	

```

Cada **menú** tiene distintas opciones, en **TUF2000M.h** se han definido unas cuantas opciones, solo las que se han necesitado en el proyecto. Lo suyo sería definir todas las opciones pero queda como tarea pendiente definir las ocpciones necesarias en cada proyecto.

Otras funciones necesarias para configurar o leer los datos de los **registros**  del dispositivo son:

    int IntToBcd (int nDato );                          //Convierte un numero decimal en el correposndiente BCD 
    void WriteNumber ( int nNumero, int nDecimales );	//Escribe un numero en un registro digito a digito
    float LeeRegistrosFloat ( int nRegistro );	        //Lee el dato float de un par de registros
    long LeeRegistrosLong ( int nRegistro );	        //Lee el dato long de un par de registros


## Funciones de configuración.
El funciones para configurar el dispositivo son las siguientes:

    void ConfiguraIdioma (void); 	         //Funcion que configura el idioma
    void ConfiguraUnidades (void);	         //Funcion que configura las unidades del fluido y de tiempo
	void ConfiguraOuterDiameterPipe (void);	 //Configura el diametro exterior de la tuberia
	void ConfiguraThicknessPipe (void);      //Configura el grosor del cobre
	void ConfiguraMaterialPipe (void);       //Configura el material de la tuberia
	void ConfiguraFluidType (void);          //Configura el tipo de fluido
	void ConfiguraTransducerType (void);	 //Configura el tipo de transductor
	void ConfiguraTransducerMounting (void); //Configura el modo de instalacion de los sensores
    void ConfiguracionSave(void);			//Salva la configuracion a memoria Flash del TUF-2000M

No se han desarrollado todas las funciones posible pero cualquiera de las relacionadas puede servir de guía para poder crear cualquier otra que se necesite en un proyecto especifico.

Unas funciones funcionan como la detallada para configurar el idioma, otras como  la del diametro de la tuberia necesita que se introduzca el diametro dígito a dígito, por ejemplo para poner un diametro de 20,5 mm será necesario simular la pulsación de el 2, luego el 0, la coma y el 5 y lo haríamos de la siguiente forma

```C++
    WriteNumber ( 205, 1);
```

La función **ConfiguraTransducerType()** necesita una mención especial, NO HE CONSEGUIDO poener el número 19 que es el tipo que se necesita en este proyecto y he tenido que hacerlo de una forma un tanto anormal. Si el tipo es menor que 10, se simula directamente el digito correspondiente al tipo deseado, si es mayor de 9, se simula la pulsación del 9 y luego se simula la tecla de incrementar hasta alcanzar el nuemro correspondiente al tipo deseado. Seguro que no es la manera adecuada pero ha sido la manera con la que lo he conseguido.
						

## Funciones de lectura de datos.