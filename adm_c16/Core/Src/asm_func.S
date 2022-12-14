.syntax unified
.thumb
.text

@ Simbolos exportados
.global asm_svc
.global asm_sum
.global asm_zeros
.global asm_productoEscalar32
.global asm_productoEscalar16
.global asm_productoEscalar12
.global asm_filtroVentana10
.global asm_pack32to16
.global asm_max
.global asm_downsampleM
.global asm_invertir
.global asm_corr
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   void asm_svc (void)
@
.thumb_func
    asm_svc:
        svc 0
        bx lr


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Prototipo en "C":
@   uint32_t asmSum (uint32_t firstOperand, uint32_t secondOperand)
@
@ Parametros recibidos:
@   r0: firstOperand
@   r1: secondOperand
@
@ Valor de retorno:
@   r0: resultado de la suma de firstOperand y secondOperand
@
.thumb_func
    asm_sum:
        add r0, r1  @ r0 = r0 + r1
        bx lr       @ vuelve adonde fue llamada (especificamente, si "pc"
                    @ es la direccion, en el codigo del programa, de la
                    @ instruccion "bl asm_sum" usada para saltar a
                    @ esta funcion, entonces "lr" contiene ese "pc" + 4).
                    @ el valor de retorno debe quedar en el registro r0.
                    @ En este caso, r0 es el resultado de r0 + r1.

        @ Otras alternativas para el retorno
        @ mov pc,lr
        @ bx lr
        @ push {lr} ... pop {pc}

@-----------------------------------------------------------------------------------
@ Prototipo en "C"
@ void zeros (uint32_t * vector, uint32_t longitud)
@ Parametros recibidos:
@   r0: dirección de memoria apuntando al primer elemento del vector
@   r1: cantidad de elementos del vector (longitud)
@
.thumb_func					@ Se le indica al ensamblador que es una funcion
	asm_zeros:
		push	{R2}		@ guardo el valor de r2 en el stack
		movs	R2,#0		@ r2 = 0
	loop_zeros:
		str		R2,[R0],#4	@ *r2=r0 y luego r0+=4
		subs	R1,1		@ r1-- (decremento el numero de elementos por recorrer)
		bne		loop_zeros	@ salta si z=0 debido a que r1!=0 (faltan elementos por recorrer)
	exit_loop_zeros:
		pop		{R2}		@ recupero el valor de r2
		bx 		lr			@ retorno de la subrutina

@-------------------------------------------------------------------------------------------------------------------
@ Prototipo en "C"
@ static void productoEscalar32 (uint32_t * vectorIn, uint32_t * vectorOut, uint32_t longitud, uint32_t escalar)
@ Parametros recibidos:
@   r0: dirección de memoria apuntando al primer elemento del vector de entrada
@   r1: r0: dirección de memoria apuntando al primer elemento del vector de salida
@	r2: cantidad de elementos del vector (longitud)
@	r3: escalar
@
.thumb_func					@ Se le indica al ensamblador que es una funcion
	asm_productoEscalar32:
		push	{R4}		@ guardo el valor de r4 en el stack
		movs	R4,#0		@ r4 = 0
	loop_prod32:
		ldr		R4,[R0],#4	@ r4=*r0 y luego r0+=4
		muls	R4,R3		@ r4*=r3
		str		R4,[R1],#4	@ *r4=r1 y luego r1+=4
		subs	R2,1		@ r2-- (decremento el numero de elementos por recorrer)
		bne		loop_prod32	@ salta si z=0 debido a que r2!=0 (faltan elementos por recorrer)
	exit_loop_prod32:
		pop		{R4}		@ recupero el valor de r4
		bx 		lr			@ retorno de la subrutina

@-------------------------------------------------------------------------------------------------------------------

@ Prototipo en "C"
@ static void productoEscalar16 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint32_t escalar)
@ Parametros recibidos:
@   r0: dirección de memoria apuntando al primer elemento del vector de entrada
@   r1: r0: dirección de memoria apuntando al primer elemento del vector de salida
@	r2: cantidad de elementos del vector (longitud)
@	r3: escalar
@
.thumb_func					@ Se le indica al ensamblador que es una funcion
	asm_productoEscalar16:
		push	{R4}		@ guardo el valor de r4 en el stack
		movs	R4,#0		@ r4 = 0
	loop_prod16:
		ldrh	R4,[R0],#2	@ r4=(*r0) y luego r0+=2
		muls	R4,R3		@ r4*=r3
		strh	R4,[r1],#2	@ (*r4)=r1 y luego r1+=2
		subs	R2,1		@ r2-- (decremento el numero de elementos por recorrer)
		bne		loop_prod16	@ salta si z=0 debido a que r2!=0 (faltan elementos por recorrer)
	exit_loop_prod16:
		pop		{R4}		@ recupero el valor de r4
		bx 		lr			@ retorno de la subrutina

