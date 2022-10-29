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
3. ¿Qué entiende por arquitectura load-store? ¿Qué tipo de instrucciones no posee este tipo de arquitectura?
4. ¿Cómo es el mapa de memoria de la familia?
5. ¿Qué ventajas presenta el uso de los “shadowed pointers” del PSP y el MSP?
6. Describa los diferentes modos de privilegio y operación del Cortex M, sus relaciones y como se conmuta de uno al otro. Describa un ejemplo en el que se pasa del modo privilegiado a no priviligiado y nuevamente a privilegiado.
7. ¿Qué se entiende por modelo de registros ortogonal? Dé un ejemplo
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