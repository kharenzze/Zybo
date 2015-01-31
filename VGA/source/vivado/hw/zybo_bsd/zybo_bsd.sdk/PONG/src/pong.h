/*
 * pong.h
 *
 *  Created on: 29/01/2015
 *      Author: paolo
 */

#ifndef PONG_H_
#define PONG_H_

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */
#include "xil_types.h"
#include "display_ctrl.h"
#include "xgpio.h"

/* ------------------------------------------------------------ */
/*							DEFINES								*/
/* ------------------------------------------------------------ */

#define RED 0x00FF0000
#define GREEN 0x0000FF00
#define BLUE 0x000000FF
#define BLACK 0x00000000
#define WHITE 0x00FFFFFF
#define YELLOW 0x00FFFF00
#define ALTO_PALA 70
#define ANCHO_PALA 10
#define ANCHO_BOLA 10
#define ALTO_BOLA 10
#define DERECHA 1
#define IZQUIERDA 0
#define ABAJO 1
#define ARRIBA 0
#define true 1
#define false 0

#define DISPLAY_MAX_FRAME (1920*1080)
#define DISPLAY_STRIDE (1920 * 4)

/* ------------------------------------------------------------ */
/*							STRUCT RECTANGULO					*/
/* ------------------------------------------------------------ */
typedef struct{
	int x;
	int y;
	u32 color;
	u32 ancho;
	u32 alto;
}Rectangulo;


/* ------------------------------------------------------------ */
/*							headers								*/
/* ------------------------------------------------------------ */
int PongDisplayInitialize(DisplayCtrl *dispPtr, u16 vdmaId, u16 timerId, u32 dispCtrlAddr, int fHdmi, u32 *framePtr[DISPLAY_NUM_FRAMES]);
Rectangulo crearRectangulo(int x, int y, u32 color, u32 ancho, u32 alto);
char readUART(u32 uartAdress);
void pong(DisplayCtrl *video ,u32 uartAddress, XGpio *btn, XGpio *sw);
void DisplayDemoCRMenu(DisplayCtrl *dispPtr);
void DisplayDemoChangeRes(DisplayCtrl *dispPtr, u32 uartAddr);
int nextFrame(DisplayCtrl *dispPtr, u32 **frame);
u32 dir(u32 x, u32 y, u32 stride);
void pintarRectangulo(u32 *frame, Rectangulo *r, u32 width, u32 height, u32 stride);
void pintarFondo(u32 *frame, u32 color, u32 width, u32 height, u32 stride);

#endif /* PONG_H_ */
