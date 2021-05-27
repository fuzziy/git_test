#ifndef __QH_MODBUS_H
#define __QH_MODBUS_H
#include "stm32f10x.h"




/*--------------------------0x4 regs --------------------------------------*/
#define	MB_INDEX_AI1		0	//	2	-	
#define	MB_INDEX_AI2		2	//	2	-	
#define	MB_INDEX_AI3		4	//	2	-	
#define	MB_INDEX_AI4		6	//	2	-	
#define	MB_INDEX_AI5		8	//	2	-	
#define	MB_INDEX_AI6		10	//	2	-	
#define	MB_INDEX_AI7		12	//	2	-	
#define	MB_INDEX_AI8		14	//	2	-	
#define	MB_INDEX_QH_FEEDBACK		16	//	2	-	qi hou feedback value
#define	MB_INDEX_WATER_LEVEL		18	//	2	-	water level
#define	MB_INDEX_OUTDOOR_TEMP		20	//	2	-	the end outdoor temp
#define	MB_INDEX_TARGET_TEMP		22	//	2	-	target temp
#define	MB_INDEX_OUTDOOR_TEMP_LOC		24	//	2	-	outdoor temp local
#define	MB_INDEX_OUTPUT		26	//	2	-	qi hou output
#define	MB_INDEX_NULL1		28	//	2	-	
#define	MB_INDEX_NULL2		30	//	2	-	
#define	MB_INDEX_NULL3		32	//	2	-	
#define	MB_INDEX_NULL4		34	//	2	-	
#define	MB_INDEX_NULL5		36	//	2	-	
#define	MB_INDEX_NULL6		38	//	2	-	
#define	MB_INDEX_DI1		40	//	1	-	
#define	MB_INDEX_DI2		41	//	1	-	
#define	MB_INDEX_DI3		42	//	1	-	
#define	MB_INDEX_DI4		43	//	1	-	
#define	MB_INDEX_DI5		44	//	1	-	
#define	MB_INDEX_DI6		45	//	1	-	
#define	MB_INDEX_DI7		46	//	1	-	
#define	MB_INDEX_DI8		47	//	1	-	
#define	MB_INDEX_DI9		48	//	1	-	
#define	MB_INDEX_NULL7		49	//	1	-	
#define	MB_INDEX_ERR_CODE		50	//	1	-	
#define	MB_INDEX_RESERVE1		51	//	1	-	
#define	MB_INDEX_RESERVE2		52	//	1	-	
#define	MB_INDEX_RESERVE3		53	//	1	-	
#define	MB_INDEX_RESERVE4		54	//	1	-	
#define	MB_INDEX_RESERVE5		55	//	1	-	
#define	MB_INDEX_RESERVE6		56	//	1	-	
#define	MB_INDEX_RESERVE7		57	//	1	-	
#define	MB_INDEX_RESERVE8		58	//	1	-	
#define	MB_INDEX_RESERVE9		59	//	1	-	
#define	MB_INDEX_REM_MEMORY1		60	//	1	-	
#define	MB_INDEX_REM_MEMORY2		61	//	1	-	
#define	MB_INDEX_REM_MEMORY3		62	//	1	-	
#define	MB_INDEX_REM_MEMORY4		63	//	1	-	
#define	MB_INDEX_REM_MEMORY5		64	//	1	-	
#define	MB_INDEX_REM_MEMORY6		65	//	1	-	
#define	MB_INDEX_REM_MEMORY7		66	//	1	-	
#define	MB_INDEX_REM_MEMORY8		67	//	1	-	
#define	MB_INDEX_REM_MEMORY9		68	//	2	-	
#define	MB_INDEX_REM_MEMORY10		70	//	2	-	
#define	MB_INDEX_REM_MEMORY11		72	//	2	-	
#define	MB_INDEX_REM_MEMORY12		74	//	2	-	
#define	MB_INDEX_REM_MEMORY13		76	//	2	-	
#define	MB_INDEX_REM_MEMORY14		78	//	2	-	
#define	MB_INDEX_REM_MEMORY15		80	//	2	-	
#define	MB_INDEX_REM_MEMORY16		82	//	2	-	
#define	MB_INDEX_REM_MEMORY17		84	//	2	-	
#define	MB_INDEX_REM_MEMORY18		86	//	2	-	
#define	MB_INDEX_REM_MEMORY19		88	//	2	-	
#define	MB_INDEX_REM_MEMORY20		90	//	2	-	
#define	MB_INDEX_REM_MEMORY21		92	//	2	-	
#define	MB_INDEX_REM_MEMORY22		94	//	2	-	
#define	MB_INDEX_REM_MEMORY23		96	//	2	-	
#define	MB_INDEX_REM_MEMORY24		98	//	2	-	
#define	MB_INDEX_SET_TEMP		100	//	2	-	set temp
#define	MB_INDEX_START_TEMP		102	//	2	-	start temp
#define	MB_INDEX_OUTDOOR_TEMP_REM		104	//	2	-	outdoor temp remote
#define	MB_INDEX_FEEL_TEMP		106	//	2	-	feeling temp
#define	MB_INDEX_FEEL_TEMP_K		108	//	2	-	feeling temp k
#define	MB_INDEX_CURVATURE1		110	//	2	-	
#define	MB_INDEX_CURVATURE2		112	//	2	-	
#define	MB_INDEX_CURVATURE3		114	//	2	-	
#define	MB_INDEX_CURVATURE4		116	//	2	-	
#define	MB_INDEX_MANUAL_OUTPUT		118	//	2	-	
#define	MB_INDEX_HOLIDAY_OFFSET		120	//	2	-	
#define	MB_INDEX_WATER_LEVEL_HL		122	//	2	-	water level high limit
#define	MB_INDEX_WATER_LEVEL_LL		124	//	2	-	water level low limit
#define	MB_INDEX_OUTPUT_LL		126	//	2	-	qi hou output low limit
#define	MB_INDEX_TARGET_TEMP_HL		128	//	2	-	target temp high limit
#define	MB_INDEX_TARGET_TEMP_LL		130	//	2	-	target temp low limit
#define	MB_INDEX_SOLAR_OFFSET		132	//	2	-	solar radiation offset
#define	MB_INDEX_NULL8		134	//	2	-	
#define	MB_INDEX_NULL9		136	//	2	-	
#define	MB_INDEX_NULL10		138	//	2	-	
#define	MB_INDEX_NULL11		140	//	2	-	
#define	MB_INDEX_NULL12		142	//	2	-	
#define	MB_INDEX_NULL13		144	//	2	-	
#define	MB_INDEX_NULL14		146	//	2	-	
#define	MB_INDEX_NULL15		148	//	2	-	
#define	MB_INDEX_MODE		150	//	1	-	qi hou ctrl mode
#define	MB_INDEX_OUTDOOR_TEMP_SW		151	//	1	-	outdoor temp switch
#define	MB_INDEX_SOLAR_SW		152	//	1	-	solar radiation switch
#define	MB_INDEX_NULL16		153	//	1	-	
#define	MB_INDEX_CLOCK_YEAR		154	//	1	-	
#define	MB_INDEX_CLOCK_MONTH		155	//	1	-	
#define	MB_INDEX_CLOCK_DAY		156	//	1	-	
#define	MB_INDEX_CLOCK_WEEK		157	//	1	-	
#define	MB_INDEX_CLOCK_HOUR		158	//	1	-	
#define	MB_INDEX_CLOCK_MIN		159	//	1	-	
#define	MB_INDEX_CLOCK_SEC		160	//	1	-	
#define	MB_INDEX_HOLIDAY_STATE		161	//	1	-	
#define	MB_INDEX_SOLAR_START_HOUR		162	//	1	-	
#define	MB_INDEX_SOLAR_START_MIN		163	//	1	-	
#define	MB_INDEX_SOLAR_END_HOUR		164	//	1	-	
#define	MB_INDEX_SOLAR_END_MIN		165	//	1	-	
#define	MB_INDEX_NULL17		166	//	1	-	
#define	MB_INDEX_NULL18		167	//	1	-	
#define	MB_INDEX_NULL19		168	//	1	-	
#define	MB_INDEX_NULL20		169	//	1	-	
#define	MB_INDEX_DO1		170	//	1	-	
#define	MB_INDEX_DO2		171	//	1	-	
#define	MB_INDEX_DO3		172	//	1	-	
#define	MB_INDEX_DO4		173	//	1	-	
#define	MB_INDEX_DO5		174	//	1	-	
#define	MB_INDEX_NULL21		175	//	1	-	
#define	MB_INDEX_NULL22		176	//	1	-	
#define	MB_INDEX_NULL23		177	//	1	-	
#define	MB_INDEX_NULL24		178	//	1	-	
#define	MB_INDEX_NULL25		179	//	1	-	
#define	MB_INDEX_NULL26		180	//	1	-	
#define	MB_INDEX_NULL27		181	//	1	-	
#define	MB_INDEX_NULL28		182	//	1	-	
#define	MB_INDEX_NULL29		183	//	1	-	
#define	MB_INDEX_NULL30		184	//	1	-	
#define	MB_INDEX_NULL31		185	//	1	-	
#define	MB_INDEX_NULL32		186	//	1	-	
#define	MB_INDEX_NULL33		187	//	1	-	
#define	MB_INDEX_NULL34		188	//	1	-	
#define	MB_INDEX_NULL35		189	//	1	-	
#define	MB_INDEX_SEG1_START_HOUR		190	//	1	-	
#define	MB_INDEX_SEG1_START_MIN		191	//	1	-	
#define	MB_INDEX_SEG1_END_HOUR		192	//	1	-	
#define	MB_INDEX_SEG1_END_MIN		193	//	1	-	
#define	MB_INDEX_SEG2_START_HOUR		194	//	1	-	
#define	MB_INDEX_SEG2_START_MIN		195	//	1	-	
#define	MB_INDEX_SEG2_END_HOUR		196	//	1	-	
#define	MB_INDEX_SEG2_END_MIN		197	//	1	-	
#define	MB_INDEX_SEG3_START_HOUR		198	//	1	-	
#define	MB_INDEX_SEG3_START_MIN		199	//	1	-	
#define	MB_INDEX_SEG3_END_HOUR		200	//	1	-	
#define	MB_INDEX_SEG3_END_MIN		201	//	1	-	
#define	MB_INDEX_SEG4_START_HOUR		202	//	1	-	
#define	MB_INDEX_SEG4_START_MIN		203	//	1	-	
#define	MB_INDEX_SEG4_END_HOUR		204	//	1	-	
#define	MB_INDEX_SEG4_END_MIN		205	//	1	-	
#define	MB_INDEX_SEG5_START_HOUR		206	//	1	-	
#define	MB_INDEX_SEG5_START_MIN		207	//	1	-	
#define	MB_INDEX_SEG5_END_HOUR		208	//	1	-	
#define	MB_INDEX_SEG5_END_MIN		209	//	1	-	
#define	MB_INDEX_SEG6_START_HOUR		210	//	1	-	
#define	MB_INDEX_SEG6_START_MIN		211	//	1	-	
#define	MB_INDEX_SEG6_END_HOUR		212	//	1	-	
#define	MB_INDEX_SEG6_END_MIN		213	//	1	-	
#define	MB_INDEX_SEG7_START_HOUR		214	//	1	-	
#define	MB_INDEX_SEG7_START_MIN		215	//	1	-	
#define	MB_INDEX_SEG7_END_HOUR		216	//	1	-	
#define	MB_INDEX_SEG7_END_MIN		217	//	1	-	
#define	MB_INDEX_SEG8_START_HOUR		218	//	1	-	
#define	MB_INDEX_SEG8_START_MIN		219	//	1	-	
#define	MB_INDEX_SEG8_END_HOUR		220	//	1	-	
#define	MB_INDEX_SEG8_END_MIN		221	//	1	-	
#define	MB_INDEX_SEG1_SW		222	//	1	-	
#define	MB_INDEX_SEG2_SW		223	//	1	-	
#define	MB_INDEX_SEG3_SW		224	//	1	-	
#define	MB_INDEX_SEG4_SW		225	//	1	-	
#define	MB_INDEX_SEG5_SW		226	//	1	-	
#define	MB_INDEX_SEG6_SW		227	//	1	-	
#define	MB_INDEX_SEG7_SW		228	//	1	-	
#define	MB_INDEX_SEG8_SW		229	//	1	-	
#define	MB_INDEX_SEG1_OFFSET		230	//	2	-	
#define	MB_INDEX_SEG2_OFFSET		232	//	2	-	
#define	MB_INDEX_SEG3_OFFSET		234	//	2	-	
#define	MB_INDEX_SEG4_OFFSET		236	//	2	-	
#define	MB_INDEX_SEG5_OFFSET		238	//	2	-	
#define	MB_INDEX_SEG6_OFFSET		240	//	2	-	
#define	MB_INDEX_SEG7_OFFSET		242	//	2	-	
#define	MB_INDEX_SEG8_OFFSET		244	//	2	-	
#define	MB_INDEX_NULL36		246	//	2	-	
#define	MB_INDEX_NULL37		248	//	2	-	
#define	MB_INDEX_NULL38		250	//	2	-	
#define	MB_INDEX_NULL39		252	//	2	-	
#define	MB_INDEX_NULL40		254	//	2	-	
#define	MB_INDEX_NULL41		256	//	2	-	
#define	MB_INDEX_NULL42		258	//	2	-	
#define	MB_INDEX_N40_TEMP		260	//	2	-	-40 target temp
#define	MB_INDEX_N39_TEMP		262	//	2	-	-39 target temp
#define	MB_INDEX_N38_TEMP		264	//	2	-	-38 target temp
#define	MB_INDEX_N37_TEMP		266	//	2	-	-37 target temp
#define	MB_INDEX_N36_TEMP		268	//	2	-	-36 target temp
#define	MB_INDEX_N35_TEMP		270	//	2	-	-35 target temp
#define	MB_INDEX_N34_TEMP		272	//	2	-	-34 target temp
#define	MB_INDEX_N33_TEMP		274	//	2	-	-33 target temp
#define	MB_INDEX_N32_TEMP		276	//	2	-	-32 target temp
#define	MB_INDEX_N31_TEMP		278	//	2	-	-31 target temp
#define	MB_INDEX_N30_TEMP		280	//	2	-	-30 target temp
#define	MB_INDEX_N29_TEMP		282	//	2	-	-29 target temp
#define	MB_INDEX_N28_TEMP		284	//	2	-	-28 target temp
#define	MB_INDEX_N27_TEMP		286	//	2	-	-27 target temp
#define	MB_INDEX_N26_TEMP		288	//	2	-	-26 target temp
#define	MB_INDEX_N25_TEMP		290	//	2	-	-25 target temp
#define	MB_INDEX_N24_TEMP		292	//	2	-	-24 target temp
#define	MB_INDEX_N23_TEMP		294	//	2	-	-23 target temp
#define	MB_INDEX_N22_TEMP		296	//	2	-	-22 target temp
#define	MB_INDEX_N21_TEMP		298	//	2	-	-21 target temp
#define	MB_INDEX_N20_TEMP		300	//	2	-	-20 target temp
#define	MB_INDEX_N19_TEMP		302	//	2	-	-19 target temp
#define	MB_INDEX_N18_TEMP		304	//	2	-	-18 target temp
#define	MB_INDEX_N17_TEMP		306	//	2	-	-17 target temp
#define	MB_INDEX_N16_TEMP		308	//	2	-	-16 target temp
#define	MB_INDEX_N15_TEMP		310	//	2	-	-15 target temp
#define	MB_INDEX_N14_TEMP		312	//	2	-	-14 target temp
#define	MB_INDEX_N13_TEMP		314	//	2	-	-13 target temp
#define	MB_INDEX_N12_TEMP		316	//	2	-	-12 target temp
#define	MB_INDEX_N11_TEMP		318	//	2	-	-11 target temp
#define	MB_INDEX_N10_TEMP		320	//	2	-	-10 target temp
#define	MB_INDEX_N9_TEMP		322	//	2	-	-9 target temp
#define	MB_INDEX_N8_TEMP		324	//	2	-	-8 target temp
#define	MB_INDEX_N7_TEMP		326	//	2	-	-7 target temp
#define	MB_INDEX_N6_TEMP		328	//	2	-	-6 target temp
#define	MB_INDEX_N5_TEMP		330	//	2	-	-5 target temp
#define	MB_INDEX_N4_TEMP		332	//	2	-	-4 target temp
#define	MB_INDEX_N3_TEMP		334	//	2	-	-3 target temp
#define	MB_INDEX_N2_TEMP		336	//	2	-	-2 target temp
#define	MB_INDEX_N1_TEMP		338	//	2	-	-1 target temp
#define	MB_INDEX_0_TEMP		340	//	2	-	0 target temp
#define	MB_INDEX_P1_TEMP		342	//	2	-	1 target temp
#define	MB_INDEX_P2_TEMP		344	//	2	-	2 target temp
#define	MB_INDEX_P3_TEMP		346	//	2	-	3 target temp
#define	MB_INDEX_P4_TEMP		348	//	2	-	4 target temp
#define	MB_INDEX_P5_TEMP		350	//	2	-	5 target temp
#define	MB_INDEX_P6_TEMP		352	//	2	-	6 target temp
#define	MB_INDEX_P7_TEMP		354	//	2	-	7 target temp
#define	MB_INDEX_P8_TEMP		356	//	2	-	8 target temp
#define	MB_INDEX_P9_TEMP		358	//	2	-	9 target temp
#define	MB_INDEX_P10_TEMP		360	//	2	-	10 target temp
#define	MB_INDEX_P11_TEMP		362	//	2	-	11 target temp
#define	MB_INDEX_P12_TEMP		364	//	2	-	12 target temp
#define	MB_INDEX_P13_TEMP		366	//	2	-	13 target temp
#define	MB_INDEX_P14_TEMP		368	//	2	-	14 target temp
#define	MB_INDEX_P15_TEMP		370	//	2	-	15 target temp
#define	MB_INDEX_NULL43		372	//	2	-	
#define	MB_INDEX_NULL44		374	//	2	-	
#define	MB_INDEX_NULL45		376	//	2	-	
#define	MB_INDEX_NULL46		378	//	2	-	
#define	MB_INDEX_NULL47		380	//	2	-	
#define	MB_INDEX_NULL48		382	//	2	-	
#define	MB_INDEX_NULL49		384	//	2	-	
#define	MB_INDEX_NULL50		386	//	2	-	
#define	MB_INDEX_NULL51		388	//	2	-	
#define	MB_INDEX_NULL52		390	//	2	-	
#define	MB_INDEX_NULL53		392	//	2	-	
#define	MB_INDEX_NULL54		394	//	2	-	
#define	MB_INDEX_NULL55		396	//	2	-	
#define	MB_INDEX_NULL56		398	//	2	-	
#define	MB_INDEX_NULL57		400	//	2	-	
#define	MB_INDEX_NULL58		402	//	2	-	
#define	MB_INDEX_NULL59		404	//	2	-	
#define	MB_INDEX_NULL60		406	//	2	-	
#define	MB_INDEX_NULL61		408	//	2	-	
#define	MB_INDEX_ACTUATOR_TYPE		410	//	1	-	
#define	MB_INDEX_AO_SIGNAL_TYPE		411	//	1	-	4~20mA / 0-10V
#define	MB_INDEX_QH_FEEDBACK_CHN		412	//	1	-	
#define	MB_INDEX_OUTDOOR_TEMP_LOC_CHN		413	//	1	-	
#define	MB_INDEX_WATER_LEVEL_CHN		414	//	1	-	
#define	MB_INDEX_WATER_CTRL_CHN		415	//	1	-	
#define	MB_INDEX_ADJUST_TIME		416	//	1	-	auto ctrl adjust cycle
#define	MB_INDEX_AI1_TYPE		417	//	1	-	
#define	MB_INDEX_AI2_TYPE		418	//	1	-	
#define	MB_INDEX_AI3_TYPE		419	//	1	-	
#define	MB_INDEX_AI4_TYPE		420	//	1	-	
#define	MB_INDEX_AI5_TYPE		421	//	1	-	
#define	MB_INDEX_AI6_TYPE		422	//	1	-	
#define	MB_INDEX_AI7_TYPE		423	//	1	-	
#define	MB_INDEX_AI8_TYPE		424	//	1	-	
#define	MB_INDEX_COM1_START_ID		425	//	1	-	
#define	MB_INDEX_COM2_START_ID		426	//	1	-	
#define	MB_INDEX_COM1_BAUD		427	//	1	-	
#define	MB_INDEX_COM1_DATABITS		428	//	1	-	
#define	MB_INDEX_COM1_PARITY		429	//	1	-	
#define	MB_INDEX_COM2_BAUD		430	//	1	-	
#define	MB_INDEX_COM2_DATABITS		431	//	1	-	
#define	MB_INDEX_COM2_PARITY		432	//	1	-	
#define	MB_INDEX_NULL62		433	//	1	-	
#define	MB_INDEX_NULL63		434	//	1	-	
#define	MB_INDEX_NULL64		435	//	1	-	
#define	MB_INDEX_NULL65		436	//	1	-	
#define	MB_INDEX_NULL66		437	//	1	-	
#define	MB_INDEX_NULL67		438	//	1	-	
#define	MB_INDEX_NULL68		439	//	1	-	
#define	MB_INDEX_NULL69		440	//	1	-	
#define	MB_INDEX_NULL70		441	//	1	-	
#define	MB_INDEX_NULL71		442	//	1	-	
#define	MB_INDEX_NULL72		443	//	1	-	
#define	MB_INDEX_NULL73		444	//	1	-	
#define	MB_INDEX_NULL74		445	//	1	-	
#define	MB_INDEX_NULL75		446	//	1	-	
#define	MB_INDEX_NULL76		447	//	1	-	
#define	MB_INDEX_NULL77		448	//	1	-	
#define	MB_INDEX_NULL78		449	//	1	-	
#define	MB_INDEX_ERR_SEG1		450	//	2	-	
#define	MB_INDEX_ERR_SEG2		452	//	2	-	
#define	MB_INDEX_ERR_SEG3		454	//	2	-	
#define	MB_INDEX_ERR_SEG4		456	//	2	-	
#define	MB_INDEX_ERR_CHANGE		458	//	2	-	
#define	MB_INDEX_ERR_SEG1_OUTPUT		460	//	2	-	
#define	MB_INDEX_ERR_SEG2_OUTPUT		462	//	2	-	
#define	MB_INDEX_ERR_SEG3_OUTPUT		464	//	2	-	
#define	MB_INDEX_ERR_SEG4_OUTPUT		466	//	2	-	
#define	MB_INDEX_NULL79		468	//	2	-	
#define	MB_INDEX_NULL80		470	//	2	-	
#define	MB_INDEX_NULL81		472	//	2	-	
#define	MB_INDEX_NULL82		474	//	2	-	
#define	MB_INDEX_NULL83		476	//	2	-	
#define	MB_INDEX_NULL84		478	//	2	-	
#define	MB_INDEX_NULL85		480	//	2	-	
#define	MB_INDEX_NULL86		482	//	2	-	
#define	MB_INDEX_NULL87		484	//	2	-	
#define	MB_INDEX_NULL88		486	//	2	-	
#define	MB_INDEX_NULL89		488	//	2	-	
#define	MB_INDEX_AI1_RANGE_L		490	//	2	-	
#define	MB_INDEX_AI1_RANGE_H		492	//	2	-	
#define	MB_INDEX_AI2_RANGE_L		494	//	2	-	
#define	MB_INDEX_AI2_RANGE_H		496	//	2	-	
#define	MB_INDEX_AI3_RANGE_L		498	//	2	-	
#define	MB_INDEX_AI3_RANGE_H		500	//	2	-	
#define	MB_INDEX_AI4_RANGE_L		502	//	2	-	
#define	MB_INDEX_AI4_RANGE_H		504	//	2	-	
#define	MB_INDEX_AI5_RANGE_L		506	//	2	-	
#define	MB_INDEX_AI5_RANGE_H		508	//	2	-	
#define	MB_INDEX_AI6_RANGE_L		510	//	2	-	
#define	MB_INDEX_AI6_RANGE_H		512	//	2	-	
#define	MB_INDEX_AI7_RANGE_L		514	//	2	-	
#define	MB_INDEX_AI7_RANGE_H		516	//	2	-	
#define	MB_INDEX_AI8_RANGE_L		518	//	2	-	
#define	MB_INDEX_AI8_RANGE_H		520	//	2	-	
#define	MB_INDEX_NULL90		522	//	2	-	
#define	MB_INDEX_NULL91		524	//	2	-	
#define	MB_INDEX_AI1_ZERO_FAC		526	//	2	-	
#define	MB_INDEX_AI2_ZERO_FAC		528	//	2	-	
#define	MB_INDEX_AI3_ZERO_FAC		530	//	2	-	
#define	MB_INDEX_AI4_ZERO_FAC		532	//	2	-	
#define	MB_INDEX_AI5_ZERO_FAC		534	//	2	-	
#define	MB_INDEX_AI6_ZERO_FAC		536	//	2	-	
#define	MB_INDEX_AI7_ZERO_FAC		538	//	2	-	
#define	MB_INDEX_AI8_ZERO_FAC		540	//	2	-	
#define	MB_INDEX_AI1_FULL_FAC		542	//	2	-	
#define	MB_INDEX_AI2_FULL_FAC		544	//	2	-	
#define	MB_INDEX_AI3_FULL_FAC		546	//	2	-	
#define	MB_INDEX_AI4_FULL_FAC		548	//	2	-	
#define	MB_INDEX_AI5_FULL_FAC		550	//	2	-	
#define	MB_INDEX_AI6_FULL_FAC		552	//	2	-	
#define	MB_INDEX_AI7_FULL_FAC		554	//	2	-	
#define	MB_INDEX_AI8_FULL_FAC		556	//	2	-	
#define	MB_INDEX_AI1_FILTER_FAC		558	//	2	-	
#define	MB_INDEX_AI2_FILTER_FAC		560	//	2	-	
#define	MB_INDEX_AI3_FILTER_FAC		562	//	2	-	
#define	MB_INDEX_AI4_FILTER_FAC		564	//	2	-	
#define	MB_INDEX_AI5_FILTER_FAC		566	//	2	-	
#define	MB_INDEX_AI6_FILTER_FAC		568	//	2	-	
#define	MB_INDEX_AI7_FILTER_FAC		570	//	2	-	
#define	MB_INDEX_AI8_FILTER_FAC		572	//	2	-	
//#define	MB_ADD                 	573	//	add


#define REG_HOLDING_NREGS 		    600     //
#define MB_INDEX_START_SAVE         100     // start save regs
#define MB_INDEX_END_SAVE           573     // end save regs
#define  SAVE_REGS_NUM		        474     // 573 - 100 + 1 = 473 

#endif
