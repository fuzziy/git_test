#include "dac.h"



void DAC_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );	  //ʹ��PORTAͨ��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	  //ʹ��DACͨ��ʱ�� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;				 // �˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 //ģ������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);//PA.4 �����
	GPIO_SetBits(GPIOA,GPIO_Pin_5); //PA5 �����
	
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//��ʹ�ô������� TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η���
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1�������ر� BOFF1=1
    DAC_Init(DAC_Channel_1,&DAC_InitType);	 //��ʼ��DACͨ��1
    DAC_Init(DAC_Channel_2,&DAC_InitType);	 //��ʼ��DACͨ��2

	DAC_Cmd(DAC_Channel_1,ENABLE);
	DAC_Cmd(DAC_Channel_2,ENABLE);
	
	DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ
	DAC_SetChannel2Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ
	
}

const u16 DAC1_MA_D_VALUE[] = {
632, 632, 665, 681, 698, 714, 730, 747, 763, 780,		//3.9 - 4.8 mA
797, 813, 830, 846, 863, 879, 896, 913, 929, 946,		//4.9 - 5.8 mA
962, 979, 995, 1012, 1029, 1045, 1062, 1078, 1095, 1112,	//5.9 - 6.8 mA
1128, 1144, 1161, 1178, 1194, 1211, 1227, 1243, 1260, 1277,	//6.9 - 7.8 mA
1294, 1311, 1328, 1343, 1359, 1376, 1393, 1410, 1426, 1442,	//7.9 - 8.8 mA
1459, 1476, 1492, 1508, 1525, 1542, 1559, 1575, 1592, 1608,	//8.9 - 9.8 mA
1625, 1641, 1658, 1674, 1691, 1707, 1724, 1740, 1757, 1774,	//9.9 - 10.8 mA
1790, 1808, 1824, 1840, 1856, 1873, 1890, 1906, 1923, 1939,	//10.9 - 11.8 mA
1956, 1972, 1989, 2006, 2022, 2039, 2056, 2072, 2089, 2105,	//11.9 - 12.8 mA
2122, 2138, 2155, 2171, 2188, 2205, 2221, 2238, 2254, 2271,	//12.9 - 13.8 mA
2287, 2304, 2321, 2338, 2354, 2371, 2387, 2404, 2420, 2437,	//13.9 - 14.8 mA
2453, 2470, 2487, 2503, 2520, 2536, 2553, 2570, 2587, 2603,	//14.9 - 15.8 mA
2620, 2636, 2653, 2670, 2686, 2703, 2719, 2736, 2752, 2769,	//15.9 - 16.8 mA
2786, 2802, 2820, 2836, 2853, 2869, 2886, 2902, 2919, 2935,	//16.9 - 17.8 mA
2952, 2968, 2985, 3002, 3018, 3035, 3051, 3068, 3085, 3102,	//17.9 - 18.8 mA
3118, 3135, 3151, 3168, 3184, 3201, 3218, 3234, 3251, 3267,	//18.9 - 19.8 mA
3284, 3315, 3317						//19.9 - 20.1 mA
};



static u16 dac1_mA_dvalue_calc(float mA)
{
	int index,diff, res;
	int itmp1, itmp2;
	float ftmp;
	if(mA < 3.9){
		return DAC1_MA_D_VALUE[0];
	}

	if(mA > 20.1){
		return DAC1_MA_D_VALUE[162];
	}

	index = ( int )(mA*10);
	index = index - 39;

	itmp1 = ( int )(mA*10);
	itmp1 = itmp1*10;
	itmp2 = ( int )(mA*100);
	if(itmp2 > itmp1){
		diff = itmp2 - itmp1;
		ftmp = diff*1.6;
		res = DAC1_MA_D_VALUE[index] + ( int )ftmp;
	}else{
		res = DAC1_MA_D_VALUE[index];
	}
	return res;
}



