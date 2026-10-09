#ifndef PROJECT_STATUS_H
#define PROJECT_STATUS_H

typedef enum {
  PROJECT_STATUS_OK = 0,
  PROJECT_STATUS_TIMEOUT = 1,
  PROJECT_STATUS_INVALID = 2
} project_status_t;

int project_status_is_ok(project_status_t status);

#endif
