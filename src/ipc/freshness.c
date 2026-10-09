#include "project/ipc/freshness.h"

#include <stddef.h>

void project_freshness_init(project_freshness_t *monitor, uint32_t timeout_ms) {
  if (monitor == NULL) {
    return;
  }
  monitor->last_heartbeat = 0u;
  monitor->last_change_ms = 0u;
  monitor->timeout_ms = timeout_ms;
  monitor->initialized = 0u;
  monitor->state = PROJECT_DATA_STALE;
}

project_data_state_t project_freshness_observe(project_freshness_t *monitor,
                                               uint16_t heartbeat,
                                               uint32_t now_ms) {
  if (monitor == NULL) {
    return PROJECT_DATA_STALE;
  }
  if ((monitor->initialized == 0u) || (heartbeat != monitor->last_heartbeat)) {
    monitor->initialized = 1u;
    monitor->last_heartbeat = heartbeat;
    monitor->last_change_ms = now_ms;
    monitor->state = PROJECT_DATA_FRESH;
    return monitor->state;
  }
  if ((uint32_t)(now_ms - monitor->last_change_ms) > monitor->timeout_ms) {
    monitor->state = PROJECT_DATA_STALE;
  }
  return monitor->state;
}

project_data_state_t
project_freshness_state(const project_freshness_t *monitor) {
  return monitor == NULL ? PROJECT_DATA_STALE : monitor->state;
}

int project_ipc_can_publish(int flag_busy) { return flag_busy == 0; }