@-------------------------------------------------------------------------------------------------------------------

@ Prototipo en "C"
@ static void productoEscalar12 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint32_t escalar)
@ Parametros recibidos:
@   r0: dirección de memoria apuntando al primer elemento del vector de entrada
@   r1: dirección de memoria apuntando al primer elemento del vector de salida
@	r2: cantidad de elementos del vector (longitud)
@	r3: escalar
@
.thumb_func						@ Se le indica al ensamblador que es una funcion
	asm_productoEscalar12:
		push	{R4}			@ guardo el valor de r4 en el stack
		movs	R4,#0			@ r4 = 0
	loop_prod12:
		ldrh	R4,[R0],#2		@ r4=(*r0) y luego r0+=2
		muls	R4,R3			@ r4*=r3
		usat	R4,#12,R4		@ saturamos el valor de r4 y lo guardamos en el mismo registro
		strh	R4,[R1],#2		@ (*r4)=r1 y luego r1+=2
		subs	R2,1			@ r2-- (decremento el numero de elementos por recorrer)
		bne		loop_prod12		@ salta si z=0 debido a que r2!=0 (faltan elementos por recorrer)
	exit_loop_prod12:
		pop		{R4}			@ recupero el valor de r4
		bx 		lr				@ retorno de la subrutina

@ opcion sin aritmetica saturada
@.thumb_func					@ Se le indica al ensamblador que es una funcion
@	asm_productoEscalar12:
@		push	{R4-R5}			@ guardo el valor de r4 y r5 en el stack
@		movs	R4,#0			@ r4 = 0
@		movs	R5,#0			@ r5 = 0
@	loop_prod12:
@		ldrh	R4,[R0],#2		@ r4=*r0 y luego r0+=2
@		muls	R4,R3			@ r4*=r3
@		mov		R5,R4			@ r5=*r4
@		lsr		R5,#12			@ r5=r5>>12
@		cbz		R5,carga		@ si r5=0 no hay saturacion y se carga el resultado de la multiplicacion
@	saturacion:
@		ldrh	R4,=#0xfff		@ 0x0fff=r4 (saturamos el resultado a 12 bits)
@	carga:
@		strh	R4,[R1],#2		@ *r4=r1 y luego r1+=2
@		subs	R2,1			@ r2-- (decremento el numero de elementos por recorrer)
@		bne		loop_prod12		@ salta si z=0 debido a que r2!=0 (faltan elementos por recorrer)
@	exit_loop_prod12:
@		pop		{R4}			@ recupero el valor de r4
@		bx 		lr				@ retorno de la subrutina
@-------------------------------------------------------------------------------------------------------------------

@ Prototipo en "C"
@ static void filtroVentana10(uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud);
@ Parametros recibidos:
@   r0: dirección de memoria apuntando al primer elemento del vector de entrada
@   r1: dirección de memoria apuntando al primer elemento del vector de salida
@	r2: cantidad de elementos del vector (longitud)
@
.thumb_func								@ Se le indica al ensamblador que es una funcion
	asm_filtroVentana10:
		push	{R4-R10}				@ guardo el valor de r4 a r10 en el stack
		movs	R4,#0					@ pivote
		movs	R8,#0
		mov		R9,R2					@ logitud vector
	loop_shift_piv:
		movs	R5,#0					@ reinicio muestras = 0
		movs	R6,#0					@ reinicio suma acumulada = 0
		movs	R7,#1					@ reinicio corrimientos = 1
	loop_shift_left_window:
		subs	R8,R4,R7				@ calculo el indice anterior al pivote
		ITTT	PL						@ verifico si el indice es >= 0
		ldrhPL	R10,[R0,R8,LSL 1]		@ r10=*(r0+(r8<<1))
		addPL	R6,R10					@ actualizo la suma acumulada sin afectar APSR
		addPL	R5,#1					@ muestras++ sin afectar APSR
	shift_right_window:
		adds	R8,R4,R7				@ calculo el indice posterior al pivote
		cmp		R9,R8					@ verifico si el indice es menor que la longitud
		ITTT	HI
		ldrhHI	R10,[R0,R8,LSL 1]		@ r10=*(r0+(r8<<1))
		addHI	R6,R10					@ actualizo la suma acumulada sin afectar APSR
		addHI	R5,#1					@ muestras++ sin afectar APSR
	iterations:
		cmp		R7,#5					@ comparamos el numero de iteraciones con el valor de la mitad de la ventana
		add		R7,#1					@ iteraciones++ sin afectar el APSR
		ITTE	EQ						@ verifica si son iguales
	mean:
		udivEQ	R8,R6,R5				@ calculo el promedio
		strhEQ	R8,[R1],#2				@ cargamos el valor a memoria
		bNE		loop_shift_left_window	@ en caso falte iterar retornamos
		adds	R4,#1					@ pivote++
		cmp		R4,R9					@ comparamos si no alcanzamos el indicie maximo del vector
		bne		loop_shift_piv			@ volvemos a repetir el ciclo, caso contrario terminamos
	exit_loop_filtro:
		pop		{R4-R10}				@ recupero el valor de r4
		bx		lr					@ retorno de la subrutina
