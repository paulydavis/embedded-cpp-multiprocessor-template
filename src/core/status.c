#include "project/status.h"

int project_status_is_ok(project_status_t status) {
  return status == PROJECT_STATUS_OK;
}
