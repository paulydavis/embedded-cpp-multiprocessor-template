# IPC Freshness Contract

The producer owns the message payload and increments the heartbeat only after
the payload is complete. The consumer observes the heartbeat after the
hardware IPC interrupt and acknowledges the flag only after the payload has
been copied or consumed.

The reference monitor treats any changed 16-bit heartbeat as fresh, including
wraparound. An unchanged heartbeat becomes stale only after the injected clock
passes the configured timeout. A busy IPC flag blocks publication; the caller
must choose a bounded retry or a fault response rather than overwrite data.

Each real CPU pair must add its cache flush/invalidate and memory-barrier rules
to the platform adapter. This example does not claim a universal timeout or
that stale data is safe to use: the system requirement decides whether the
consumer holds the last-known-good value, enters a safe state, or restarts the
producer.

