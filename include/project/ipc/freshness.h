#ifndef PROJECT_IPC_FRESHNESS_H
#define PROJECT_IPC_FRESHNESS_H

#include <stdint.h>

typedef enum {
  PROJECT_DATA_FRESH = 0,
  PROJECT_DATA_STALE = 1
} project_data_state_t;

typedef struct {
  uint16_t last_heartbeat;
  uint32_t last_change_ms;
  uint32_t timeout_ms;
  uint8_t initialized;
  project_data_state_t state;
} project_freshness_t;

void project_freshness_init(project_freshness_t *monitor, uint32_t timeout_ms);
project_data_state_t project_freshness_observe(project_freshness_t *monitor,
                                               uint16_t heartbeat,
                                               uint32_t now_ms);
project_data_state_t
project_freshness_state(const project_freshness_t *monitor);
int project_ipc_can_publish(int flag_busy);

#endif
