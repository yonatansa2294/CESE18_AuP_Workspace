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
Podemos describir las diferencias entre los procesadores en mencion mediante la Tabla 1:
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

6. Describa los diferentes modos de privilegio y operación del Cortex M, sus relaciones y como se conmuta de uno al otro. Describa un ejemplo en el que se pasa del modo privilegiado a no priviligiado y nuevamente a privilegiado.
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

**Nota: **Una instrucción de salto, tal como la llamada a una función, cambia el valor del PC a una dirección específica, y guarda su valor (dirección de retorno) en el Link Register (LR)

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
9. Describa brevemente las excepciones más prioritarias (reset, NMI, Hardfault).
10. Describa las funciones principales de la pila. ¿Cómo resuelve la arquitectura el llamado a funciones y su retorno?
11. Describa la secuencia de reset del microprocesador.
12. ¿Qué entiende por “core peripherals”? ¿Qué diferencia existe entre estos y el resto de los periféricos?
13. ¿Cómo se implementan las prioridades de las interrupciones? Dé un ejemplo
14. ¿Qué es el CMSIS? ¿Qué función cumple? ¿Quién lo provee? ¿Qué ventajas aporta?
15. Cuando ocurre una interrupción, asumiendo que está habilitada ¿Cómo opera el microprocesador para atender a la subrutina correspondiente? Explique con un ejemplo
16. ¿Cómo cambia la operación de stacking al utilizar la unidad de punto flotante?
17. Explique las características avanzadas de atención a interrupciones: tail chaining y late arrival.
18. ¿Qué es el systick? ¿Por qué puede afirmarse que su implementación favorece la portabilidad de los sistemas operativos embebidos?
Es un temporizador de proposito general presente en todos los procesadores Cortex que opera en modo descendente con una opcion de auto recarga tal cual se muestra en la FIgura 6. Cuenta con una resolución de 24 bits y por defecto la fuente de reloj utilizada por el Systick es el clock de la CPU utilizada por el procesador. 

***Figura 6.*** Temporizador Systick

[![systick.png](https://i.postimg.cc/wjCj1xnj/systick.png)](https://postimg.cc/mPSsq4bv)

Una vez iniciado el temporizador con cada clock descuenta en 1 desde su valor inicial, una vez que llegue a cero genera una interrupción y se cargará un nuevo valor de cuenta desde el registro de recarga. El propósito principal de este temporizador es generar una interrupción periódica para un sistema operativo en tiempo real (RTOS) para la llamada al sheduler o algún otro software manejado por eventos. En caso no se ejecute un RTOS, también se utilizar como un simple periférico de temporizador.

Tener en cuenta lo siguiente:
	* Cuando el procesador se detiene para la depuración, el contador no disminuye.
	* El acceso a los cuatro registros que tiene el temporizador requieren un modo privilegiado. 

19. ¿Qué funciones cumple la unidad de protección de memoria (MPU)?
20. ¿Cuántas regiones pueden configurarse como máximo? ¿Qué ocurre en caso de haber solapamientos de las regiones? ¿Qué ocurre con las zonas de memoria no cubiertas por las regiones definidas?
21. ¿Para qué se suele utilizar la excepción PendSV? ¿Cómo se relaciona su uso con el resto de las excepciones? Dé un ejemplo
22. ¿Para qué se suele utilizar la excepción SVC? Expliquelo dentro de un marco de un sistema operativo embebido.

ISA
-------
1. ¿Qué son los sufijos y para qué se los utiliza? Dé un ejemplo
2. ¿Para qué se utiliza el sufijo ‘s’? Dé un ejemplo
3. ¿Qué utilidad tiene la implementación de instrucciones de aritmética saturada? Dé un ejemplo con operaciones con datos de 8 bits.
4. Describa brevemente la interfaz entre assembler y C ¿Cómo se reciben los argumentos de las funciones? ¿Cómo se devuelve el resultado? ¿Qué registros deben guardarse en la pila antes de ser modificados?
5. ¿Qué es una instrucción SIMD? ¿En qué se aplican y que ventajas reporta su uso? Dé un ejemplo.
