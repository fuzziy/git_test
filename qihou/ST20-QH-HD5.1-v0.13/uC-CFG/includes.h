/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                           MASTER INCLUDES
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                     Micrium uC-Eval-STM32F107
*                                        Evaluation Board
*
* Filename      : includes.h
* Version       : V1.00
* Programmer(s) : EHS
*********************************************************************************************************
*/

#ifndef  INCLUDES_PRESENT
#define  INCLUDES_PRESENT


/*
*********************************************************************************************************
*                                         STANDARD LIBRARIES
*********************************************************************************************************
*/

#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>


/*
*********************************************************************************************************
*                                              LIBRARIES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_def.h>
#include  <lib_ascii.h>
#include  <lib_math.h>
#include  <lib_mem.h>
#include  <lib_str.h>


/*
*********************************************************************************************************
*                                              APP / BSP
*********************************************************************************************************
*/

#include  <bsp.h>
#include "led.h"
#include "mb.h"
#include "my_modbus.h"

#include "mb2.h"
#include "mb3.h"
#include "mcp3421.h"
#include "74hc405x.h"
#include "adc.h"
#include "calculate.h"
#include "rx8025.h"
#include "24cxx.h"
#include "di.h"
#include "do.h"
#include "qihou.h"
#include "dac.h"

#include "iwdg.h"

//debug
#include "mb2rtu.h"
#include "rs485_2.h"
#include "24cxx.h"

#include "ai.h"

#include "wrappers.h"
#include "timer.h"


/*
*********************************************************************************************************
*                                                 OS
*********************************************************************************************************
*/

#include  <ucos_ii.h>


/*
*********************************************************************************************************
*                                                 ST
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                            INCLUDES END
*********************************************************************************************************
*/


#endif

