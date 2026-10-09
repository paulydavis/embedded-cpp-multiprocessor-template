#include "project/status.h"
#include "unity.h"

void test_status_ok_is_success(void) {
  TEST_ASSERT_TRUE(project_status_is_ok(PROJECT_STATUS_OK));
}

void test_status_timeout_is_not_success(void) {
  TEST_ASSERT_FALSE(project_status_is_ok(PROJECT_STATUS_TIMEOUT));
}
