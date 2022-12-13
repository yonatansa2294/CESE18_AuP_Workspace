
<h1 align="center"> Cuestionario del curso </h1>

Preguntas orientadoras
-----------
1. Describa brevemente los diferentes perfiles de familias de microprocesadores/microcontroladores de ARM. Explique alguna de sus diferencias características.

	En general podriamos describir los perfiles como sigue:
	* ***Perfil A:*** pensado para trabajar en plataformas de aplicaciones abiertas de alto rendimiento como sistemas operativos Windows, Linux o iOS por mencionar algunos, manteniendo un alto desempeño y eficiencia. Manejan una arquitectura ARMV7 y en algunos casos ARMV8.
	*  ***Perfil R:*** orientado a la implementación de sistemas embebidos de gama alta en los que es fundamental un rendimiento en tiempo real con baja latencia, alta capacidad de procesamiento y fiabilidad. En este perfil podemos destacar a los ARM Cortex R4/5/7 con una arquitectura ARMV7A/R
	* ***Perfil M:*** enfocado en cubrir aplicaciones de bajo costo y facil uso; como electronica de consumo o aplicaciones a baja escala, donde se apunta a cumplir requerimientos de eficiencia tanto de procesamiento como energetica, con baja latencia de interrupciones y costo. En este perfil podemos destacar a los ARM Cortex M0/M0+/M3/M4 y M7 con una arquitectura ARMV6/7M.

La figura 1, muestra los perfiles de ARM.

***Figura 1.*** Perfiles de ARM

