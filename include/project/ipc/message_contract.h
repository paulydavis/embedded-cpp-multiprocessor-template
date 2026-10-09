#ifndef PROJECT_IPC_MESSAGE_CONTRACT_H
#define PROJECT_IPC_MESSAGE_CONTRACT_H

/* The producer owns payload fields and increments heartbeat after the payload
 * is written. The consumer acknowledges the hardware IPC flag only after it
 * has copied or consumed the payload. Platform adapters provide barriers and
 * cache maintenance around these operations. */
typedef struct {
  unsigned int heartbeat;
  unsigned int payload_length;
} project_message_contract_t;

#endif