@-------------------------------------------------------------------------------------------------------------------

@ Prototipo en "C"
@ static void pack32to16 (int32_t * vectorIn, int16_t *vectorOut, uint32_t longitud);
@ Parametros recibidos:
@   r0: dirección de memoria apuntando al primer elemento del vector de entrada
@   r1: dirección de memoria apuntando al primer elemento del vector de salida
@	r2: cantidad de elementos del vector (longitud)
@
.thumb_func							@ Se le indica al ensamblador que es una funcion
	asm_pack32to16:
		push	{R4}				@ guardo el valor de r4 y r5 en el stack
		movs	R4,#0				@ r4 = 0
	loop_pack:
		ldr		R4,[R0],#4			@ r4=*r0 y luego r0+=4
		sxth 	R4, R4, ROR #16		@ r4=(int16)(*r4>>16)
		strh	R4,[R1],#2			@ *r4=r1 y luego r1+=2
		subs	R2,#1				@ r2-- (decremento el numero de elementos por recorrer)
		bne		loop_pack			@ salta si z=0 debido a que r2!=0 (faltan elementos por recorrer)
	exit_loop_pack:
		pop		{R4}				@ recupero el valor de r4
		bx		lr					@ retorno de la subrutina
@-------------------------------------------------------------------------------------------------------------------

@ Prototipo en "C"
@ int32_t max (int32_t * vectorIn, uint32_t longitud);
@ Parametros recibidos:
@   r0: dirección de memoria apuntando al primer elemento del vector de entrada
@	r1: cantidad de elementos del vector (longitud)
@
.thumb_func
	asm_max:
		push	{R4-R7}
		movs	R4,#0			@ registro para almacenar el maximo valor
		movs	R5,#0			@ registro para almacenar el valor actual
		movs	R6,#0			@ registro para el indice del vector
		movs	R7,#0			@ registro para el indice del maximo valor
		ldr		R4,[R0],#4		@ copio el primer elemento del vector para tomarlo como el maximo valor, luego R0+=4}
		adds	R6,#1			@ indice++
	loop_max:
		ldr		R5,[R0],#4		@ R5 tiene el valor actual a comparar con el maximo valor actual
		cmp		R5,R4			@ compara si R5(valor actual) > R4(maximo valor actual) -> R5-R4
		ITT		GT				@ ejecuta dos instrucciones si R5>R4
		movgt	R4,R5			@ actualizo R4(maximo valor)
		movgt	R7,R6			@ actualizo el indice donde se encontro el maximo valor
		adds	R6,#1			@ indice++
		cmp		R1,R6
		bne		loop_max		@ salta si z=0 debido a que r1!=r6 (faltan elementos por recorrer)
	exit_loop_max:
		mov		R0,R7			@ retorno el valor del indice
		pop		{R4-R7}			@ recupero los valores de r4-r7
		bx		lr				@ retorno de la subrutina
@-------------------------------------------------------------------------------------------------------------------

@ Prototipo en "C"
@ downsampleM (int32_t * vectorIn, int32_t * vectorOut, uint32_t longitud, uint32_t N);
@ Parametros recibidos:
@   r0: dirección de memoria apuntando al primer elemento del vector de entrada
@	r1: dirección de memoria apuntando al primer elemento del vector de salida
@	r2: cantidad de elementos del vector (longitud)
@	r3: numero de N muestras por la cual se debe descartar un elemento del vector de entrada
@
.thumb_func
	asm_downsampleM:
		push	{R4-R5}				@ resguardo registros para operaciones
		movs	R4,#0				@ registro para guardar los valores del vector de entrada
		movs	R5,#0				@ contador de muestras
	loop_downsampleM:
		ldr		R4,[R0],#4			@ r4=*r0 , luego r0+=4
		adds	R5,#1				@ incrementamos contador de muestras
		cmp		R3,R5				@ comparamos el numero de muestras tomadas con N -> R3-R5
		ITE		NE					@ evalua si R3=R5
		strne	R4,[R1],#4			@ *r4=r1, luego r1+=4
		movseq	R5,#0				@ reinicia contador de muestras
		subs	R2,#1				@ decremento el numero de elementos por recorrer
		bne		loop_downsampleM	@ salta si z=0 debido a que r2!=0 (faltan elementos por recorrer)
	exit_loop_downsampleM:
		pop		{R4-R5}				@ recupero los valores de r4 y r5
		bx		lr					@ retorno de la subrutina
