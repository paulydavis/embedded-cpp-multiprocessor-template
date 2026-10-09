# Embedded C/C++ Multiprocessor Template

Template for professional firmware and embedded applications spanning an NXP
i.MX8 application processor, an STM32 microcontroller, and a Texas Instruments
DSP. Portable behavior lives in `src/` and `include/`; board and SDK glue
lives under `platforms/`.

## Host-first workflow

The host build is the fast feedback loop and does not require a board or vendor
SDK:

```sh
cmake --preset host-debug
cmake --build --preset host-debug
ctest --preset host-debug --output-on-failure
```

STM32-facing behavior is tested with Unity on the host and can be linked into
the STM32 target. The i.MX8 and TI DSP builds reuse the same core sources and
provide only platform adapters. Vendor-native CubeIDE and CCS projects belong
under `platforms/` and must not fork core behavior.

## Engineering rules

- Write a failing test before production code and run the complete host suite.
- Treat cross-CPU messages as contracts: document ownership, sequence or
  heartbeat, acknowledgement, barriers/cache maintenance, timeout, and the
  last-known-good policy.
- Make assumptions and tradeoffs explicit. Prefer the smallest change that
  satisfies an observable requirement.
- Review adversarially: remove the claimed mechanism and prove the test goes
  red; do not confuse a green fixture or mock assertion with proof of behavior.
- Hardware-only claims require a named board, SDK/compiler, procedure, and log.

See `CLAUDE.md`, `docs/interfaces/ipc-freshness.md`, and
`docs/testing/tdd-and-hostile-review.md` for the project workflow.

Bitbucket Pipelines runs the host build, Unity tests, formatting check, and
lint script from `bitbucket-pipelines.yml`. Hardware qualification remains a
separate job with the required board and vendor SDK.
