#include "project/ipc/freshness.h"
#include "unity.h"

void test_new_heartbeat_is_fresh(void) {
  project_freshness_t monitor;
  project_freshness_init(&monitor, 100u);
  TEST_ASSERT_EQUAL_INT(PROJECT_DATA_FRESH,
                        project_freshness_observe(&monitor, 1u, 10u));
}

void test_unchanged_heartbeat_times_out(void) {
  project_freshness_t monitor;
  project_freshness_init(&monitor, 100u);
  (void)project_freshness_observe(&monitor, 1u, 10u);
  TEST_ASSERT_EQUAL_INT(PROJECT_DATA_STALE,
                        project_freshness_observe(&monitor, 1u, 111u));
}

void test_heartbeat_wraparound_is_fresh(void) {
  project_freshness_t monitor;
  project_freshness_init(&monitor, 100u);
  (void)project_freshness_observe(&monitor, UINT16_MAX, 10u);
  TEST_ASSERT_EQUAL_INT(PROJECT_DATA_FRESH,
                        project_freshness_observe(&monitor, 0u, 20u));
}

void test_busy_ipc_flag_blocks_publish(void) {
  TEST_ASSERT_FALSE(project_ipc_can_publish(1));
  TEST_ASSERT_TRUE(project_ipc_can_publish(0));
}
