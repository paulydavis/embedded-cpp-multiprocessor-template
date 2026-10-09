#ifndef PROJECT_PLATFORM_TIME_H
#define PROJECT_PLATFORM_TIME_H

#include <stdint.h>

uint32_t project_time_now_ms(void);
void project_test_set_time_ms(uint32_t now_ms);

#endif