#define DAC2_4_6_DVALUE_CMP_DAC1	4
#define DAC2_6_11_DVALUE_CMP_DAC1	7
#define DAC2_11_14_DVALUE_CMP_DAC1	9
#define DAC2_14_17_DVALUE_CMP_DAC1	12
#define DAC2_17_19_DVALUE_CMP_DAC1	14
#define DAC2_19_20_DVALUE_CMP_DAC1	16
static u16 dac2_mA_dvalue_calc(float mA)
{
	u16 res;
	u16 itmp;
	res = dac1_mA_dvalue_calc(mA);	
	itmp = ( u16 )(mA*10);
	if( itmp <= 60){
		return res - DAC2_4_6_DVALUE_CMP_DAC1;
	}

	if((itmp > 60) && (itmp <= 110)){
		return res - DAC2_6_11_DVALUE_CMP_DAC1;
	}

	if((itmp > 110) && (itmp <= 140)){
		return res - DAC2_11_14_DVALUE_CMP_DAC1;
	}

	if((itmp > 140) && (itmp <= 170)){
		return res - DAC2_14_17_DVALUE_CMP_DAC1;
	}

	if((itmp > 170) && (itmp <= 190)){
		return res - DAC2_17_19_DVALUE_CMP_DAC1;
	}

	if(itmp > 190){
		return res - DAC2_19_20_DVALUE_CMP_DAC1;
	}

	return 0;
}




void DAC_OutmA(u8 chn, float mA)
{    
    u16 res;
	switch ( chn ){
	case 1:
		res = dac1_mA_dvalue_calc(mA);
		DAC_SetChannel1Data(DAC_Align_12b_R, res);
		break;
	case 2:
		res = dac2_mA_dvalue_calc(mA);
		DAC_SetChannel2Data(DAC_Align_12b_R, res);
		break;
	}
}



/*
**********************************************************
*
*				DAC_OutputV
*
* �����������ѹ�����������ѹ��Χ0-10V
*
* ������voltage-Ҫ����ĵ�ѹֵ
*
* ���أ���
*
* ˵������Ӳ���汾���ο���ѹΪ2.5V��DAC�ķֱ���Ϊ12λ����
*		��ѹ�Ŵ��·�ķŴ���Ϊ5����������������Ӧ��ϵΪ
*		CPU���0-2.5V����Ӧ������Ϊ0-4095����4096�ݣ�����
*		CPU�������ѹ���Ӧ�������Ĺ�ϵΪ��
*			y = 1638.4*x������xΪ��ѹֵ��yΪ��Ӧ������
*
*		��Ӳ���Ŵ���Ϊ5��������ʵ�������ѹ��CPU�����
*		��ѹ��ϵΪ��
*			y = 0.2*x������xΪʵ�������ѹ��yΪ��ӦCPU��
*					   �����ѹ
*
*	    ����ʵ�������ѹ��DAC�Ŀ�����������ϵΪ��
*			y = 1638.4*(0.2*x),xΪʵ�ʵ�ѹֵ��yΪ��ӦDAC
*			�����������
*
************************************************************
*/
void DAC_OutV(u16 chn, float voltage)
{
  u16 res = 0;  
  res = ( u16 )(voltage*1638.4*0.2);
  if(res > 3278){  // 10V -- 2V -- 3276.8
    res = 3278;
  }
  
  switch( chn ){
  case 1:
    DAC_SetChannel1Data(DAC_Align_12b_R,res);
    break;
  case 2:
    DAC_SetChannel2Data(DAC_Align_12b_R,res);
    break;
  }
}




/*
 * ���������򵥷�ϵͳ�������
 *
 * ������ value  ���ſ������ֵ
 *		  
 *		  dac_signal_type	DAC �ź����� 0-���� 1-��ѹ
 * 
 * ע�⣺ �ڵ���ϵͳ�£����������Χ 0~100 (����)����Ӧ����
 *		  DAC1��DAC2 ͬʱ������������ͬ����ֵ
 *
 */
