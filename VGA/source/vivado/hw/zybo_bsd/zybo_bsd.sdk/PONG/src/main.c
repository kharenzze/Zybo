/************************************************************************/
/*																		*/
/*	main.c	--	Pong 							 						*/
/*																		*/
/*	Authors: 	Pablo R�ez S�nchez										*/
/* 				Vicente Cant�n Paterna									*/
/************************************************************************/
/*	Based on ZYBO Base System demonstration, Sam Bobrowicz				*/
/*	Copyright 2014, Digilent Inc.										*/
/************************************************************************/
/*  Module Description: 												*/
/*																		*/
/*		This file is the main file in this proyect. It initializes the	*/
/*		rest of the modules. 											*/
/*																		*/
/************************************************************************/


/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include "pong.h"
#include "timer_ps.h"
#include "xparameters.h"
#include "xuartps.h"
#include "xgpio.h"

/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

/*
 * Display Driver structs
 */
DisplayCtrl hdmiCtrl;
DisplayCtrl vgaCtrl;

/*
 * btn-sw Driver structs
 */
XGpio sw, btn;

/*
 * Framebuffers for each display device
 */
u32 vgaBuf[DISPLAY_NUM_FRAMES][DISPLAY_MAX_FRAME];
u32 hdmiBuf[DISPLAY_NUM_FRAMES][DISPLAY_MAX_FRAME];

/*
 * XPAR redefines
 */
#define VGA_BASEADDR XPAR_AXI_DISPCTRL_0_S_AXI_BASEADDR
#define HDMI_BASEADDR XPAR_AXI_DISPCTRL_1_S_AXI_BASEADDR
#define VGA_VDMA_ID XPAR_AXIVDMA_0_DEVICE_ID
#define HDMI_VDMA_ID XPAR_AXIVDMA_1_DEVICE_ID
#define AUDIO_IIC_ID XPAR_XIICPS_0_DEVICE_ID
#define AUDIO_CTRL_BASEADDR XPAR_AXI_I2S_ADI_1_S_AXI_BASEADDR
#define SCU_TIMER_ID XPAR_SCUTIMER_DEVICE_ID
#define UART_BASEADDR XPAR_PS7_UART_1_BASEADDR
#define SW_BASEADDR XPAR_SWS_4BITS_BASEADDR
#define BTN_BASEADDR XPAR_BTNS_4BITS_BASEADDR

void Menu();

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

int main(void)
{
	//INICIALIZACION DE LAS VARIABLES
	u32 *vgaPtr[DISPLAY_NUM_FRAMES];
	u32 *hdmiPtr[DISPLAY_NUM_FRAMES];
	int i;
	char userInput = 0;

	for (i = 0; i < DISPLAY_NUM_FRAMES; i++)
	{
		vgaPtr[i] = vgaBuf[i];
		hdmiPtr[i] = hdmiBuf[i];
	}

	PongDisplayInitialize(&vgaCtrl, VGA_VDMA_ID, SCU_TIMER_ID, VGA_BASEADDR, DISPLAY_NOT_HDMI, vgaPtr);
	PongDisplayInitialize(&hdmiCtrl, HDMI_VDMA_ID, SCU_TIMER_ID, HDMI_BASEADDR, DISPLAY_HDMI, hdmiPtr);
	XGpio_Initialize(&sw, XPAR_SWS_4BITS_DEVICE_ID);
	XGpio_SetDataDirection(&sw, 1, 0xffffffff);
	XGpio_Initialize(&btn, XPAR_BTNS_4BITS_DEVICE_ID);
	XGpio_SetDataDirection(&btn, 1, 0xffffffff);
	TimerInitialize(SCU_TIMER_ID);

	/* Flush UART FIFO */
	while (XUartPs_IsReceiveData(UART_BASEADDR))
	{
		XUartPs_ReadReg(UART_BASEADDR, XUARTPS_FIFO_OFFSET);
	}

	while (userInput != 'q')
	{
		Menu();

		//lee una entrada de UART, y la muestra por el terminal.
		userInput=readUART(UART_BASEADDR);

		switch (userInput)
		{
		case '1':
			DisplayDemoChangeRes(&vgaCtrl,UART_BASEADDR);
			break;
		case '2':
			DisplayDemoChangeRes(&hdmiCtrl,UART_BASEADDR);
			break;
		case '3':
			pong(&vgaCtrl ,UART_BASEADDR, &btn, &sw);
			break;
		case '4':
			pong(&hdmiCtrl,UART_BASEADDR, &btn, &sw);
			break;
		case 'q':
			break;
		default :
			xil_printf("\n\rInvalid Selection");
			TimerDelay(500000);
		}
	}

	return 0;
}

void Menu()
{
	//xil_printf("\x1B[H"); //Set cursor to top left of terminal
	//xil_printf("\x1B[2J"); //Clear terminal
	xil_printf("\n\r");
	xil_printf("\n\r");
	xil_printf("\n\r");
	xil_printf("**************************************************\n\r");
	xil_printf("**************************************************\n\r");
	xil_printf("*               Bienvenido a PONG!!!             *\n\r");
	xil_printf("**************************************************\n\r");
	xil_printf("**************************************************\n\r");
	xil_printf("\n\r");
	xil_printf("1 - Cambiar resoluci�n VGA\n\r");
	xil_printf("2 - Cambiar resoluci�n HDMI\n\r");
	xil_printf("3 - Jugar en VGA\n\r");
	xil_printf("4 - Jugar en HDMI\n\r");
	xil_printf("q - Salir\n\r");
	xil_printf("\n\r");
	xil_printf("Se recuerda que para jugar, SW3 debe estar on\n\r");
	xil_printf("Para pausar el juego, SW3 debe estar off\n\r");
	xil_printf("SW0 cambia el color de la bola\n\r");
	xil_printf("Barra espaciadora --> pausa\n\r");
	xil_printf(" + --> aumentar velocidad");
	xil_printf(" - --> disminuir velocidad");
	xil_printf("Indique un numero:");
}