[![ARM-perfiles.png](https://i.postimg.cc/ZnLbn9Dr/ARM-perfiles.png)](https://postimg.cc/zHVZPfNB)

Cortex M
--------------------------------------------------------
1. Describa brevemente las diferencias entre las familias de procesadores ***Cortex M0, M3 y M4***.

	Podemos describir las diferencias entre los procesadores en mención mediante la Tabla 1.
Tabla 1. Diferencias entre Cortex M0, M3 y M4

|Característica|M0|M3|M4|
|:-------------|:-:|:-:|:-:|
|Version de arquitectura|V6M|V7M|V7ME|
|ISA|Thumb,Thumb-2|Thumb+,Thumb2|Thumb+,Thumb2,DSP,SIMD,FP|
|DMIPS/MHz|0.9|1.25|1.25|
|Interrupciones|1-32 +NMI|1-240 +NMI|1-240 +NMI|
|Niveles de Prioridad|4|8-256|8-256|
|MPU|No|Si|Si|
|FPU|No|No|Si|
|Division un solo ciclo|No|Si|Si|
|DSP/SIMD un solo ciclo|No|No|Si|

En cuanto al set de instrucciones del ***Cortex M0***, tienen un set de instrucciones reducido soportando 56 instrucciones y en su mayoría de 16 bits. Por otra parte, los ***Cortex-M3*** soportan más de 100 instrucciones y los ***Cortex-M4*** además tienen instrucciones de DSP y opcionalmente de punto flotante. La figura 2, muestra el core para cada Cortex.

***Figura 2.*** Core de procesadores ARM Cortex M0, M3 y M4

[![Core-ARM-Cortex-M0-M3-M4.jpg](https://i.postimg.cc/nLq1XppH/Core-ARM-Cortex-M0-M3-M4.jpg)](https://postimg.cc/7CYzcv5c)

2. ¿Por qué se dice que el set de instrucciones Thumb permite mayor densidad de código? Explique

	La densidad de código esta asociada con la cantidad de memoria que se necesita para contener instrucciones. Dicho de otra forma es el número de instrucciones que necesito para realizar una operación y la cantidad de memoria que necesito, por lo tanto; mientras menos espacio de memoria ocupe la instrucción y mas operaciones se puedan realizar, el codigo será mas denso. 

	Thumb como tal es una compresión del set de instrucciones ARM de 32 bits original en donde es importante la densidad de código.Con Thumb-1, podemos utilizar instrucciones de 16 bits reduciendo el tamaño de código, pero se pierde rendmiento debido a que se tiene que descomprimir las instrucciones de 16 a 32 bits. Por otro lado, Thumb-2 permite combinar instrucciones Thumb de 32 bits y conjuntos de instrucciones Thumb-1 originales de 16 bits. En comparación con el conjunto de instrucciones ARM de 32 bits, el tamaño del código se reduce, mientras se mantiene un rendimiento similar.

3. ¿Qué entiende por arquitectura load-store? ¿Qué tipo de instrucciones no posee este tipo de arquitectura?

	La arquitectura *load-store* o de carga-almacenamiento, hace referencia a que el set de instrucciones solamente procesa valores que estén en los registros o directamente especificados dentro de la instrucción en sí misma (valor inmediato), siendo el resultado de la operación guardado en un registro. Las únicas operaciones que se aplican a la memoria son aquellas que copian datos de la memoria en los registros (instrucciones de carga) o copian datos de los registros en la memoria (instrucciones de almacenamiento).

4. ¿Cómo es el mapa de memoria de la familia?

	El mapa de memoria se puede describir como un arreglo lineal de bytes numerados desde cero hasta 2<sup>32</sup>-1, haciendo un total de 4GB de almacenamiento. Las palabras están siempre alineadas en bloques de 4 bytes(32 bits), por lo tanto, los 2 bits LSB son cero para mantener la alineación. En algunos procesadores se soporta el acceso no alineado y en otros casos se genera un fallo cuando se produce un acceso no alineado. La Figura 3, describe como se distribuye la memoria para el programa, datos y periféricos.

***Figura 3.*** Mapa de memoria ARM Cortex M3/M4

[![mapa-memoria.png](https://i.postimg.cc/wTnnvr3c/mapa-memoria.png)](https://postimg.cc/ykXLpvmk)

5. ¿Qué ventajas presenta el uso de los “shadowed pointers” del PSP y el MSP?

	El uso del ***Processor Stack Pointer*** (PSP) y el ***Main Stack Pointer*** (MSP), permite la implementación de un RTOS. Para esto se asigna el MSP al sistema operativo y el PSP a cada tarea que se ejecuta, con esto si una tarea de aplicación se encuentra con un problema que lleva a una corrupción de la pila, es probable que la pila utilizada por el núcleo del sistema operativo y otras tareas siga intacta, lo que ayuda a tener un sistema mas confiable.

	Cada tarea de aplicación tiene su propio espacio de pila como se muestra en la Figura 4, y el código de cambio de contexto en el OS actualiza la PSP cada vez que se cambia de contexto.

***Figura 4.*** Asignación del stack para cada tarea

[![PSP-stack-OS.jpg](https://i.postimg.cc/j2fYyRzs/PSP-stack-OS.jpg)](https://postimg.cc/SYmPhFT5)

6. Describa los diferentes modos de privilegio y operación del Cortex M, sus relaciones y como se conmuta de uno al otro. Describa un ejemplo en el que se pasa del modo privilegiado a no privilegiado y nuevamente a privilegiado.

	En lo que se refiere a niveles de acceso, se definen dos niveles: 
	* **Privilegiado:** El nivel de acceso privilegiado puede acceder a todos los recursos del procesador.
	*  **No privilegiado:** En este nivel algunas regiones de memoria y registros pueden resultar inaccesibles, así como algunas operaciones no pueden ser utilizadas.

	En cuanto a los estados se distinguen dos: 
	* ***Thumb:*** el cual se da cuando el procesador esta ejecutando un programa.
	* ***debug:*** cuando el procesador detiene la ejecución de las instrucciones para empezar con la depuración.

	Finalmente en cuanto a los modos de operación que se dan en el estado normal, describimos los siguientes:
	* ***Modo Thread:*** modo ejecutado por defecto al inicio, en el cual se ejecuta el código de forma normal y con acceso privilegiado con la opción de cambiar el acceso a no privilegiado por software. Al entrar en este último nivel de acceso no se puede retornar a un acceso privilegiado salvo se ingrese al modo Handler.
	* ***Modo Handler:*** modo invocado por al producirse una excepción y siempre cuenta con un nivel de acceso privilegiado. Puede darse cuando el modo Thread cuenta con acceso privilegiado o no privilegiado, pero al retornar; siempre lo hace a un modo Thread con acceso privilegiado.

	**Nota:** El **bit [0]** del registro **CONTROL** sirve para seleccionar entre los estados Privilegiado y No Privilegiado durante el modo ***Tread***.

	Ejemplo:  En un RTOS, tanto el OS como las tareas se ejecutan en modo ***Thread*** con la diferencia que el primero lo hace con un acceso privilegiado, mientras que las segundas lo hacen con acceso no privilegiado. Al producirse una interrupción, generalmente producida por el SysTick en la cual se llama al ***scheduler***, que a su vez decide que tarea debe ejecutarse; se cambiar al modo ***Handler*** y al retornar de este, se devolvuelve el acceso privilegiado al OS.

7. ¿Qué se entiende por modelo de registros ortogonal? Dé un ejemplo

	Un modelo de registros ortogonal implica que todos los registros disponibles para el programador son de propósitos generales, con muy pocas excepciones como por ejemplo el contador de programa, el puntero al stack o el enlazador. La familia ARM dispone de 16 registros de 32 bits, accesibles por el programador detiquetados como R0 hasta R15. Solo 3 registros tienen funciones específicas:
	* ***R15:*** contador de programa (*Program Counter,PC*). Automáticamente se incrementa en 4 al finalizar cada instrucción (para
instrucciones de 32-bit), excepto si la instrucción provoca un salto.
	* ***R14:*** registro de enlace o enlazador (*Link Register,LR*), utilizado para almacenar la dirección de retorno cuando se llama a una subrutina o se genera una excepción.
	* ***R13:*** puntero al stack (*Stack Pointer,SP*). 

	**Nota:** Una instrucción de salto, tal como la llamada a una función, cambia el valor del PC a una dirección específica, y guarda su valor (dirección de retorno) en el Link Register (LR)

	Los cuatro primeros registros R0-R3 se utilizan para pasar los valores de los argumentos a una subrutina y para devolver un valor de resultado
de una función. Adicional a estos registros esta el registro de estado xPSR (Program Status Register). Este registro proporciona información sobre la ejecución del programa y sobre los flags de la ALU y se puede dividir en tres zonas:
	* Application PSR (APSR): contiene los flags de la ALU N,Z,C,Q,V.
	* Interrupt PSR (IPSR): contiene los bits que representan el número de subrutina de interrupción que se está ejecutando en un momento dado.
	* Execution PSR (EPSR): contiene los bits para la inhabilitación de interrupciones normales como de alta prioridad, así como para habilitar el set de instrucciones comprimido (Thumb).

	La Figura 5, muestra el significado de cada bit del registro xPSR.

***Figura 5.*** Registros de procesador ARM Cortex M

[![registros-arm-cortex.png](https://i.postimg.cc/VN32W344/registros-arm-cortex.png)](https://postimg.cc/Y4zXpD5m)

Ejemplo: **MOV** PC, #0 ; R15 = 0, salta a la posición 0x00

8. ¿Qué ventajas presenta el uso de intrucciones de ejecución condicional (IT)? Dé un ejemplo

	Además de los saltos condicionales, los procesadores  Cortex-M3 y Cortex-M4, permiten colocar las instrucciones condicionales en un bloque de instrucciones IF-THEN (IT), con lo cual se pueden ejecutar hasta 4 instrucciones en bloque.

	Un bloque de instrucciones IT consta de una instrucción IT, con detalles de la ejecución condicional, seguida de una a cuatro instrucciones (procesamiento de datos/acceso a memoria) de ejecución condicional, las cuales pueden ejecutarse condicionalmente en función de la condición especificada por la instrucción IT y el valor APSR. 

	La última instrucción de ejecución condicional del bloque de IT también puede ser una instrucción de salto condicional. La declaración de la instrucción IT contiene el opcode de la instrucción IT con hasta tres sufijos opcionales de ***"T" (then)*** y ***"E" (else)*** , seguidos de la condición a comprobar, que es la misma que el símbolo de condición para los saltos condicionales.

	Finalmente, el sufijo "T"/"E" indica cuántas instrucciones subsiguientes hay dentro del bloque de instrucciones IT, y si deben o no ejecutarse si se cumple la condición.

````
	Ejemplo:
	1 CMP R0, #1 ; Compara R0 con 1
	;ejecucion condicional
	2 ITE	EQ	   ; Ejecuta la instruccion 3 si la condicion es verdadera
				   ; caso contrario salta a la instruccion 4 
	3 MOVEQ R3, #9 ; R3 = 9 si R0 es igual a 1 (EQ) 
	4 MOVNE R3, #5 ; R3 = 5 si R0 es diferente de 1 (NE)
```` 
9. Describa brevemente las excepciones más prioritarias (reset, NMI, Hardfault).

	Las excepciones son eventos que ocasionan cambios en el flujo de programa. Cuando ocurre una excepcion, el procesador suspende la ejecución actual de las tareas y ejecuta otro código que se encarga de atender la excepcion, llamado manejador de excepciones. Al finalizar, el procesador retoma la ejecuciòn normal del programa.
	* ***Reset:*** se produce cuando el procesador inicia, por un reset en caliente (durante la ejecución) o por una caida en el nivel de alimentación por debajo de un valor especificado como mínimo para el correcto funcionamiento del procesador (*brown-out reset*). 
	* ***NMI (NonMaskable Interrupt):*** esta excepción puede ser ocasionada por un periférico o por software. Esta es la excepción de mayor prioridad despues del reset. Está permanentemente habilitado y tiene una prioridad fija de -2. Los NMI no pueden ser: 
		* enmascarado o impedido de activación por cualquier otra excepción.
		* reemplazada por cualquier excepción que no sea reset.
	* ***Hardfault:*** las excepciones de fallo detectan los accesos ilegales a la memoria y el funcionamiento incorrecto del programa. Esta excepción se produce debido a un error durante el procesamiento de la excepción o porque ningún otro mecanismo de excepción puede gestionarla.

	La Tabla 2, presenta el nivel de prioridad de estos tres tipos de excepción. 
	
|Número de excepción|Número de IRQ|Tipo|Prioridad|Activación|
|:-------------|:-:|:-:|:-:|:-:|
|1|-|Reset|3 (mayor prioridad)|Asincrona|
|2|-14|NMI|2|Asincrona|
|3|-13|Hardfault|1|-|
10. Describa las funciones principales de la pila. ¿Cómo resuelve la arquitectura el llamado a funciones y su retorno?

	La pila es un tipo de mecanismo de uso de memoria que permite utilizar una porción de memoria como búfer de almacenamiento de datos de último en entrar, primero en salir. Cuando se realiza una llamada a una función/subrutina se hace uso de las instrucciones PUSH y POP para guardar el contenido de los bancos de registros . Al principio de la llamada a la función, el contenido de algunos de los registros puede guardarse en la pila mediante la instrucción PUSH, y luego restaurarse a sus valores originales al final de la función mediante la instrucción POP. De esta forma, el código del programa que llamó a la función no perderá ningún dato y podrá seguir ejecutar.
11. Describa la secuencia de reset del microprocesador.

	Cuando se produce un reset el procesador detiene la ejecución del programa sin importar la instrucción. Al desaparecer el reset, se reinicia la ejecución en la dirección especificada por el reset en la tabla de vectores, con nivel privilegiado y modo *Thread*. 
12. ¿Qué entiende por “core peripherals”? ¿Qué diferencia existe entre estos y el resto de los periféricos?

	Como parte del mapa de memoria CortexM, ARM ha predefinido desde la dirección 0xE0000000 hasta la parte superior de la memoria (0xFFFFFFFF) como región del sistema.  El procesador reserva estas regiones de direcciones del bus periférico privado (PPB) para los registros de los periféricos del núcleo a diferencia de los demas perifericos alojados en otra zona de memoria.
	Estos perifericos de núcleo son:  NVIC, System Control Block, System timer o SysTick y la Memory Protection Unit (MPU).
13. ¿Cómo se implementan las prioridades de las interrupciones? Dé un ejemplo

	Los niveles de prioridad se representan con números negativos para indicar que son de mayor prioridad que otras excepciones. Otras excepciones tienen niveles de prioridad programables, que van de 0 a 255.
	
	Los niveles de prioridad de las interrupciones se controlan mediante registros de nivel de prioridad, con un ancho de 3 a 8 bits. La figura 6 muestra ejemplos de niveles de prioridad para 3 y 4 bits de ancho.

***Figura 6.*** Niveles de prioridad disponibles con ancho de prioridad de 3 ó 4 bits

[![prioridad-interrupciones.png](https://i.postimg.cc/CLGjR3LZ/prioridad-interrupciones.png)](https://postimg.cc/9rf4SN9V)	
	
**Nota:** un mayor número de bits de prioridad también puede aumentar el número de compuertas y, por tanto, el consumo de energía de los diseños de silicio.

14. ¿Qué es el CMSIS? ¿Qué función cumple? ¿Quién lo provee? ¿Qué ventajas aporta?

	CMSIS  o Interfaz de software estándar para microcontroladores Cortex (*Cortex Microcontroller Software Interface Standard*),  es un conjunto de herramientas, API, marcos y flujos de trabajo de código abierto proporcionada por ARM de forma gratuita bajo la licencia Apache 2.0. Entre las ventajas principales podemos mencionar que ayudan a simplificar la reutilización del software, con el fin de acelerar la creación y depuración de proyectos y, por lo tanto, lograr una reducción en el tiempo de comercialización de nuevas aplicaciones. La Figura 7, muestra la organización del Core CMSIS
	
***Figura 7.*** Estructura del CMSIS

[![CMSIS.png](https://i.postimg.cc/pTwqnKmB/CMSIS.png)](https://postimg.cc/TLcrzLqy)

15. Cuando ocurre una interrupción, asumiendo que está habilitada ¿Cómo opera el microprocesador para atender a la subrutina correspondiente? Explique con un ejemplo

	Las interrupciones son eventos típicamente generados por hardware (por ejemplo, periféricos o pines de entrada externos) que alteran el flujo de ejecución normal que debe seguir el programa. 
	
	Por ejemplo, si la interupción por recepción de un byte en el periferico UART esta habilitada y se produce dicha condición, la UART  genera una interrupcion solicitando ser atendido por el procesador y la secuencia de ejecución seria la siguiente:
	
		1.  La UART envía una petición de interrupción al procesador.
		2.  El procesador suspende la tarea que se está ejecutando en ese momento. En este paso se guardan en el stack los registros R0-R3, R12, LR y PSR
		3.  El procesador ejecuta una Rutina de Servicio de Interrupción (ISR) asociada a a la UART para atender al periférico, y opcionalmente borra el flag de interrupción por software si es necesario.
		4.  El procesador recupera los registros guardos en el stack en el paso 2 y reanuda la tarea previamente suspendida.

16. ¿Cómo cambia la operación de stacking al utilizar la unidad de punto flotante?

	El banco de registros de punto flotante contiene treinta y dos registros de 32 bits (S0 a S31), que pueden ser organizarse como dieciséis registros de doble palabra de 64 bits (D0 a D15).

	En los llamados a funciones desde una funcion 1 a otra funcion 2, la función 1 guarda en la pila los registros de de S0 a S15 antes de llamar a la función mientras que la función 2 se encarga de guardar los registros S16 al S31 y debe debe restaurar estos registros de la pila antes de volver a la función 1.

17. Explique las características avanzadas de atención a interrupciones: tail chaining y late arrival.

	* ***Tail Chaining:*** tiene lugar cuando se produce una excepción pero el procesador está gestionando otra excepción de la misma o mayor prioridad, ocasionando que entre a un estado pendiente. Cuando el procesador termina de ejecutar el manejador de excepción actual, puede entonces proceder a procesar la petición de excepción/interrupción pendiente, sin embargo, no hace el proceso de apilar y desapilar, por el contrario el procesador entra en el manejador de excepciones de la excepción pendiente tan pronto como sea posible, permitiendo reducir de forma considerable la diferencia de tiempo entre los ambos.
	* ***Late Arrival:*** en el mismo contexto cuando se produce una excepción, el procesador acepta la solicitud de excepción e inicia la operación de apilamiento. Si durante esta operación de apilamiento se produce otra excepción de mayor prioridad, se atenderá primero la excepción tardía de mayor prioridad. 

Las figuras 8 y 9 muestran ejemplos de estos casos.

***Figura 8.*** Tail chaining

[![tail-chaining.png](https://i.postimg.cc/GpYphyCv/tail-chaining.png)](https://postimg.cc/f3zs8kkb)

***Figura 9.*** Late arrival

[![late-arrival.png](https://i.postimg.cc/9X9s8YBd/late-arrival.png)](https://postimg.cc/qgkmgnSq)

18. ¿Qué es el systick? ¿Por qué puede afirmarse que su implementación favorece la portabilidad de los sistemas operativos embebidos?

	Es un temporizador de proposito general presente en todos los procesadores Cortex que opera en modo descendente con una opcion de auto recarga tal cual se muestra en la FIgura 10. Cuenta con una resolución de 24 bits y por defecto la fuente de reloj utilizada por el Systick es el clock de la CPU utilizada por el procesador. 

***Figura 10.*** Temporizador Systick

[![systick.png](https://i.postimg.cc/wjCj1xnj/systick.png)](https://postimg.cc/mPSsq4bv)

	Una vez iniciado el temporizador con cada clock descuenta en 1 desde su valor inicial, una vez que llegue a cero genera una interrupción y se cargará un nuevo valor de cuenta desde el registro de recarga. El propósito principal de este temporizador es generar una interrupción periódica para un sistema operativo en tiempo real (RTOS) para la llamada al sheduler o algún otro software manejado por eventos. En caso no se ejecute un RTOS, también se utilizar como un simple periférico de temporizador.

	Tener en cuenta lo siguiente:
	* Cuando el procesador se detiene para la depuración, el contador no disminuye.
	* El acceso a los cuatro registros que tiene el temporizador requieren un modo privilegiado. 

19. ¿Qué funciones cumple la unidad de protección de memoria (MPU)?

	La MPU o Unidad de Protección de Memoria (*Memory Proteccion Unit*), es la encarga de configurar los permisos necesarios  para acceder a la memoria, adicionales a los que ya vienen configurados de forma predeterminada. Con esto se garantiza que los programas de usuario que se ejecutan en modo no privilegiados, no puedan acceder a los espacios de memoria de control del sistema.
	**Nota:** Cuando se bloquea un acceso sin privilegios, se produce inmediatamente la excepción de fallo, pudiendo ser una excepción HardFault o BusFault.
20. ¿Cuántas regiones pueden configurarse como máximo? ¿Qué ocurre en caso de haber solapamientos de las regiones? ¿Qué ocurre con las zonas de memoria no cubiertas por las regiones definidas?

	 Admite hasta ocho regiones de memoria programables, cada una con sus propias direcciones de inicio, tamaños y configuraciones programables.
	
	Para el caso de existir solapamiento, si una posición de memoria se encuentra en dos regiones MPU programadas, los atributos y permisos de acceso a la memoria se basarán en la región con el número más alto.

	En caso de acceder a una posición de memoria que no está definida en las regiones programadas de la MPU, la transferencia se bloquearía y se activaría una excepción de fallo.

21. ¿Para qué se suele utilizar la excepción PendSV? ¿Cómo se relaciona su uso con el resto de las excepciones? Dé un ejemplo

	PendSV (Pended Service Call) es otro tipo de excepción importante para soportar las operaciones del SO, con el tipo de excepción 14 y un nivel de prioridad programable. 

	Se utiliza por ejemplo en las operaciones de cambio de contexto cuando se utilizan sistemas operativos, retrasando la petición de cambio de contexto hasta que todos los demás manejadores de IRQ hayan completado su procesamiento. Para ello PendSV se programa como la excepción de menor prioridad. 

22. ¿Para qué se suele utilizar la excepción SVC? Expliquelo dentro de un marco de un sistema operativo embebido.

	Esta excepción tiene gran relevancia en el diseño de sistemas operativos y cuenta  con el tipo de excepción 11 y un nivel de prioridad programable y se genera utilizando la instrucción SVC.

	En sistemas con requisitos de robustez y alta seguridad, las tareas de aplicación pueden ejecutarse en un nivel de acceso sin privilegios, y algunos de los recursos de hardware se pueden hardware pueden configurarse para que sólo se acceda a ellos con privilegios (utilizando la MPU). La única forma de que una tarea de aplicación puede acceder a estos recursos de hardware protegidos es a través de los servicios del sistema operativo.
	
	En este punto, SVC puede utilizarse como API para permitir que las tareas de aplicación accedan a los recursos del sistema, a los cuales solo se tiene acceso en modo privilegiado.

ISA
-------
1. ¿Qué son los sufijos y para qué se los utiliza? Dé un ejemplo
	
	Los sufijos acompañan a algunas instrucciones y el efecto que tienen sobre la instruccion depende precisamente del tipo de sufijo utilizado. Podemos mencionar los sufijos para las instrucciones de procesamiento de datos, de acceso a memoria y las del tipo condicional.
Para la instrucciones de acceso a memoria (***load*** y ***store***) tenemos los sufijos que indican el tamaño del dato a manipular.

	Por otro lado, para las instrucciones condicionales debemos mencionar que los procesadores Cortex-M3 y Cortex-M4 admiten los saltos condicionales. Actualizando el registro de estado **APSR** mediante operaciones de datos, o instrucciones como test (TST) o comparar (CMP), se puede controlar el flujo del programa en base al resultado de las operaciones.  Las Tablas 3 y 4 muestran los sufijos para las instrucciones condicionales y para las de acceso a memoria.

Tabla 3. Sufijos de tamaño para instrucciones de acceso a memoria.

|Tipo de dato|Sufijo|Load|Store|
|:-------------|:-:|:-:|:-:|
|8-bit sin signo|B|LDRB|STRB|
|8-bit con signo|SB|LDRSB|STRB|
|16-bit sin signo|H|LDRH|STRH|
|16-bit con signo|SH|LDRSH|STRH|
|32-bit|-|LDR|STR|
|Operaciones sobre stack (32-bits)|-|PUSH|POP|

Tabla 4. Sufijos para instrucciones condicionales.

|Sufijo|Descripción|Flags|
|:-------------|:-:|:-:|
|EQ|Igual|Z=1|
|NE|Diferente|Z=0|
|CS o HS|Mayor o igual, sin signo|C=1|
|GE|Mayor o igual, con signo|N=V|
|LS|Menor o igual, sin signo|C=0 o Z=1|
|LE|Menor o igual que, con signo|Z=1 o N$\neq$|
|MI|Negativo|N=1|
|PL|Positivo o cero|N=0|
|VS|Desbordamiento (***Overflow***)|V=1|
|VC|Sin desbordamiento (***No overflow***)|V=0|
|HI|Mayor que, sin signo|C=1 y Z=0|
|GT|Mayor que, con signo|Z=0 y N=v|
|CC o LO|Menor que, sin signo|C=0|
|LT|Menor que, con signo|N$\neq$|
|AL|Valor por defecto|Algun valor|

2. ¿Para qué se utiliza el sufijo ‘s’? Dé un ejemplo

	El sufijo *** "s"*** es utilizado por las instrucciones orientadas al procesamiento de los datos, permitiendo ademas de ejecutar la operación de la instrucción, actualizar los bits o flags del registro de estado **APSR**.
````Ejemplo: ADDS R0, R1;  R0=(R0)+(R1) y actualiza el registro APSR ````

3. ¿Qué utilidad tiene la implementación de instrucciones de aritmética saturada? Dé un ejemplo con operaciones con datos de 8 bits.

	La aritmética saturada elimina la necesidad de overflow o underflow, dado que si un resultado está fuera de los límites, se satura al valor más cercano permitido.

		Ejemplo: usat	R4,#8,R4		@ saturamos el valor de r4 a 255 (máximo valor para 8 bits) y lo guardamos en el mismo registro
 
4. Describa brevemente la interfaz entre assembler y C ¿Cómo se reciben los argumentos de las funciones? ¿Cómo se devuelve el resultado? ¿Qué registros deben guardarse en la pila antes de ser modificados?

	Los cuatro primeros registros r0-r3 (a1-a4) se utilizan para pasar valores de argumento a una subrutina y para devolver un valor de resultado de una función. También pueden utilizarse para mantener valores intermedios dentro de una rutina (pero, en general, sólo entre llamadas a subrutinas).
	Normalmente, los registros r4-r8, r10 y r11 (v1-v5, v7 y v8) se utilizan para guardar los valores de las variables locales de una rutina. Una subrutina debe conservar el contenido de los registros r4-r8, r10, r11 y SP (y r9 en las variantes PCS que designan a r9 como v6).
5. ¿Qué es una instrucción SIMD? ¿En qué se aplican y que ventajas reporta su uso? Dé un ejemplo.

	Las instrucciones SIMD estan orientadas a mejorar el rendimiento de las tareas en el campo del procesamiento digital de señales e imagenes, dado que procesa múltiples datos en una única operación (Single Instruction, Multiple Data).

	Como tal, la instrucción opera con múltiples valores de 16 u 8 bits empaquetados en registros de propósito general estándar de 32 bits. Esto permite que ciertas operaciones se ejecuten dos o cuatro veces más rápido, sin necesidad de implementar unidades de cálculo adicionales. Los mnemónicos para estas instrucciones se reconocen por tener 8 o 16 añadido a la forma base, indicando el tamaño de los valores de datos operados.

		Ejemplo: uadd8 R0, R1, R2	;R0[7:0]   = R1[7:0] + R2[7:0]
									;R0[15:8]  = R1[15:8] + R2[15:8]
									;R0[23:16] = R1[23:16] + R2[23:16]
									;R0[31:24] = R1[31:24] + R2[31:24]
									