static void qh_single_valve_output(float value, u16 dac_signal_type)
{
	float mA, voltage;
	if(dac_signal_type == 0){ 	//��������
		// 0~100% --  4~20mA
		mA = value*0.16 + 4;
		if(mA > 20){
			mA = 20;
		}
		// �������
		DAC_OutmA(1, mA);
		DAC_OutmA(2, mA);
	}else{						// ��ѹ����
		// 0~100% --  0~10V
		voltage = value/10;
		if(voltage > 10){
			voltage = 10.0;
		}
		// �����ѹ
		DAC_OutV(1, voltage);
		DAC_OutV(2, voltage);
	}
}

/*
 * ����������˫��ϵͳ�������
 *
 * ������ value  ���ſ������ֵ
 *		  
 *		  dac_signal_type	DAC �ź����� 0-���� 1-��ѹ
 * 
 * ע�⣺ ��˫��ϵͳ�£�����������ԣ� 
 *        0~50%   ��ͨ��ȫ���� ֱͨ����Ӧ 0~100%������
 *		  50~100  ֱͨ��ȫ���� ��ͨ����Ӧ 100~0�� �ݼ�
 *
 */
static void qh_double_valve_output(float value, u16 dac_signal_type)
{
	float mA, voltage;
	if(dac_signal_type == 0){ 	//��������
		if(value < 50){
			//0~50%   ��ͨ��ȫ���� ֱͨ����Ӧ 0~100%������
			// 0~50% -- 4~20mA
			mA = value*0.32 + 4;
			DAC_OutmA(1, mA);
			DAC_OutmA(2, 20);
		}else if(value > 50){
			//50~100  ֱͨ��ȫ���� ��ͨ����Ӧ 100~0�� �ݼ�
			// 50~100% -- 20~4mA
			mA = 36 - 0.32*value;
			DAC_OutmA(1, 20);
			DAC_OutmA(2, mA);
		}else{
			DAC_OutmA(1, 20);
			DAC_OutmA(2, 20);
		}
	}else{						// ��ѹ����
		//0~50%   ��ͨ��ȫ���� ֱͨ����Ӧ 0~100%������
		if(value < 50){
			// 0~50% -- 0~10V
			voltage = value/5;
			DAC_OutV(1, voltage);
			DAC_OutV(2, 10);
		}else if(value > 50){
			// 50~100% -- 10~0V
			voltage = 20 - 0.2*value;
			DAC_OutV(1, 10);
			DAC_OutV(2, voltage);
		}else{
			DAC_OutV(1, 10);
			DAC_OutV(2, 10);
		}
	}
}

/*
 * ���������������ϵͳ�������
 *
 * ������ value  ���ſ������ֵ
 *		  
 *		  dac_signal_type	DAC �ź����� 0-���� 1-��ѹ
 * 
 * ע�⣺ �ڶ����ÿ���ϵͳ�£����������Χ 0~50(��Ƶ��Ƶ��Hz)
 *		  DAC1��DAC2 ͬʱ������������ͬ����ֵ
 *
 */
static void qh_pump_output(float value, u16 dac_signal_type)
{	
	float mA, voltage;
	if(dac_signal_type == 0){ //��������
		// 0~50Hz --  4~20mA
		mA = value*0.32 + 4;
		if(mA > 20){
			mA = 20;
		}
		// �������
		DAC_OutmA(1, mA);
		DAC_OutmA(2, mA);
	}else{	// ��ѹ����
		// 0~50Hz --  0~10V
		voltage = value/5;
		if(voltage > 10){
			voltage = 10.0;
		}
		// �����ѹ
		DAC_OutV(1, voltage);
		DAC_OutV(2, voltage);
	}

}

/*
 * ���������򵥷�ϵͳ�������
 *
 * ������ value  ���ſ������ֵ
 *
 */
void hal_dac_output(u16 actuator_type, u16 dac_signal_type, float value)
{
	switch(actuator_type){
	case ACTUATOR_TYPE_SINGLE_VALVE:
		qh_single_valve_output(value, dac_signal_type);
		break;
	case ACTUATOR_TYPE_DOUBLE_VALVE:
		qh_double_valve_output(value, dac_signal_type);
		break;
	case ACTUATOR_TYPE_PUMP:
		qh_pump_output(value, dac_signal_type);
		break;
	}

}