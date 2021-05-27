#include "rx8025.h"
#include "calculate.h"

#include "my_modbus.h"
#include "sim_i2c.h"


#define _NOP()    __no_operation()


static u8 rx8025t_write_data(u8 addr, u8 *buf, u8 len)
{
	u8 i;
	RX8025_IIC_Start();
	RX8025_IIC_SendByte(0x64);
	RX8025_IIC_WaitAck();
	RX8025_IIC_SendByte(addr);
	RX8025_IIC_WaitAck();

	for(i = 0; i < len; i++){
		RX8025_IIC_SendByte(buf[i]);
		RX8025_IIC_WaitAck();
	}
	RX8025_IIC_Stop();
	return 0;
}


static u8 rx8025t_read_data(u8 addr, u8 *buf, u8 len)
{
	u8 i;
	RX8025_IIC_Start();
	RX8025_IIC_SendByte(0x64);
	RX8025_IIC_WaitAck();
	RX8025_IIC_SendByte(addr);
	RX8025_IIC_WaitAck();
	RX8025_IIC_Start();
	RX8025_IIC_SendByte(0x65); //read
	RX8025_IIC_WaitAck();

	for(i = 0; i < len - 1; i++){
		buf[i] = RX8025_IIC_ReadByte(1);
	}
	buf[i] = RX8025_IIC_ReadByte(0);
	RX8025_IIC_Stop();
	return 0;
}


u8 get_rx8025t_time(void)
{
	u8 buf[8];
	if(rx8025t_read_data(0, buf, 7)){
		return 1;
	}
	usRegHoldingBuf[MB_INDEX_CLOCK_YEAR] = BCD2HEX(buf[6]);
  	usRegHoldingBuf[MB_INDEX_CLOCK_MONTH] = BCD2HEX(buf[5]);
  	usRegHoldingBuf[MB_INDEX_CLOCK_DAY] = BCD2HEX(buf[4]);
  	usRegHoldingBuf[MB_INDEX_CLOCK_HOUR] = BCD2HEX(buf[2]);
  	usRegHoldingBuf[MB_INDEX_CLOCK_MIN] = BCD2HEX(buf[1]);
  	usRegHoldingBuf[MB_INDEX_CLOCK_SEC] = BCD2HEX(buf[0]);  
        return 0;
}

u8 rx8025t_init(void)
{
	u8 val[3] = {0x40, 0x00, 0x48};
    rx8025t_write_data(0x0D, val, 3);
	return 0;
}


u8 rx8025t_set_time(rt_time_t *t)
{
	u8 rtc_str[7];
	rtc_str[0] = ((t->sec/10)<<4) | (t->sec%10);
	rtc_str[1] = ((t->min/10)<<4) | (t->min%10);
	rtc_str[2] = ((t->hour/10)<<4) | (t->hour%10);
	rtc_str[3] = t->week;
	rtc_str[4] = ((t->day/10)<<4) | (t->day%10);
	rtc_str[5] = ((t->month/10)<<4) | (t->month%10);
	rtc_str[6] = ((t->year/10)<<4) | (t->year%10);
	
	rx8025t_write_data(0, rtc_str, 7);
	return 0;
}



