#include "ai.h"		
#include "my_modbus.h"
#include "calculate.h"

static ai_control_calc_para_t g_qh_ai_ctrl_para;

ai_control_calc_para_t *getAndUpdate_qh_ai_ctrlPara(void)
{
	g_qh_ai_ctrl_para.err_seg1 = Words2Float(&usRegHoldingBuf[MB_INDEX_ERR_SEG1], 3412);
	g_qh_ai_ctrl_para.err_seg2 = Words2Float(&usRegHoldingBuf[MB_INDEX_ERR_SEG2], 3412);
	g_qh_ai_ctrl_para.err_seg3 = Words2Float(&usRegHoldingBuf[MB_INDEX_ERR_SEG3], 3412);
	g_qh_ai_ctrl_para.err_seg4 = Words2Float(&usRegHoldingBuf[MB_INDEX_ERR_SEG4], 3412);
	g_qh_ai_ctrl_para.err_change = Words2Float(&usRegHoldingBuf[MB_INDEX_ERR_CHANGE], 3412);
	g_qh_ai_ctrl_para.err_seg1_out = Words2Float(&usRegHoldingBuf[MB_INDEX_ERR_SEG1_OUTPUT], 3412);
	g_qh_ai_ctrl_para.err_seg2_out = Words2Float(&usRegHoldingBuf[MB_INDEX_ERR_SEG2_OUTPUT], 3412);
	g_qh_ai_ctrl_para.err_seg3_out = Words2Float(&usRegHoldingBuf[MB_INDEX_ERR_SEG3_OUTPUT], 3412);
	g_qh_ai_ctrl_para.err_seg4_out = Words2Float(&usRegHoldingBuf[MB_INDEX_ERR_SEG4_OUTPUT], 3412);

	return &g_qh_ai_ctrl_para;
}


float ai_control_calc(ai_control_calc_para_t *ai_para, float sv, float pv)
{
	float res = 0.0;
	float err = 0.0, abs_err = 0.0;
	u8 flag_err_pn = 0;
	err = sv - pv;

	if((ai_para->err_seg1 > 0) &&
	   (ai_para->err_seg1 < ai_para->err_seg2) &&
	   (ai_para->err_seg2 < ai_para->err_seg3) &&
	   (ai_para->err_seg3 < ai_para->err_seg4) &&
	   (ai_para->err_change > 0)){   //参数递增，且均大于0
		//误差绝对值求解，正反向误差判断
		if(err > ai_para->err_seg1){	//正误差
			flag_err_pn = 0;
			abs_err = err;
		}else if(err < (0.0 - ai_para->err_seg1)){	//负误差
			flag_err_pn = 1;
			abs_err = 0.0 - err;
		}else{
			return 0.0;					// 误差小于定义的最小误差，不动作，返回0
		}

		// AI计算
		// 误差区间判断
		if(abs_err > ai_para->err_seg1){
			if(FloatIntervalJudge(abs_err, ai_para->err_seg1, ai_para->err_seg2)){
				res = ai_para->err_seg1_out;
			}else if(FloatIntervalJudge(abs_err, ai_para->err_seg2, ai_para->err_seg3)){
				res = ai_para->err_seg2_out;
			}else if(FloatIntervalJudge(abs_err, ai_para->err_seg3, ai_para->err_seg4)){
				res = ai_para->err_seg3_out;	
			}else if(abs_err > ai_para->err_seg4){  
				res = ai_para->err_seg4_out;	
			}else{
				res = ai_para->err_seg2_out;
			}
		}

		// 变积分输出
		// 正反向误差区别判断输出
		if(flag_err_pn > 0){	//反向误差
			if((err - ai_para->err_last) > ai_para->err_change){
				res /= -2.0;		// 变化明显，输出减半，实现“微分”调控
			}else{
				res = 0.0 - res;		// 由于是反向误差，所以输出叫递减，而res值是绝对值，所以要通过
										// 0 减，转换成负值
			}
		}else{	//正向误差
			if((ai_para->err_last - err) > ai_para->err_change){
				res /= 2.0;
			}
		}
		// 更新err_last
		ai_para->err_last = err;
	}else{		//误差区间参数异常，则返回±0.2，之所以有返回是为了有提示功能，而且也会缓慢调节到目标
		if(err > 0){
			res = 0.2;
		}else if(err < 0){
			res = -0.2;
		}else{
			res = 0.0;
		}
	}
	return res;
}