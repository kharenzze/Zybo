/*
 * pong.c
 *
 *  Created on: 29/01/2015
 *      Author: Pablo R�ez
 *      		Vicente Cant�n
 *
 *      Este archivo incluye ficheros creados propios, y ficheros
 *      reutilizados de:
 *
 *      ZYBO Base System demonstration, Sam Bobrowicz
 *		Copyright 2014, Digilent Inc.
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

char readUART(u32 uartAddress){
	char userInput;
	/* Wait for data on UART */
	while (!XUartPs_IsReceiveData(uartAddress))
	{}

	/* Store the first character in the UART recieve FIFO and echo it */
	userInput = XUartPs_ReadReg(uartAddress, XUARTPS_FIFO_OFFSET);
	xil_printf("%c", userInput);
	return userInput;
}

void pong(DisplayCtrl *video ,u32 uartAddress, XGpio *btn, XGpio *sw){
	u32 	height = video->vMode.height,
			width = video->vMode.width,
			stride = video->stride,
			*frame,
			speed = 2,
			pause=false;
	char 	entrada;
	frame=video->framePtr[video->curFrame];

	xil_printf("\x1B[H"); //Set cursor to top left of terminal
	xil_printf("\x1B[2J"); //Clear terminal


	Rectangulo bola=crearRectangulo(10,10,RED,ANCHO_BOLA,ALTO_BOLA);
	Rectangulo palaIzquierda = crearRectangulo(0,height/2-ALTO_PALA/2,GREEN,ANCHO_PALA,ALTO_PALA);
	Rectangulo palaDerecha = crearRectangulo(width-ANCHO_PALA,height/2-ALTO_PALA/2,GREEN,ANCHO_PALA,ALTO_PALA);

	int dir_x=true, dir_y=true, indice_frame=0,salir=0,pulsar=0,puntuacionA=0,puntuacionB=0;

	salir = XGpio_DiscreteRead(sw, 1);
	while(salir & 0x8){

		if (salir & 0x1) // 0x1=0b0001=posicion de SW0
			bola.color = WHITE;
		else
			bola.color = RED;

		//apunta a al siguente frame
		indice_frame=nextFrame(video,&frame);
		pintarFondo(frame, BLACK, width,height,stride);

		//movimiento y colision
		if(dir_x == DERECHA)
			if ((bola.x+ANCHO_BOLA > palaDerecha.x) && (bola.y >= palaDerecha.y && bola.y < palaDerecha.y+ALTO_PALA )){
				bola.x-=speed;
				dir_x = IZQUIERDA;
			}else{
				bola.x+=speed;
			}
		else
			if ((bola.x < palaIzquierda.x+ANCHO_PALA) && (bola.y >= palaIzquierda.y && bola.y < palaIzquierda.y+ALTO_PALA)){
				bola.x+=speed;
				dir_x = DERECHA;
			}else{
				bola.x-=speed;
			}
		if(dir_y == ABAJO)
			bola.y+=speed;
		else
			bola.y-=speed;
		//control de direccion
		if (bola.x<0){
			bola.x=width/2;
			bola.y=height/2;
			dir_x=DERECHA;
			puntuacionB++;
			xil_printf("%d - %d\n\r",puntuacionA,puntuacionB);
			TimerDelay(1500000);
		}else if(bola.x>width - ANCHO_BOLA){
			bola.x=width/2;
			bola.y=height/2;
			dir_x=IZQUIERDA;
			puntuacionA++;
			xil_printf("%d - %d\n\r",puntuacionA,puntuacionB);
			TimerDelay(1500000);
		}
		if (bola.y<0){
			bola.y=0;
			dir_y=ABAJO;
		}else if(bola.y>height - ALTO_BOLA){
			bola.y=height - ALTO_BOLA;
			dir_y=ARRIBA;
		}

		//pintar la pelota y las palas
		pintarRectangulo(frame,&bola,width,height,stride);
		pintarRectangulo(frame,&palaDerecha,width,height,stride);
		pintarRectangulo(frame,&palaIzquierda,width,height,stride);
		//flush
		Xil_DCacheFlushRange((unsigned int) frame, DISPLAY_MAX_FRAME * 4);
		DisplayChangeFrame(video,indice_frame);
		//TimerDelay(17000);
		salir = XGpio_DiscreteRead(sw, 1);
		pulsar = XGpio_DiscreteRead(btn, 1);

		//control de las palas
		if (pulsar & 0x1){  	//Pala derecha
			if (palaDerecha.y+ALTO_PALA < height){
				palaDerecha.y+=speed;
			}
		}else if(pulsar & 0x2){
			if (palaDerecha.y >= 0){
				palaDerecha.y-=speed;
			}
		}

		if (pulsar & 0x4 ){		//Pala izquierda
			if (palaIzquierda.y+ALTO_PALA < height){
				palaIzquierda.y+=speed;
			}
		}else if(pulsar & 0x8){
			if (palaIzquierda.y >= 0){
				palaIzquierda.y-=speed;
			}
		}

		//recolocar las palas si se han salido fuera
		if (palaDerecha.y<0)
			palaDerecha.y=0;
		else if (palaDerecha.y+ALTO_PALA>height)
			palaDerecha.y=height-ALTO_PALA;
		if (palaIzquierda.y<0)
			palaIzquierda.y=0;
		else if (palaIzquierda.y+ALTO_PALA>height)
			palaIzquierda.y=height-ALTO_PALA;

		//leer pulsaciones de cambio de velocidad
		if (XUartPs_IsReceiveData(uartAddress)){
			entrada=XUartPs_ReadReg(uartAddress, XUARTPS_FIFO_OFFSET);
			if (entrada==' '){
				pause=true;
				while(pause){
					if (XUartPs_IsReceiveData(uartAddress)){
						entrada=XUartPs_ReadReg(uartAddress, XUARTPS_FIFO_OFFSET);
						if (entrada==' ')
							pause=false;
						else
							cambiarVelocidad(&speed,entrada);
					}
				}
			}else
				cambiarVelocidad(&speed,entrada);
		}
	}

}

