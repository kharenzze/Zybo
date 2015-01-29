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

/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

/*
 * Display Driver structs
 */
DisplayCtrl hdmiCtrl;
DisplayCtrl vgaCtrl;

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

void MainDemoPrintMenu();
char readUART();

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
	TimerInitialize(SCU_TIMER_ID);

	/* Flush UART FIFO */
	while (XUartPs_IsReceiveData(UART_BASEADDR))
	{
		XUartPs_ReadReg(UART_BASEADDR, XUARTPS_FIFO_OFFSET);
	}

	while (userInput != 'q')
	{
		MainDemoPrintMenu();

		//lee una entrada de UART, y la muestra por el terminal.
		userInput=readUART();

		switch (userInput)
		{
		case '1':
			AudioRunDemo(AUDIO_CTRL_BASEADDR, UART_BASEADDR, SW_BASEADDR, BTN_BASEADDR);
			break;
		case '2':
			DisplayDemoRun(&vgaCtrl, UART_BASEADDR);
			break;
		case '3':
			DisplayDemoRun(&hdmiCtrl, UART_BASEADDR);
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
	xil_printf("\x1B[H"); //Set cursor to top left of terminal
	xil_printf("\x1B[2J"); //Clear terminal
	xil_printf("**************************************************\n\r");
	xil_printf("**************************************************\n\r");
	xil_printf("*       		 Bienvenido a PONG!!!		     *\n\r");
	xil_printf("**************************************************\n\r");
	xil_printf("**************************************************\n\r");
	xil_printf("\n\r");
	xil_printf("1 - Cambiar resoluci�n");
	xil_printf("2 - VGA\n\r");
	xil_printf("3 - HDMI\n\r");
	xil_printf("q - Quit\n\r");
	xil_printf("\n\r");
	xil_printf("Select a demo to run:");
}

char readUART(){
	char userInput;
	/* Wait for data on UART */
	while (!XUartPs_IsReceiveData(UART_BASEADDR))
	{}

	/* Store the first character in the UART recieve FIFO and echo it */
	userInput = XUartPs_ReadReg(UART_BASEADDR, XUARTPS_FIFO_OFFSET);
	xil_printf("%c", userInput);
	return userInput;
}
