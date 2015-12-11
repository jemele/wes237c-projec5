#ifndef _cordic_h
#define _cordic_h
#include <ap_fixed.h>
typedef ap_fixed<64,8> fix_num;
void cordic(fix_num x, fix_num y, fix_num * r, fix_num * theta);
#endif //_cordic_h