void cambiarVelocidad(u32 *speed,char entrada){
	if(entrada=='+' && *speed < ANCHO_PALA-1){
		(*speed)++;
		xil_printf("           velocidad = %d\n\r",*speed);
	}
	else if (entrada=='-' && *speed >1){
		(*speed)--;
		xil_printf("           velocidad = %d\n\r",*speed);
	}


}

void DisplayDemoChangeRes(DisplayCtrl *dispPtr, u32 uartAddr)
{
	char userInput = 0;
	int fResSet = 0;
	int status;

	/* Flush UART FIFO */
	while (XUartPs_IsReceiveData(uartAddr))
	{
		XUartPs_ReadReg(uartAddr, XUARTPS_FIFO_OFFSET);
	}

	while (!fResSet)
	{
		DisplayDemoCRMenu(dispPtr);

		/* Wait for data on UART */
		while (!XUartPs_IsReceiveData(uartAddr))
		{}

		/* Store the first character in the UART recieve FIFO and echo it */
		userInput = XUartPs_ReadReg(uartAddr, XUARTPS_FIFO_OFFSET);
		xil_printf("%c", userInput);
		status = XST_SUCCESS;
		switch (userInput)
		{
		case '1':
			status = DisplayStop(dispPtr);
			DisplaySetMode(dispPtr, &VMODE_640x480);
			DisplayStart(dispPtr);
			fResSet = 1;
			break;
		case '2':
			status = DisplayStop(dispPtr);
			DisplaySetMode(dispPtr, &VMODE_800x600);
			DisplayStart(dispPtr);
			fResSet = 1;
			break;
		case '3':
			status = DisplayStop(dispPtr);
			DisplaySetMode(dispPtr, &VMODE_1280x720);
			DisplayStart(dispPtr);
			fResSet = 1;
			break;
		case '4':
			status = DisplayStop(dispPtr);
			DisplaySetMode(dispPtr, &VMODE_1280x1024);
			DisplayStart(dispPtr);
			fResSet = 1;
			break;
		case '5':
			status = DisplayStop(dispPtr);
			DisplaySetMode(dispPtr, &VMODE_1920x1080);
			DisplayStart(dispPtr);
			fResSet = 1;
			break;
		case 'q':
			fResSet = 1;
			break;
		default :
			xil_printf("\n\rInvalid Selection");
			TimerDelay(500000);
		}
		if (status == XST_DMA_ERROR)
		{
			xil_printf("\n\rWARNING: AXI VDMA Error detected and cleared\n\r");
		}
	}
}

void DisplayDemoCRMenu(DisplayCtrl *dispPtr)
{
	xil_printf("\x1B[H"); //Set cursor to top left of terminal
	xil_printf("\x1B[2J"); //Clear terminal
	xil_printf("**************************************************\n\r");
	xil_printf("*           ZYBO Display User Demo               *\n\r");
	xil_printf("**************************************************\n\r");
	xil_printf("*Port: %42s*\n\r", (dispPtr->fHdmi == DISPLAY_HDMI) ? "HDMI" : "VGA");
	xil_printf("*Current Resolution: %28s*\n\r", dispPtr->vMode.label);
	printf("*Pixel Clock Freq. (MHz): %23.3f*\n\r", (dispPtr->fHdmi == DISPLAY_HDMI) ? (dispPtr->pxlFreq / 5.0) : (dispPtr->pxlFreq));
	xil_printf("**************************************************\n\r");
	xil_printf("\n\r");
	xil_printf("1 - %s\n\r", VMODE_640x480.label);
	xil_printf("2 - %s\n\r", VMODE_800x600.label);
	xil_printf("3 - %s\n\r", VMODE_1280x720.label);
	xil_printf("4 - %s\n\r", VMODE_1280x1024.label);
	xil_printf("5 - %s\n\r", VMODE_1920x1080.label);
	xil_printf("q - Quit (don't change resolution)\n\r");
	xil_printf("\n\r");
	xil_printf("Select a new resolution:");
}

/**
 * Pinta toda la pantalla de un color dado
 */
void pintarFondo(u32 *frame, u32 color, u32 width, u32 height, u32 stride){
	Rectangulo r;
	r=crearRectangulo(0,0,color,width,height);
	pintarRectangulo(frame,&r,width,height,stride);
}

/**
 * Dadas las coordenadas de un pixel, devuelve la direcci�n del registro
 * en el que se encuentra
 */
u32 dir(u32 x, u32 y, u32 stride){
	u32 wStride = stride/4, direccion;
	direccion= x+y*wStride;
	return direccion;
}

int nextFrame(DisplayCtrl *dispPtr, u32 **frame){
	int nextFrame = dispPtr->curFrame + 1;
	if (nextFrame >= DISPLAY_NUM_FRAMES)
	{
		nextFrame = 0;
	}
	*frame=dispPtr->framePtr[nextFrame];
	return nextFrame;
}

void pintarRectangulo(u32 *frame, Rectangulo *r, u32 width, u32 height, u32 stride){
	u32 x,y,pixel;
	if (r->x > width || r->y > height)
		return;
	for (y=r->y; y< r->alto+r->y;y++){
		pixel=dir(r->x,y,stride);
		for (x=0; x< r->ancho;x++){
			frame[pixel]=r->color;
			pixel++;
		}
	}
}

