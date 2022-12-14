/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"
#include "stddef.h"
#include "stdio.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "asm_func.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SHIFT_12BITS		12
#define MAX_VALUE_12BITS	0x0FFF
#define NUM_ELEMENTS		10
#define NUM_ELEMENTS_X		10
#define NUM_ELEMENTS_Y		10
#define NUM_ELEMENTS_CORR	(NUM_ELEMENTS_X>NUM_ELEMENTS_Y)?(NUM_ELEMENTS_X*2-1):(NUM_ELEMENTS_Y*2-1)
#define ESCALAR				25
#define NUM_SAMPLES_DISCARD	3
#define SIZE_WINDOW			10
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

ETH_TxPacketConfig TxConfig;
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

 ETH_HandleTypeDef heth;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */
uint32_t u32vectorIn  [NUM_ELEMENTS]={2235, 987, 41, 313, 894, 22, 9, 2294, 1024, 723};
uint32_t u32vectorOut [NUM_ELEMENTS];
uint16_t u16vectorIn  [NUM_ELEMENTS]={250, 9, 32, 4, 5010, 6782, 711, 878, 9230, 1150};
uint16_t u16vectorOut [NUM_ELEMENTS];
int32_t  s32vectorIn  [NUM_ELEMENTS]={71251, -32817, 1521, 278, 85039, 2294, -2154, 97851, 963, -15230};
int32_t  s32vectorOut [NUM_ELEMENTS];
int16_t  s16vectorOut [NUM_ELEMENTS];
int16_t  x_n 		  [NUM_ELEMENTS_X]={1, -2, -3, 4, 5, -8, 25, 175, -11, 9};
int16_t  y_n 		  [NUM_ELEMENTS_Y]={2, -4, 5, 7, 9, 1, -8, 75, -132, 160};
int16_t  vectorCorr	  [NUM_ELEMENTS_CORR];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ETH_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
void zeros (uint32_t * vector, uint32_t longitud);
void productoEscalar32 (uint32_t * vectorIn, uint32_t * vectorOut, uint32_t longitud, uint32_t escalar);
void productoEscalar16 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar);
void productoEscalar12 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar);
void filtroVentana10(uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud);
void pack32to16 (int32_t * vectorIn, int16_t *vectorOut, uint32_t longitud);
int32_t max (int32_t * vectorIn, uint32_t longitud);
void downsampleM (int32_t * vectorIn, int32_t * vectorOut, uint32_t longitud, uint32_t N);
void invertir (uint16_t * vector, uint32_t longitud);
void corr (int16_t * vectorXn, int16_t * vectorYn, int16_t * vectorCorr, uint32_t longitud);

/* USER CODE BEGIN PFP */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}

/*
 * @brief	Initializes a vector with zero values
 * @param	Pointer to vector of type uint32_t
 * @param	Vector length
 * @retval	None
 * */
void zeros (uint32_t *vectorIn, uint32_t longitud)
{
	uint32_t i;

	if(NULL != vectorIn && longitud > 0)	/** check parameters*/
		for(i=0;i<longitud;i++)		/** alternative option: memset(&vectorIn,0,longitud) */
			*vectorIn++ = 0;
	else
		Error_Handler();
}

/*
 * @brief	Implements the product of a vector and a scalar
 * @param	Pointer to input vector of type uint32_t
 * @param	Pointer to output vector of type uint32_t
 * @param	Vector length
 * @param	Scalar value
 * @retval	None
 * */
void productoEscalar32 (uint32_t *vectorIn, uint32_t *vectorOut, uint32_t longitud, uint32_t escalar)
{
	uint32_t i;

	if(NULL != vectorIn && vectorOut != NULL && longitud > 0)/** check parameters*/
	{
		if(escalar != 0)
			for(i=0;i<longitud;i++)
				*vectorOut++ = *vectorIn++ * escalar;
		else
			memset(&vectorOut,0,longitud);
	}
	else
		Error_Handler();
}

/*
 * @brief	Implements the product of a vector and a scalar limited to 16 bits
 * @param	Pointer to input vector of type uint16_t
 * @param	Pointer to output vector of type uint16_t
 * @param	Vector length
 * @param	Scalar value
 * @retval	None
 * */
void productoEscalar16 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar)
{
	uint32_t i;

		if(NULL != vectorIn && vectorOut != NULL && longitud > 0)/** check parameters*/
		{
			if(escalar != 0)
				for(i=0;i<longitud;i++)
					*vectorOut++ = *vectorIn++ * escalar;
			else
				memset(&vectorOut,0,longitud);
		}
		else
			Error_Handler();

}