@-------------------------------------------------------------------------------------------------------------------

@ Prototipo en "C"
@ invertir (uint16_t * vector, uint32_t longitud);;
@ Parametros recibidos:
@   r0: dirección de memoria apuntando al primer elemento del vector de entrada
@	r1: cantidad de elementos del vector (longitud)
@
.thumb_func
	asm_invertir:
		push	{R4-R8}
		movs	R4,#0				@ registro para guardar valor temporal
		movs	R5,#0				@ registro para guardar valor en la poscion actual + offset2
		movs	R6,#2				@ registro para guardar numero de intercambios
		udiv	R6,R1,R6			@ valor maximo de intercambios.
		movs	R7,#0				@ offset1 para ir de inicio -> fin
		movs	R8,#0				@ ofsset2 para ir de fin -> inicio
		subs	R1,#1				@ longitud--
	loop_invertir:
		ldrh	R4,[R0,R7,LSL 1]	@ r4 = *(r0+(r7<<1)) -> temp = v[i]
		subs	R8,R1,R7			@ r8 = r1-r7 -> longitud-i
		ldrh	R5,[R0,R8,LSL 1]    @ r5=*(r0+(r8<<1))
		strh	R5,[R0,R7,LSL 1]	@ *r5=r0+(r7<<1) -> v[i]=v[longitud-i]
		strh	R4,[R0,R8,LSL 1]	@ *r4=r0+(r8<<1) -> v[longitud-i]=temp
		adds	R7,#1				@ offset1++
		cmp		R7,R6				@ comparamos si realizamos el numero de intercambios maximos
		bne		loop_invertir		@ salta si z=0 debido a que r7!=r6 (faltan elementos por recorrer)
	exit_loop_invertir:
		push	{R4-R8}				@ recupero los valores de R4-R8
		bx		lr					@ retorno de la subrutina
@-------------------------------------------------------------------------------------------------------------------

@ Prototipo en "C"
@ corr (int16_t * vectorXn, int16_t * vectorYn, int16_t * vectorCorr, uint32_t longitud)
@ Parametros recibidos:
@   r0: dirección de memoria apuntando al primer elemento del vector de la señal Xn
@	r1: dirección de memoria apuntando al primer elemento del vector de la señal Yn
@	r2: direccion del vector de correlación
@	r3: cantidad de elementos del vector (longitud)
@
.thumb_func
	asm_corr:
		push	{R4-R9}
		movs	R4,#1				@ delay
		movs	R6,#0				@ index de vector x
		movs	R7,#0				@ index de vector y
		movs	R8,#1
		movs	R9,#0
		mls		R4,R3,R8,R4			@ r4 = r3*r8+r4 -> delay = 1-longitud = delay_max
	loop_delay:
		cmp		R4,R3				@ delay < longitud?
		bGT		exit_loop_corr		@ si es mayor, ha terminado la correlación
		movs	R5,#0				@ suma acum = 0
		movs	R6,#0				@ indexX = 0
	loop_sum_prod:
		cmp		R6,R3				@ indexX > longitud
		bGE		save_value			@ si es mayor, se ha calculado xn*yn para el primer shift de y[n]
		adds	R7,R6,R4			@ actualizo indexY
		cmp		R7,#0				@ r7 < 0?
		bLT		next_iter_sum_prod	@ si es menor, saltamos a la siguiente iteracion al no estar definidos y[n+L]
	sum_prod:
		cmp		R7,R3				@ r7 > longitud?
		bGT		next_iter_sum_prod	@ si es mayor, saltamos a la siguiente iteración
		ldrh	R8,[R0,R7,LSL 1]	@ r8 = *(r0+(r7<<1) -> r8= x[n+indexY]
		sxth	R8,R8				@ r8=(int16)(*r8)
		ldrh	R9,[R1,R6,LSL 1]	@ r9 = *(r1+(r6<<1) -> r9= y[n+indexX]
		sxth	R9,R9				@ @ r8=(int16)(*r8)
		smlabb	R5,R8,R9,R5			@ r5 = r5 + r8[15:0]*r9[15:0]
	next_iter_sum_prod:
		adds	R6,#1				@ indexX++
		b		loop_sum_prod
	save_value:
		strh	R5,[R2],#2			@ *r5=r2, luego r2+=2
		adds	R4,#1				@ delay++
		b		loop_delay
	exit_loop_corr:
		pop		{R4-R9}				@ recuperamos los registros r4-r9
		bx		lr					@ retorno de la subrutina
