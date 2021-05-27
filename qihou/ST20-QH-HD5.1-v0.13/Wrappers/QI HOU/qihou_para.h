#ifndef	__PARA_H
#define __PARA_H
#include "stm32f10x.h"

#ifdef	PARA_GLOBALS
#define PARA_EXT
#else
#define PARA_EXT extern 
#endif

typedef	enum
{
	NONE=0,ODD=1,EVEN=2
} COMM_Parity;

typedef	enum
{
	Modbus=0,MBus=1,DLT645_07=2,DLT645_97=3,
}Comm_Protocol;

#define EE_FLAG_CS_H  0x5A   
#define EE_FLAG_CS_L  0xA5

#define EE_FLAG_CRC_H  0x6A   
#define EE_FLAG_CRC_L  0xA6

/************************************************
*
************************************************/
#define	EE_ADDR_CS_H	            0	// 
#define	EE_ADDR_CS_L	            1	// 

#define	EE_ADDR_SET_TEMP		2	//	2	-	set temp
#define	EE_ADDR_START_TEMP		6	//	2	-	start temp
#define	EE_ADDR_OUTDOOR_TEMP_REM		10	//	2	-	outdoor temp remote
#define	EE_ADDR_FEEL_TEMP		14	//	2	-	feeling temp
#define	EE_ADDR_FEEL_TEMP_K		18	//	2	-	feeling temp k
#define	EE_ADDR_CURVATURE1		22	//	2	-	
#define	EE_ADDR_CURVATURE2		26	//	2	-	
#define	EE_ADDR_CURVATURE3		30	//	2	-	
#define	EE_ADDR_CURVATURE4		34	//	2	-	
#define	EE_ADDR_MANUAL_OUTPUT		38	//	2	-	
#define	EE_ADDR_HOLIDAY_OFFSET		42	//	2	-	
#define	EE_ADDR_WATER_LEVEL_HL		46	//	2	-	water level high limit
#define	EE_ADDR_WATER_LEVEL_LL		50	//	2	-	water level low limit
#define	EE_ADDR_OUTPUT_LL		54	//	2	-	qi hou output low limit
#define	EE_ADDR_TARGET_TEMP_HL		58	//	2	-	target temp high limit
#define	EE_ADDR_TARGET_TEMP_LL		62	//	2	-	target temp low limit
#define	EE_ADDR_SOLAR_OFFSET		66	//	2	-	solar radiation offset
#define	EE_ADDR_NULL8		70	//	2	-	
#define	EE_ADDR_NULL9		74	//	2	-	
#define	EE_ADDR_NULL10		78	//	2	-	
#define	EE_ADDR_NULL11		82	//	2	-	
#define	EE_ADDR_NULL12		86	//	2	-	
#define	EE_ADDR_NULL13		90	//	2	-	
#define	EE_ADDR_NULL14		94	//	2	-	
#define	EE_ADDR_NULL15		98	//	2	-	
#define	EE_ADDR_MODE		102	//	1	-	qi hou ctrl mode
#define	EE_ADDR_OUTDOOR_TEMP_SW		104	//	1	-	outdoor temp switch
#define	EE_ADDR_SOLAR_SW		106	//	1	-	solar radiation switch
#define	EE_ADDR_NULL16		108	//	1	-	
#define	EE_ADDR_CLOCK_YEAR		110	//	1	-	
#define	EE_ADDR_CLOCK_MONTH		112	//	1	-	
#define	EE_ADDR_CLOCK_DAY		114	//	1	-	
#define	EE_ADDR_CLOCK_WEEK		116	//	1	-	
#define	EE_ADDR_CLOCK_HOUR		118	//	1	-	
#define	EE_ADDR_CLOCK_MIN		120	//	1	-	
#define	EE_ADDR_CLOCK_SEC		122	//	1	-	
#define	EE_ADDR_HOLIDAY_STATE		124	//	1	-	
#define	EE_ADDR_SOLAR_START_HOUR		126	//	1	-	
#define	EE_ADDR_SOLAR_START_MIN		128	//	1	-	
#define	EE_ADDR_SOLAR_END_HOUR		130	//	1	-	
#define	EE_ADDR_SOLAR_END_MIN		132	//	1	-	
#define	EE_ADDR_NULL17		134	//	1	-	
#define	EE_ADDR_NULL18		136	//	1	-	
#define	EE_ADDR_NULL19		138	//	1	-	
#define	EE_ADDR_NULL20		140	//	1	-	
#define	EE_ADDR_DO1		142	//	1	-	
#define	EE_ADDR_DO2		144	//	1	-	
#define	EE_ADDR_DO3		146	//	1	-	
#define	EE_ADDR_DO4		148	//	1	-	
#define	EE_ADDR_DO5		150	//	1	-	
#define	EE_ADDR_NULL21		152	//	1	-	
#define	EE_ADDR_NULL22		154	//	1	-	
#define	EE_ADDR_NULL23		156	//	1	-	
#define	EE_ADDR_NULL24		158	//	1	-	
#define	EE_ADDR_NULL25		160	//	1	-	
#define	EE_ADDR_NULL26		162	//	1	-	
#define	EE_ADDR_NULL27		164	//	1	-	
#define	EE_ADDR_NULL28		166	//	1	-	
#define	EE_ADDR_NULL29		168	//	1	-	
#define	EE_ADDR_NULL30		170	//	1	-	
#define	EE_ADDR_NULL31		172	//	1	-	
#define	EE_ADDR_NULL32		174	//	1	-	
#define	EE_ADDR_NULL33		176	//	1	-	
#define	EE_ADDR_NULL34		178	//	1	-	
#define	EE_ADDR_NULL35		180	//	1	-	
#define	EE_ADDR_SEG1_START_HOUR		182	//	1	-	
#define	EE_ADDR_SEG1_START_MIN		184	//	1	-	
#define	EE_ADDR_SEG1_END_HOUR		186	//	1	-	
#define	EE_ADDR_SEG1_END_MIN		188	//	1	-	
#define	EE_ADDR_SEG2_START_HOUR		190	//	1	-	
#define	EE_ADDR_SEG2_START_MIN		192	//	1	-	
#define	EE_ADDR_SEG2_END_HOUR		194	//	1	-	
#define	EE_ADDR_SEG2_END_MIN		196	//	1	-	
#define	EE_ADDR_SEG3_START_HOUR		198	//	1	-	
#define	EE_ADDR_SEG3_START_MIN		200	//	1	-	
#define	EE_ADDR_SEG3_END_HOUR		202	//	1	-	
#define	EE_ADDR_SEG3_END_MIN		204	//	1	-	
#define	EE_ADDR_SEG4_START_HOUR		206	//	1	-	
#define	EE_ADDR_SEG4_START_MIN		208	//	1	-	
#define	EE_ADDR_SEG4_END_HOUR		210	//	1	-	
#define	EE_ADDR_SEG4_END_MIN		212	//	1	-	
#define	EE_ADDR_SEG5_START_HOUR		214	//	1	-	
#define	EE_ADDR_SEG5_START_MIN		216	//	1	-	
#define	EE_ADDR_SEG5_END_HOUR		218	//	1	-	
#define	EE_ADDR_SEG5_END_MIN		220	//	1	-	
#define	EE_ADDR_SEG6_START_HOUR		222	//	1	-	
#define	EE_ADDR_SEG6_START_MIN		224	//	1	-	
#define	EE_ADDR_SEG6_END_HOUR		226	//	1	-	
#define	EE_ADDR_SEG6_END_MIN		228	//	1	-	
#define	EE_ADDR_SEG7_START_HOUR		230	//	1	-	
#define	EE_ADDR_SEG7_START_MIN		232	//	1	-	
#define	EE_ADDR_SEG7_END_HOUR		234	//	1	-	
#define	EE_ADDR_SEG7_END_MIN		236	//	1	-	
#define	EE_ADDR_SEG8_START_HOUR		238	//	1	-	
#define	EE_ADDR_SEG8_START_MIN		240	//	1	-	
#define	EE_ADDR_SEG8_END_HOUR		242	//	1	-	
#define	EE_ADDR_SEG8_END_MIN		244	//	1	-	
#define	EE_ADDR_SEG1_SW		246	//	1	-	
#define	EE_ADDR_SEG2_SW		248	//	1	-	
#define	EE_ADDR_SEG3_SW		250	//	1	-	
#define	EE_ADDR_SEG4_SW		252	//	1	-	
#define	EE_ADDR_SEG5_SW		254	//	1	-	
#define	EE_ADDR_SEG6_SW		256	//	1	-	
#define	EE_ADDR_SEG7_SW		258	//	1	-	
#define	EE_ADDR_SEG8_SW		260	//	1	-	
#define	EE_ADDR_SEG1_OFFSET		262	//	2	-	
#define	EE_ADDR_SEG2_OFFSET		266	//	2	-	
#define	EE_ADDR_SEG3_OFFSET		270	//	2	-	
#define	EE_ADDR_SEG4_OFFSET		274	//	2	-	
#define	EE_ADDR_SEG5_OFFSET		278	//	2	-	
#define	EE_ADDR_SEG6_OFFSET		282	//	2	-	
#define	EE_ADDR_SEG7_OFFSET		286	//	2	-	
#define	EE_ADDR_SEG8_OFFSET		290	//	2	-	
#define	EE_ADDR_NULL36		294	//	2	-	
#define	EE_ADDR_NULL37		298	//	2	-	
#define	EE_ADDR_NULL38		302	//	2	-	
#define	EE_ADDR_NULL39		306	//	2	-	
#define	EE_ADDR_NULL40		310	//	2	-	
#define	EE_ADDR_NULL41		314	//	2	-	
#define	EE_ADDR_NULL42		318	//	2	-	
#define	EE_ADDR_N40_TEMP		322	//	2	-	-40 target temp
#define	EE_ADDR_N39_TEMP		326	//	2	-	-39 target temp
#define	EE_ADDR_N38_TEMP		330	//	2	-	-38 target temp
#define	EE_ADDR_N37_TEMP		334	//	2	-	-37 target temp
#define	EE_ADDR_N36_TEMP		338	//	2	-	-36 target temp
#define	EE_ADDR_N35_TEMP		342	//	2	-	-35 target temp
#define	EE_ADDR_N34_TEMP		346	//	2	-	-34 target temp
#define	EE_ADDR_N33_TEMP		350	//	2	-	-33 target temp
#define	EE_ADDR_N32_TEMP		354	//	2	-	-32 target temp
#define	EE_ADDR_N31_TEMP		358	//	2	-	-31 target temp
#define	EE_ADDR_N30_TEMP		362	//	2	-	-30 target temp
#define	EE_ADDR_N29_TEMP		366	//	2	-	-29 target temp
#define	EE_ADDR_N28_TEMP		370	//	2	-	-28 target temp
#define	EE_ADDR_N27_TEMP		374	//	2	-	-27 target temp
#define	EE_ADDR_N26_TEMP		378	//	2	-	-26 target temp
#define	EE_ADDR_N25_TEMP		382	//	2	-	-25 target temp
#define	EE_ADDR_N24_TEMP		386	//	2	-	-24 target temp
#define	EE_ADDR_N23_TEMP		390	//	2	-	-23 target temp
#define	EE_ADDR_N22_TEMP		394	//	2	-	-22 target temp
#define	EE_ADDR_N21_TEMP		398	//	2	-	-21 target temp
#define	EE_ADDR_N20_TEMP		402	//	2	-	-20 target temp
#define	EE_ADDR_N19_TEMP		406	//	2	-	-19 target temp
#define	EE_ADDR_N18_TEMP		410	//	2	-	-18 target temp
#define	EE_ADDR_N17_TEMP		414	//	2	-	-17 target temp
#define	EE_ADDR_N16_TEMP		418	//	2	-	-16 target temp
#define	EE_ADDR_N15_TEMP		422	//	2	-	-15 target temp
#define	EE_ADDR_N14_TEMP		426	//	2	-	-14 target temp
#define	EE_ADDR_N13_TEMP		430	//	2	-	-13 target temp
#define	EE_ADDR_N12_TEMP		434	//	2	-	-12 target temp
#define	EE_ADDR_N11_TEMP		438	//	2	-	-11 target temp
#define	EE_ADDR_N10_TEMP		442	//	2	-	-10 target temp
#define	EE_ADDR_N9_TEMP		446	//	2	-	-9 target temp
#define	EE_ADDR_N8_TEMP		450	//	2	-	-8 target temp
#define	EE_ADDR_N7_TEMP		454	//	2	-	-7 target temp
#define	EE_ADDR_N6_TEMP		458	//	2	-	-6 target temp
#define	EE_ADDR_N5_TEMP		462	//	2	-	-5 target temp
#define	EE_ADDR_N4_TEMP		466	//	2	-	-4 target temp
#define	EE_ADDR_N3_TEMP		470	//	2	-	-3 target temp
#define	EE_ADDR_N2_TEMP		474	//	2	-	-2 target temp
#define	EE_ADDR_N1_TEMP		478	//	2	-	-1 target temp
#define	EE_ADDR_0_TEMP		482	//	2	-	0 target temp
#define	EE_ADDR_P1_TEMP		486	//	2	-	1 target temp
#define	EE_ADDR_P2_TEMP		490	//	2	-	2 target temp
#define	EE_ADDR_P3_TEMP		494	//	2	-	3 target temp
#define	EE_ADDR_P4_TEMP		498	//	2	-	4 target temp
#define	EE_ADDR_P5_TEMP		502	//	2	-	5 target temp
#define	EE_ADDR_P6_TEMP		506	//	2	-	6 target temp
#define	EE_ADDR_P7_TEMP		510	//	2	-	7 target temp
#define	EE_ADDR_P8_TEMP		514	//	2	-	8 target temp
#define	EE_ADDR_P9_TEMP		518	//	2	-	9 target temp
#define	EE_ADDR_P10_TEMP		522	//	2	-	10 target temp
#define	EE_ADDR_P11_TEMP		526	//	2	-	11 target temp
#define	EE_ADDR_P12_TEMP		530	//	2	-	12 target temp
#define	EE_ADDR_P13_TEMP		534	//	2	-	13 target temp
#define	EE_ADDR_P14_TEMP		538	//	2	-	14 target temp
#define	EE_ADDR_P15_TEMP		542	//	2	-	15 target temp
#define	EE_ADDR_NULL43		546	//	2	-	
#define	EE_ADDR_NULL44		550	//	2	-	
#define	EE_ADDR_NULL45		554	//	2	-	
#define	EE_ADDR_NULL46		558	//	2	-	
#define	EE_ADDR_NULL47		562	//	2	-	
#define	EE_ADDR_NULL48		566	//	2	-	
#define	EE_ADDR_NULL49		570	//	2	-	
#define	EE_ADDR_NULL50		574	//	2	-	
#define	EE_ADDR_NULL51		578	//	2	-	
#define	EE_ADDR_NULL52		582	//	2	-	
#define	EE_ADDR_NULL53		586	//	2	-	
#define	EE_ADDR_NULL54		590	//	2	-	
#define	EE_ADDR_NULL55		594	//	2	-	
#define	EE_ADDR_NULL56		598	//	2	-	
#define	EE_ADDR_NULL57		602	//	2	-	
#define	EE_ADDR_NULL58		606	//	2	-	
#define	EE_ADDR_NULL59		610	//	2	-	
#define	EE_ADDR_NULL60		614	//	2	-	
#define	EE_ADDR_NULL61		618	//	2	-	
#define	EE_ADDR_ACTUATOR_TYPE		622	//	1	-	
#define	EE_ADDR_AO_SIGNAL_TYPE		624	//	1	-	4~20mA / 0-10V
#define	EE_ADDR_QH_FEEDBACK_CHN		626	//	1	-	
#define	EE_ADDR_OUTDOOR_TEMP_LOC_CHN		628	//	1	-	
#define	EE_ADDR_WATER_LEVEL_CHN		630	//	1	-	
#define	EE_ADDR_WATER_CTRL_CHN		632	//	1	-	
#define	EE_ADDR_ADJUST_TIME		634	//	1	-	auto ctrl adjust cycle
#define	EE_ADDR_AI1_TYPE		636	//	1	-	
#define	EE_ADDR_AI2_TYPE		638	//	1	-	
#define	EE_ADDR_AI3_TYPE		640	//	1	-	
#define	EE_ADDR_AI4_TYPE		642	//	1	-	
#define	EE_ADDR_AI5_TYPE		644	//	1	-	
#define	EE_ADDR_AI6_TYPE		646	//	1	-	
#define	EE_ADDR_AI7_TYPE		648	//	1	-	
#define	EE_ADDR_AI8_TYPE		650	//	1	-	
#define	EE_ADDR_COM1_START_ID		652	//	1	-	
#define	EE_ADDR_COM2_START_ID		654	//	1	-	
#define	EE_ADDR_COM1_BAUD		656	//	1	-	
#define	EE_ADDR_COM1_DATABITS		658	//	1	-	
#define	EE_ADDR_COM1_PARITY		660	//	1	-	
#define	EE_ADDR_COM2_BAUD		662	//	1	-	
#define	EE_ADDR_COM2_DATABITS		664	//	1	-	
#define	EE_ADDR_COM2_PARITY		666	//	1	-	
#define	EE_ADDR_NULL62		668	//	1	-	
#define	EE_ADDR_NULL63		670	//	1	-	
#define	EE_ADDR_NULL64		672	//	1	-	
#define	EE_ADDR_NULL65		674	//	1	-	
#define	EE_ADDR_NULL66		676	//	1	-	
#define	EE_ADDR_NULL67		678	//	1	-	
#define	EE_ADDR_NULL68		680	//	1	-	
#define	EE_ADDR_NULL69		682	//	1	-	
#define	EE_ADDR_NULL70		684	//	1	-	
#define	EE_ADDR_NULL71		686	//	1	-	
#define	EE_ADDR_NULL72		688	//	1	-	
#define	EE_ADDR_NULL73		690	//	1	-	
#define	EE_ADDR_NULL74		692	//	1	-	
#define	EE_ADDR_NULL75		694	//	1	-	
#define	EE_ADDR_NULL76		696	//	1	-	
#define	EE_ADDR_NULL77		698	//	1	-	
#define	EE_ADDR_NULL78		700	//	1	-	
#define	EE_ADDR_ERR_SEG1		702	//	2	-	
#define	EE_ADDR_ERR_SEG2		706	//	2	-	
#define	EE_ADDR_ERR_SEG3		710	//	2	-	
#define	EE_ADDR_ERR_SEG4		714	//	2	-	
#define	EE_ADDR_ERR_CHANGE		718	//	2	-	
#define	EE_ADDR_ERR_SEG1_OUTPUT		722	//	2	-	
#define	EE_ADDR_ERR_SEG2_OUTPUT		726	//	2	-	
#define	EE_ADDR_ERR_SEG3_OUTPUT		730	//	2	-	
#define	EE_ADDR_ERR_SEG4_OUTPUT		734	//	2	-	
#define	EE_ADDR_NULL79		738	//	2	-	
#define	EE_ADDR_NULL80		742	//	2	-	
#define	EE_ADDR_NULL81		746	//	2	-	
#define	EE_ADDR_NULL82		750	//	2	-	
#define	EE_ADDR_NULL83		754	//	2	-	
#define	EE_ADDR_NULL84		758	//	2	-	
#define	EE_ADDR_NULL85		762	//	2	-	
#define	EE_ADDR_NULL86		766	//	2	-	
#define	EE_ADDR_NULL87		770	//	2	-	
#define	EE_ADDR_NULL88		774	//	2	-	
#define	EE_ADDR_NULL89		778	//	2	-	
#define	EE_ADDR_AI1_RANGE_L		782	//	2	-	
#define	EE_ADDR_AI1_RANGE_H		786	//	2	-	
#define	EE_ADDR_AI2_RANGE_L		790	//	2	-	
#define	EE_ADDR_AI2_RANGE_H		794	//	2	-	
#define	EE_ADDR_AI3_RANGE_L		798	//	2	-	
#define	EE_ADDR_AI3_RANGE_H		802	//	2	-	
#define	EE_ADDR_AI4_RANGE_L		806	//	2	-	
#define	EE_ADDR_AI4_RANGE_H		810	//	2	-	
#define	EE_ADDR_AI5_RANGE_L		814	//	2	-	
#define	EE_ADDR_AI5_RANGE_H		818	//	2	-	
#define	EE_ADDR_AI6_RANGE_L		822	//	2	-	
#define	EE_ADDR_AI6_RANGE_H		826	//	2	-	
#define	EE_ADDR_AI7_RANGE_L		830	//	2	-	
#define	EE_ADDR_AI7_RANGE_H		834	//	2	-	
#define	EE_ADDR_AI8_RANGE_L		838	//	2	-	
#define	EE_ADDR_AI8_RANGE_H		842	//	2	-	
#define	EE_ADDR_NULL90		846	//	2	-	
#define	EE_ADDR_NULL91		850	//	2	-	
#define	EE_ADDR_AI1_ZERO_FAC		854	//	2	-	
#define	EE_ADDR_AI2_ZERO_FAC		858	//	2	-	
#define	EE_ADDR_AI3_ZERO_FAC		862	//	2	-	
#define	EE_ADDR_AI4_ZERO_FAC		866	//	2	-	
#define	EE_ADDR_AI5_ZERO_FAC		870	//	2	-	
#define	EE_ADDR_AI6_ZERO_FAC		874	//	2	-	
#define	EE_ADDR_AI7_ZERO_FAC		878	//	2	-	
#define	EE_ADDR_AI8_ZERO_FAC		882	//	2	-	
#define	EE_ADDR_AI1_FULL_FAC		886	//	2	-	
#define	EE_ADDR_AI2_FULL_FAC		890	//	2	-	
#define	EE_ADDR_AI3_FULL_FAC		894	//	2	-	
#define	EE_ADDR_AI4_FULL_FAC		898	//	2	-	
#define	EE_ADDR_AI5_FULL_FAC		902	//	2	-	
#define	EE_ADDR_AI6_FULL_FAC		906	//	2	-	
#define	EE_ADDR_AI7_FULL_FAC		910	//	2	-	
#define	EE_ADDR_AI8_FULL_FAC		914	//	2	-	
#define	EE_ADDR_AI1_FILTER_FAC		918	//	2	-	
#define	EE_ADDR_AI2_FILTER_FAC		922	//	2	-	
#define	EE_ADDR_AI3_FILTER_FAC		926	//	2	-	
#define	EE_ADDR_AI4_FILTER_FAC		930	//	2	-	
#define	EE_ADDR_AI5_FILTER_FAC		934	//	2	-	
#define	EE_ADDR_AI6_FILTER_FAC		938	//	2	-	
#define	EE_ADDR_AI7_FILTER_FAC		942	//	2	-	
#define	EE_ADDR_AI8_FILTER_FAC		946	//	2	-	
//define  EE_XX                 	947     //     add
//define  EE_XX                 	948     //      add
//define  EE_XX                 	949     //      add

#define EE_ADDR_CRC_H               950
#define EE_ADDR_CRC_L               951

#define EE_PARA_NBYTES              952     //


//
void qh_para_init(void);


#endif

