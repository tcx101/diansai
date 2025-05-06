#ifndef __KEY_H
#define __KEY_H
#include "headfile.h"
typedef struct key_str
{
	uint8_t key_flag;
	uint8_t key_state;
	uint8_t key_level;
	uint8_t key_time;
	uint8_t key_longflag;
}key_str;
extern key_str key[];
void key_scan();
void key_start();
void key_function();
#endif