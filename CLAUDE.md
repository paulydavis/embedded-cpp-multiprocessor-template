# Project Review and Coding Rules

These rules bias toward correctness and reviewability over speed.

## Before coding

- State assumptions and acceptance criteria.
- Present materially different interpretations instead of choosing silently.
- Prefer the simplest design; reject speculative flexibility.

## While coding

- Make surgical changes. Do not refactor unrelated code or add abstractions
  for one call site.
- Write a failing test first, then the smallest implementation that passes.
- Keep portable logic independent of vendor headers, MMIO, and wall-clock time.
- Give ownership and ordering names to every shared-data field.

## Review and evidence

- Try to falsify the claim: remove or bypass the production mechanism and make
  sure the test fails for the claimed reason.
- Test boundary cases: wraparound, timeout, restart, full IPC flag, and missed
  acknowledgement.
- Separate host evidence from hardware evidence. A host pass is not a board
  qualification.
- Every result must identify the command, target, toolchain, and test input.

## Embedded safety

- No silent fallback from a missing vendor SDK to a host build.
- No unbounded waits in a communication or shutdown path.
- Use explicit status values and injected clocks rather than hidden globals.
- Document cache maintenance and memory barriers at each CPU boundary.