/*
 * @brief	Implements the product of a vector and a scalar limited to 12 bits
 * @param	Pointer to input vector of type uint16_t
 * @param	Pointer to output vector of type uint16_t
 * @param	Vector length
 * @param	Scalar value
 * @retval	None
 * */
void productoEscalar12 (uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar)
{
	uint32_t i;

		if(NULL != vectorIn && vectorOut != NULL && longitud > 0)/** check parameters*/
		{
			if(escalar != 0)
				for(i=0;i<longitud;i++)
				{
					*vectorOut++ = *vectorIn++ * escalar;
					if((*(vectorOut-1) >>SHIFT_12BITS)>0)
						*(vectorOut-1) = MAX_VALUE_12BITS;
//					vectorIn++;
//					vectorOut++;
				}
			else
				memset(&vectorOut,0,longitud);
		}
		else
			Error_Handler();
}

/*
 * @brief	Implements the product of a vector and a scalar limited to 12 bits
 * @param	Pointer to input vector of type uint16_t
 * @param	Pointer to output vector of type uint16_t
 * @param	Vector length
 * @param	Scalar value
 * @retval	None
 * */
void filtroVentana10(uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud)
{
	uint32_t piv,shift;
	uint16_t sum,samples;
	int32_t index;

	if(NULL != vectorIn && vectorOut != NULL && longitud> 0)/** check parameters*/
	{
		for(piv=0;piv<longitud;piv++)
		{
			samples = 0;
			sum = 0;

			for(shift=1;shift<=SIZE_WINDOW/2;shift++)
			{
				if((index=piv-shift)>=0)
				{
					sum += *(vectorIn+index);
					samples++;
				}
				if((index=piv+shift)<longitud)
				{
					sum += *(vectorIn+index);
					samples++;
				}
			}
			*vectorOut++ = sum/samples;
		}
	}
}

/*
 * @brief	packs a 32-bit vector into a 16-bit vector
 * @param	Pointer to input vector of type int32_t
 * @param	Pointer to output vector of type uint16_t
 * @param	Vector length
 * @retval	None
 * */
void pack32to16 (int32_t * vectorIn, int16_t *vectorOut, uint32_t longitud)
{
	uint32_t i;

	if(NULL != vectorIn && vectorOut != NULL && longitud > 0)/** check parameters*/
		for(i = 0; i < longitud; i++)
			*(vectorOut++) = (int16_t)((*(vectorIn++))>>16);
}

/*
 * @brief	Returns the index of the maximum value inside a vector
 * @param	Pointer to input vector of type int32_t
 * @param	Vector length
 * @retval	index of max.value
 * */
int32_t max (int32_t * vectorIn, uint32_t longitud)
{
	uint32_t i;
	uint32_t index = 0;
	int32_t  maxValue = *vectorIn;

	if(NULL != vectorIn && longitud > 0)/** check parameters*/
	{
		for(i = 1; i < longitud; i++)
		{
			if(*(vectorIn+i)>maxValue)
			{
				maxValue = *(vectorIn+i);
				index = i;
			}
		}
	}
	return index;
}

/*
 * @brief	Discards one value from the input vector for every N samples
 * @param	Pointer to input vector of type uint16_t
 * @param	Pointer to output vector of type uint16_t
 * @param	Vector length
 * @retval	None
 * */
void downsampleM (int32_t * vectorIn, int32_t * vectorOut, uint32_t longitud, uint32_t N)
{
	uint32_t i;

	if(NULL != vectorIn && vectorOut != NULL && longitud > 0 && N != 0)/** check parameters*/
	{
		for(i=0; i<longitud; i++)
		{
			if((i+1) % N != 0)
				*vectorOut++ = *vectorIn;
			vectorIn++;
		}
	}
}

/*
 * @brief	Inverts the elements of a vector
 * @param	Pointer to input vector of type uint16_t
 * @param	Vector length
 * @retval	None
 * */
void invertir (uint16_t * vector, uint32_t longitud)
{
	uint32_t i;
	uint32_t temp;
	longitud--;

	if(NULL != vector && longitud > 0)/** check parameters*/
	{
		for (i=0; i<longitud/2; i++)
		{
			temp = *(vector+i);
			*(vector+i) = *(vector+longitud-i);
			*(vector+longitud-i) = temp;
		}
	}
}

/*
 * @brief	Calculates the cross-correlation of the entered vectors
 * @param	Pointer to input vector of type uint16_t
 * @param	Vector length
 * @retval	None
 * */
