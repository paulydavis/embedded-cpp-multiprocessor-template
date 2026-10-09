# TDD and Hostile Review

For each behavior, write one focused test, run it red, implement the smallest
change, then run the complete host suite. A test is not evidence for a
mechanism if it remains green after that mechanism is removed or bypassed.

Reviewers should challenge assumptions about timing, restart, heartbeat
wraparound, full IPC flags, missed acknowledgements, cache maintenance, and
vendor-only behavior. Record the command, compiler, target, fixture, and exact
result for every claim. Host tests validate portable behavior; they do not
qualify a board or its vendor SDK.

