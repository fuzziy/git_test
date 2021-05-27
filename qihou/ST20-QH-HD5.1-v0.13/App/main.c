/*
*********************************************************************************************************
*                                              STM32F103ZE-uC/OS-V2.92
*
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static   OS_STK      AppTaskStartStk[APP_TASK_START_STK_SIZE];

static   OS_STK      AppTask1Stk[APP_TASK1_STK_SIZE];
static   OS_STK      AppTask2Stk[APP_TASK2_STK_SIZE];
//static   OS_STK      AppTask3Stk[APP_TASK3_STK_SIZE];

static   OS_STK      AppSamplingStk[APP_SAMPLING_STK_SIZE];
static   OS_STK      AppMainCtrlStk[APP_MAIN_CTRL_STK_SIZE];

static   volatile u32 OSTime_PV;       //褰撳墠绯荤粺鏃堕棿
/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/



static  void    AppTaskCreate  (void);
//static	void	AppEventCreate (void);

static  void    AppTaskStart   (void *p_arg);

static	void	AppTask1(void *p_arg);
static	void	AppTask2(void *p_arg);
//static	void	AppTask3(void *p_arg);
//static	void	AppMainCtrl(void *p_arg);

static	void	AppSampling(void *p_arg);    



/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

int  main (void)
{
    CPU_INT08U  err;


    BSP_IntDisAll();                                            /* Disable all interrupts until we are ready to accept them */

    OSInit();                                                   /* Initialize "uC/OS-II, The Real-Time Kernel"              */

    OSTaskCreateExt(AppTaskStart,                               /* Create the start task                                    */
                    (void *)0,
                    (OS_STK *)&AppTaskStartStk[APP_TASK_START_STK_SIZE - 1],
                    APP_TASK_START_PRIO,
                    APP_TASK_START_PRIO,
                    (OS_STK *)&AppTaskStartStk[0],
                    APP_TASK_START_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_TASK_START_PRIO, "Start Task", &err);
#endif

    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II)       */
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    CPU_INT32U  hclk_freq;
    CPU_INT32U  cnts;


   (void)p_arg;

    CPU_Init();                                                 /* Init CPU name & int. dis. time measuring fncts.          */

    hclk_freq = BSP_CPU_ClkFreq();                              /* Determine SysTick reference freq.                        */
    cnts  = hclk_freq / (CPU_INT32U)OS_TICKS_PER_SEC;           /* Determine nbr SysTick increments in OS_TICKS_PER_SEC.    */
    OS_CPU_SysTickInit(cnts);                                   /* Init uC/OS periodic time src (SysTick).                  */

    Mem_Init();                                                 /* Init Memory Management Module.                           */

    
     BSP_Init();                                                 /* Init BSP fncts.                                          */

    // 硬件看门狗喂狗
    sp706s_feed();
    

#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                               /* Determine CPU capacity                                   */
#endif


    //AppEventCreate();                                           /* Create Application Kernel objects                        */

    //APP_TRACE_INFO(("Creating Application Tasks...\n\r"));
    AppTaskCreate();                                            /* Create application tasks                                 */

  
    OSTaskSuspend(APP_TASK_START_PRIO);
}


/*
*********************************************************************************************************
*                                      CREATE APPLICATION TASKS
*
* Description:  This function creates the application tasks.
*
* Arguments  :  none
*
* Returns    :  none
*********************************************************************************************************
*/

