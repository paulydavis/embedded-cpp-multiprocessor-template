#include "project/platform/platform_time.h"

static uint32_t test_now_ms;

uint32_t project_time_now_ms(void) { return test_now_ms; }

void project_test_set_time_ms(uint32_t now_ms) { test_now_ms = now_ms; }
