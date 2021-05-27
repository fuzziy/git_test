#include "di.h"
#include "my_modbus.h"

/*--------------------------------------------------------------
 * DI1 -- PC4
 * DI2 -- PB0
 * DI3 -- PF11
 * DI4 -- PF13
 * DI5 -- PF15
 * DI6 -- PG1
 * DI7 -- PE8
 * DI8 -- PE10
 * DI9 -- PE12
 *------------------------------------------------------------*/  
void DI_Init(void) 
{ 
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|
                    RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF|
                    RCC_APB2Periph_GPIOG, ENABLE); //

    // PC4
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    // PB0
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    // PF11-13-15
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_15;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
    // PG1
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
    GPIO_Init(GPIOG, &GPIO_InitStructure);
    // PE8-10-12
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}


static u16 g_di_last2_state_temp[10] = {0};
static u16 g_di_last1_state_temp[10] = {0};
static u16 g_di_last_state_ok[10] = {0};

static u16 di_state_calc(u16 current_state, u8 chn)
{
    u16 res;

    if((g_di_last1_state_temp[chn] == g_di_last2_state_temp[chn]) &&
       (g_di_last1_state_temp[chn] == current_state)){
           g_di_last_state_ok[chn] = current_state;
           res = current_state;
    }else{
        res = g_di_last_state_ok[chn];
    }

    g_di_last2_state_temp[chn] = g_di_last1_state_temp[chn];
    g_di_last1_state_temp[chn] = current_state;
    return res;
}

static u16 get_current_di_state(u8 chn)
{
    u16 res = 0;
    switch( chn ){
    case 1:
        res = ( u16 )(!DI1_STA);
        break; 
    case 2:
        res = ( u16 )(!DI2_STA);
        break; 
    case 3:
        res = ( u16 )(!DI3_STA);
        break; 
    case 4:
        res = ( u16 )(!DI4_STA);
        break; 
    case 5:
        res = ( u16 )(!DI5_STA);
        break; 
    case 6:
        res = ( u16 )(!DI6_STA);
        break; 
    case 7:
        res = ( u16 )(!DI7_STA);
        break; 
    case 8:
        res = ( u16 )(!DI8_STA);
        break; 
    case 9:
        res = ( u16 )(!DI9_STA);
        break; 
    }
    return res;
}



void DI_Scan(void)
{	
    int i;
    u16 current_state;
    for(i = 0; i < 9; i++){
        current_state = get_current_di_state(i + 1); 
        usRegHoldingBuf[MB_INDEX_DI1 + i] = di_state_calc(current_state, i + 1);
    }
}



/*--------------------------------------------------------------
 * ADDR_S1 -- PD8
 * ADDR_S2 -- PB15
 * ADDR_S3 -- PB14
 * ADDR_S4 -- PB13
 * ADDR_S5 -- PB12
 * ADDR_S6 -- PE15
 * ADDR_S7 -- PE14
 * ADDR_S8 -- PE13
 *------------------------------------------------------------*/ 
void Addr_sw_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD|
                    RCC_APB2Periph_GPIOE, ENABLE); //

    // PD8
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;   // down 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    // PB12-13-14-15
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    // PE13-14-15
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/*--------------------------------------------------------------
 * ADDR_S1 -- PD8
 * ADDR_S2 -- PB15
 * ADDR_S3 -- PB14
 * ADDR_S4 -- PB13
 * */
u8 com1_addr_get(u8 start_addr)
{
    u8 addr = 0;
    u8 res;
    // S1 -- PD8
    res = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8);
    if(res > 0){
        addr |= 0x01;
    }
    
   
    // S2 -- PB15
    res = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);
    if(res > 0){
        addr |= (1 << 1); 
    }

    // S3 -- PB14
    res = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
    if(res > 0){
        addr |= (1 << 2); 
    }

    // S4 -- PB13
    res = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);
    if(res > 0){
        addr |= (1 << 3); 
    }

    if(addr == 0){
        return 1;
    }

    if(start_addr > 250){
        start_addr = 0;
    }
    return start_addr + addr;
}

/*--------------------------------------------------------------
 * ADDR_S5 -- PB12
 * ADDR_S6 -- PE15
 * ADDR_S7 -- PE14
 * ADDR_S8 -- PE13
 *------------------------------------------------------------*/
u8 com2_addr_get(u8 start_addr)
{
     u8 addr = 0;
    u8 res;
    // S5 -- PB12
    res = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12);
    if(res > 0){
        addr |= 0x01;
    }

    // S6 -- PE15
    res = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_15);
    if(res > 0){
        addr |= (1 << 1); 
    }

    // S7 -- PE14
    res = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_14);
    if(res > 0){
        addr |= (1 << 2); 
    }

    // S8 -- PE13
    res = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_13);
    if(res > 0){
        addr |= (1 << 3); 
    }

    if(addr == 0){
        return 1;
    }

     if(start_addr > 250){
        start_addr = 0;
    }
    
    return start_addr + addr;
}

