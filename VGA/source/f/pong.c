/*
 * pong.c
 *
 *  Created on: 29/01/2015
 *      Author: paolo
 */


/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include "pong.h"
#include <stdio.h>
#include "xuartps.h"
#include "math.h"
#include <ctype.h>
#include <stdlib.h>
#include "xil_types.h"
#include "xil_cache.h"
#include "timer_ps.h"
#include "display_ctrl.h"


int PongDisplayInitialize(DisplayCtrl *dispPtr, u16 vdmaId, u16 timerId, u32 dispCtrlAddr, int fHdmi, u32 *framePtr[DISPLAY_NUM_FRAMES])
{
	int Status;

	TimerInitialize(timerId);

	Status = DisplayInitialize(dispPtr, vdmaId, dispCtrlAddr, fHdmi, framePtr, DISPLAY_STRIDE);
	if (Status != XST_SUCCESS)
	{
		xil_printf("Display Ctrl initialization failed during demo initialization%d\r\n", Status);
		return XST_FAILURE;
	}

	Status = DisplayStart(dispPtr);
	if (Status != XST_SUCCESS)
	{
		xil_printf("Couldn't start display during demo initialization%d\r\n", Status);
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

Rectangulo crearRectangulo(int x, int y, u32 color, u32 ancho, u32 alto){
	Rectangulo r;
	r.x=x;
	r.y=y;
	r.color=color;
	r.ancho=ancho;
	r.alto=alto;
	return r;
}

char readUART(u32 uartAdress){
	char userInput;
	/* Wait for data on UART */
	while (!XUartPs_IsReceiveData(uartAdress))
	{}

	/* Store the first character in the UART recieve FIFO and echo it */
	userInput = XUartPs_ReadReg(uartAdress, XUARTPS_FIFO_OFFSET);
	xil_printf("%c", userInput);
	return userInput;
}

void pong(DisplayCtrl *video ,u32 uartAddress, u32 botenesAddress){

}