static  void  AppTaskCreate (void)
{
  OS_CPU_SR  cpu_sr = 0u;
  OS_ENTER_CRITICAL();

  // 硬件看门狗喂狗
  sp706s_feed();

  wrapper_main_task_init();

  
  OSTaskCreateExt(AppTask1,
		  (void *)0,
		  (OS_STK *)&AppTask1Stk[APP_TASK1_STK_SIZE - 1],
		  APP_TASK1_PRIO,
		  APP_TASK1_PRIO,
		  (OS_STK *)&AppTask1Stk[0],
		  APP_TASK1_STK_SIZE,
		  (void *)0,
		  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
									
  OSTaskCreateExt(AppTask2,
		  (void *)0,
		  (OS_STK *)&AppTask2Stk[APP_TASK2_STK_SIZE - 1],
		  APP_TASK2_PRIO,
		  APP_TASK2_PRIO,
		  (OS_STK *)&AppTask2Stk[0],
		  APP_TASK2_STK_SIZE,
		  (void *)0,
		  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
   /*
   OSTaskCreateExt(AppTask3,
		  (void *)0,
		  (OS_STK *)&AppTask3Stk[APP_TASK3_STK_SIZE - 1],
		  APP_TASK3_PRIO,
		  APP_TASK3_PRIO,
		  (OS_STK *)&AppTask3Stk[0],
		  APP_TASK3_STK_SIZE,
		  (void *)0,
		  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
   */
   OSTaskCreateExt(AppSampling,
		  (void *)0,
		  (OS_STK *)&AppSamplingStk[APP_SAMPLING_STK_SIZE - 1],
		  APP_SAMPLING_PRIO,
		  APP_SAMPLING_PRIO,
		  (OS_STK *)&AppSamplingStk[0],
		  APP_SAMPLING_STK_SIZE,
		  (void *)0,
		  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
   
    OSTaskCreateExt(AppMainCtrl,
		  (void *)0,
		  (OS_STK *)&AppMainCtrlStk[APP_MAIN_CTRL_STK_SIZE - 1],
		  APP_MAIN_CTRL_PRIO,
		  APP_MAIN_CTRL_PRIO,
		  (OS_STK *)&AppMainCtrlStk[0],
		  APP_MAIN_CTRL_STK_SIZE,
		  (void *)0,
		  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

   							
  // 硬件看门狗喂狗
  sp706s_feed();
  
  
  OS_EXIT_CRITICAL();
}



/*
*********************************************************************************************************
*                                          AppEventCreate()
*
* Description : Create application kernel objects tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/
 
static	void	AppTask1(void *p_arg)
{ 
  
  // 硬件看门狗喂狗
  sp706s_feed();
  	
  while(1)
  {
    (void)eMBPoll();
    // feed dog
    feed_hd_soft_dog();
   
    OSTimeDlyHMSM(0, 0, 0, 10);
  }
}


static	void	AppTask2(void *p_arg)
{
  while(1)
  {
    eMB2Poll();
    // feed dog
    feed_hd_soft_dog();

    DO_Ioctl(1, usRegHoldingBuf[MB_INDEX_DO1]);
    DO_Ioctl(2, usRegHoldingBuf[MB_INDEX_DO3]);
    DO_Ioctl(3, usRegHoldingBuf[MB_INDEX_DO4]);
    DO_Ioctl(4, usRegHoldingBuf[MB_INDEX_DO4]);
    DO_Ioctl(5, usRegHoldingBuf[MB_INDEX_DO5]);
    
    OSTimeDlyHMSM(0, 0, 0, 10);
  }
}

/*
static	void	AppTask3(void *p_arg)
{  
  u8 cnt = 0;
  u8 cnt1 = 0;
  while(1){ 
    
    eMB3Poll();
    
    feed_hd_soft_dog();
    
    usRegHoldingBuf[36] = ( u16 )get_running_secs();
    if(cnt1++ > 30){
      cnt1 = 0;
      os_running_led_light();
    }
  
    if(cnt++ >= 10){
      cnt = 0;
      get_rx8025t_time();
    }

    OSTimeDlyHMSM(0, 0, 0, 10);
  }
}
*/


/*
* AI channel -- 74HC4051 channel
*       AI1 -- Y0
*       AI2 -- Y1
*       AI3 -- Y2
*       AI4 -- Y3
*       AI5 -- Y4
*       AI6 -- Y5
*       AI7 -- Y6
*       AI8 -- Y7
*     
*/
#define HC4051_HZ        12
static  void  AppSampling(void *p_arg)
{
  u8 cnt = 0;
  u8 led_cnt = 0;
  u8 ai_chn = 1;
  // 4051 init open 
  HC4051_Chn(0);
  while(1){
    //DI Scan
    DI_Scan();
    // feed dog
    feed_hd_soft_dog();
    // AI sampling
    if(cnt++ >= HC4051_HZ){
      cnt = 0;

      get_AI_variable_result( ai_chn );
      ai_chn++;
      if(ai_chn > 8){
        ai_chn = 1;
      }
      // switch HC4051 chn
      HC4051_Chn(ai_chn - 1);
    }
    //run led
    if(led_cnt++ > 25){
      led_cnt = 0;
      get_rx8025t_time();
      os_running_led_light();
    }
    
    OSTimeDlyHMSM(0, 0, 0, 10);
  }
}