void corr (int16_t * vectorXn, int16_t * vectorYn, int16_t * vectorCorr, uint32_t longitud)
{
	int32_t indexX,indexY;
	int16_t sumXnYn;
	int32_t delay;
	int32_t delayMax = -longitud+1;

	if(NULL != vectorXn && vectorYn != NULL && vectorCorr != NULL && longitud > 0)/** check parameters*/
	{

		for(delay=delayMax; delay<(int32_t)longitud; delay++)
		{
			sumXnYn = 0;

			for (indexX=0; indexX<longitud; indexX++)
			{
				indexY = indexX + delay;
				if(indexY >= 0 && indexY < longitud)
					sumXnYn += (*(vectorXn+indexY))*(*(vectorYn+indexX));
			}

			*(vectorCorr++) = sumXnYn;
		}
	}
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void PrivilegiosSVC (void)
{
    // Obtiene valor del registro de 32 bits del procesador llamado "control".
    // El registro guarda los siguientes estados:
    // bit 2: Uso de FPU en el contexto actual. Usado=1, no usado=0.
    // bit 1: Mapeo del stack pointer(sp). MSP=0, PSP=1.
    // bit 0: Modo de ejecucion en Thread. Privilegiado=0, No privilegiado=1.
    //        Recordar que este valor solo se usa en modo Thread. Las
    //        interrupciones siempre se ejecutan en modo Handler con total
    //        privilegio.
    uint32_t x = __get_CONTROL ();

    // Actividad de debug: Ver registro "control" y valor de variable "x".
    //__BKPT (0);

    x |= 1;
    // bit 0 a modo No privilegiado.
    __set_CONTROL (x);

    // En este punto se estaria ejecutando en modo No privilegiado.
    // Lectura del registro "control" para confirmar.
    x = __get_CONTROL ();

    // Actividad de debug: Ver registro "control" y valor de variable "x".
    //__BKPT (0);

    x &= ~1u;
    // Se intenta volver a modo Privilegiado (bit 0, valor 0).
    __set_CONTROL (x);

    // Confirma que esta operacion es ignorada por estar ejecutandose en modo
    // Thread no privilegiado.
    x = __get_CONTROL ();

    // Actividad de debug: Ver registro "control" y valor de variable "x".
    //__BKPT (0);

    // En este punto, ejecutando en modo Thread no privilegiado, la unica forma
    // de volver a modo privilegiado o de realizar cualquier cambio que requiera
    // modo privilegiado, es pidiendo ese servicio a un hipotetico sistema
    // opertivo de tiempo real.
    // Para esto se invoca por software a la interrupcion SVC (Supervisor Call)
    // utilizando la instruccion "svc".
    // No hay intrinsics para realizar esta tarea. Para utilizar la instruccion
    // es necesario implementar una funcion en assembler. Ver el archivo
    // asm_func.S.
    asm_svc ();

    // El sistema operativo (el handler de SVC) deberia haber devuelto el modo
    // de ejecucion de Thread a privilegiado (bit 0 en valor 0).
    x = __get_CONTROL ();

    // Fin del ejemplo de SVC
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint8_t pData[20];
	memset(&pData,0,sizeof(pData));
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  // Activa contador de ciclos (iniciar una sola vez)
  DWT->CTRL |= 1 << DWT_CTRL_CYCCNTENA_Pos;

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ETH_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  /* USER CODE BEGIN 2 */
  PrivilegiosSVC ();

  const uint32_t Resultado = asm_sum (5, 3);

  /* Test de ejercicio 1 */
  zeros (&u32vectorIn[0], sizeof(u32vectorIn)/sizeof(u32vectorIn[0]));
  asm_zeros(&u32vectorIn[0], sizeof(u32vectorIn)/sizeof(u32vectorIn[0]));

  /* Test de ejecicio 2 */
  productoEscalar32 (&u32vectorIn[0], &u32vectorOut[0], sizeof(u32vectorIn)/sizeof(u32vectorIn[0]), ESCALAR);
  asm_productoEscalar32 (&u32vectorIn[0], &u32vectorOut[0],sizeof(u32vectorIn)/sizeof(u32vectorIn[0]), ESCALAR);

  /* Test de ejecicio 3 */
  productoEscalar16 (&u16vectorIn[0], &u16vectorOut[0], sizeof(u16vectorIn)/sizeof(u16vectorIn[0]), ESCALAR);
  asm_productoEscalar16 (&u16vectorIn[0], &u16vectorOut[0], sizeof(u16vectorIn)/sizeof(u16vectorIn[0]), ESCALAR);

  /* Test de ejecicio 4 */
  DWT->CYCCNT = 0;
  productoEscalar12 (&u16vectorIn[0], &u16vectorOut[0], sizeof(u16vectorIn)/sizeof(u16vectorIn[0]), ESCALAR);
  const volatile uint32_t ciclos_ex4c = DWT->CYCCNT;

  DWT->CYCCNT = 0;
  asm_productoEscalar12 (&u16vectorIn[0], &u16vectorOut[0], sizeof(u16vectorIn)/sizeof(u16vectorIn[0]), ESCALAR);
  const volatile uint32_t ciclos_ex4asm = DWT->CYCCNT;

  /* Test de ejecicio 5 */
  DWT->CYCCNT = 0;
  filtroVentana10(&u16vectorIn[0], &u16vectorOut[0], sizeof(u16vectorIn)/sizeof(u16vectorIn[0]));
  const volatile uint32_t ciclos_ex5c = DWT->CYCCNT;

  DWT->CYCCNT = 0;
  asm_filtroVentana10(&u16vectorIn[0], &u16vectorOut[0], sizeof(u16vectorIn)/sizeof(u16vectorIn[0]));
  const volatile uint32_t ciclos_ex5asm = DWT->CYCCNT;

  /* Test de ejecicio 6 */
  pack32to16(&s32vectorIn[0], &s16vectorOut[0], sizeof(s32vectorIn)/sizeof(s32vectorIn[0]));
  asm_pack32to16(&s32vectorIn[0], &s16vectorOut[0], sizeof(s32vectorIn)/sizeof(s32vectorIn[0]));

  /* Test de ejecicio 7 */
  int32_t indexMaxValueASM =  asm_max(&s32vectorIn[0], sizeof(s32vectorIn)/sizeof(s32vectorIn[0]));
  int32_t indexMaxValue = max(&s32vectorIn[0], sizeof(s32vectorIn)/sizeof(s32vectorIn[0]));

  /* Test de ejecicio 8 */
  asm_downsampleM (&s32vectorIn[0], &s32vectorOut[0], sizeof(s32vectorIn)/sizeof(s32vectorIn[0]), NUM_SAMPLES_DISCARD);
  downsampleM (&s32vectorIn[0], &s32vectorOut[0], sizeof(s32vectorIn)/sizeof(s32vectorIn[0]), NUM_SAMPLES_DISCARD);

  /* Test de ejecicio 9 */
  asm_invertir (&u16vectorIn[0], sizeof(u16vectorIn)/sizeof(u16vectorIn[0]));
  invertir (&u16vectorIn[0], sizeof(u16vectorIn)/sizeof(u16vectorIn[0]));

  /* Test de ejercicio 11*/
  DWT->CYCCNT = 0;
  corr (&x_n[0],&y_n[0], &vectorCorr[0], sizeof(x_n)/sizeof(x_n[0]));
  const volatile uint32_t ciclos_ex11c = DWT->CYCCNT;

  DWT->CYCCNT = 0;
  asm_corr (&x_n[0],&y_n[0], &vectorCorr[0], sizeof(x_n)/sizeof(x_n[0]));
  const volatile uint32_t ciclos_ex11asm = DWT->CYCCNT;

  printf("Ex4 C:%lu\r\n",ciclos_ex4c);
  printf("Ex4 ASM:%lu\r\n",ciclos_ex4asm);
  printf("Ex5 C:%lu\r\n",ciclos_ex5c);
  printf("Ex5 ASM:%lu\r\n",ciclos_ex5asm);
  printf("Ex11 C:%lu\r\n",ciclos_ex11c);
  printf("Ex11 C:%lu\r\n",ciclos_ex11asm);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ETH Initialization Function
  * @param None
  * @retval None
  */
static void MX_ETH_Init(void)
{

  /* USER CODE BEGIN ETH_Init 0 */

  /* USER CODE END ETH_Init 0 */

   static uint8_t MACAddr[6];

  /* USER CODE BEGIN ETH_Init 1 */

  /* USER CODE END ETH_Init 1 */
  heth.Instance = ETH;
  MACAddr[0] = 0x00;
  MACAddr[1] = 0x80;
  MACAddr[2] = 0xE1;
  MACAddr[3] = 0x00;
  MACAddr[4] = 0x00;
  MACAddr[5] = 0x00;
  heth.Init.MACAddr = &MACAddr[0];
  heth.Init.MediaInterface = HAL_ETH_RMII_MODE;
  heth.Init.TxDesc = DMATxDscrTab;
  heth.Init.RxDesc = DMARxDscrTab;
  heth.Init.RxBuffLen = 1524;

  /* USER CODE BEGIN MACADDRESS */

  /* USER CODE END MACADDRESS */

  if (HAL_ETH_Init(&heth) != HAL_OK)
  {
    Error_Handler();
  }

  memset(&TxConfig, 0 , sizeof(ETH_TxPacketConfig));
  TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
  TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
  TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;
  /* USER CODE BEGIN ETH_Init 2 */

  /* USER CODE END ETH_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 4;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
