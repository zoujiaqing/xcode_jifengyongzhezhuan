/*
 * =====================================================================================
 *
 *       Filename:  Time.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/30/2013 10:52:01 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Plus Wu (), plus.wu1986@gmail.com
 *        Company:  Taomee
 *
 * =====================================================================================
 */
#include "Time.hpp"
namespace base {
uint32_t next_week_day_end(uint32_t w_idx) {
	uint32_t now_week_idx = Time().WeekDay();
	uint32_t interval_day = 0;
	if (w_idx == 0 ){
		interval_day = 7 - now_week_idx;
	} else if (w_idx < now_week_idx) {
		interval_day = 7 - now_week_idx + w_idx;
	} else if (w_idx > now_week_idx) {
		interval_day = w_idx - now_week_idx;
	}
	return Time().end_of_day().seconds() + interval_day * 3600 * 24;
}
}


