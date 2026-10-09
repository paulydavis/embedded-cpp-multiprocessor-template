# Embedded C/C++ Project Template Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Replace the Swift/macOS scaffold with a reusable professional C/C++ template for projects spanning i.MX8, STM32, and TI DSP targets.

**Architecture:** Portable core code and IPC contracts live in `src/` and `include/`; platform adapters live under `platforms/`; host builds and Unity tests run through CMake; vendor-native CubeIDE/CCS projects remain wrappers around the same sources.

**Tech Stack:** C11/C++17-compatible code, CMake 3.24+, Unity tests, GCC/Clang host toolchains, optional STM32 and TI vendor toolchains.

**Spec:** `docs/embedded-c-cpp-template-design.md`

## Global Constraints

- The repository is C/C++ first; Swift, Swift Package Manager, and macOS app scaffolding are removed.
- CMake is the common entry point for host builds, static analysis, and tests.
- Unity is the unit-test framework for STM32-facing code.
- Hardware access is isolated behind small interfaces and host tests do not require hardware.
- Cross-CPU structures document ownership, ordering, sequence/heartbeat, acknowledgement, timeout, and cache/barrier rules.
- The implementation uses TDD and adversarial review; tests must be able to fail when the claimed production mechanism is removed.

## Review Focus

- Heartbeat wraparound and unchanged-heartbeat timeout: Task 4.
- Full IPC flag and producer backpressure: Task 4.
- Host builds accidentally including vendor headers: Task 3.
- Unity tests passing after freshness logic is removed: Task 5.
- Vendor wrappers forking portable behavior: Task 6.

---

### Task 1: Remove the Swift scaffold and establish repository policy

**Files:** delete `Package.swift`, `Sources/LiveAstroCore/`, `Sources/LiveAstroStudio/`, `Tests/LiveAstroCoreTests/ScaffoldTests.swift`, and old Swift-specific spec/plan files; modify `.gitignore`; create `README.md` and `CLAUDE.md`.

- [ ] Add a shell contract check that asserts the Swift files are absent and that README names i.MX8, STM32, TI DSP, CMake, and Unity.
- [ ] Run the check and confirm it fails against the current scaffold.
- [ ] Remove the old files. Write README with the three target roles, host-first workflow, Unity test commands, and vendor-toolchain boundaries.
- [ ] Write `CLAUDE.md` from the linked review guidance: state assumptions, prefer simplicity, make surgical changes, define observable goals, write failing tests, and attempt mechanism-removal mutations.
- [ ] Use this ignore baseline:

```gitignore
build/
out/
cmake-build-*/
.cache/
.vscode/
.idea/
compile_commands.json
*.elf
*.hex
*.bin
*.map
*.d
```

- [ ] Run the contract check again; expect PASS.
- [ ] Commit: `chore: replace Swift scaffold with embedded project contract`.

### Task 2: Add the common CMake project and portable status API

**Files:** create `CMakeLists.txt`, `CMakePresets.json`, `cmake/toolchains/host.cmake`, `include/project/status.h`, `src/core/status.c`, `tests/unit/status_test.c`, and empty `platforms/imx8/app/`, `platforms/stm32/app/`, and `platforms/ti-dsp/app/` directories.

- [ ] Write a C test using `project_status_is_ok(PROJECT_STATUS_OK)` and `project_status_is_ok(PROJECT_STATUS_TIMEOUT)`; run `cmake --preset host-debug` and confirm failure because the project does not exist.
- [ ] Implement a pure status API, a `project_core` library, a `project_host_smoke` executable, and a CTest registration.
- [ ] Define the `host-debug` preset with Debug mode and compile commands enabled.
- [ ] Run `cmake --preset host-debug && cmake --build --preset host-debug && ctest --preset host-debug --output-on-failure`; expect PASS.
- [ ] Commit: `build: add host CMake project and portable status API`.

### Task 3: Isolate platform adapters

**Files:** create `include/project/platform/platform_time.h`, `include/project/platform/platform_memory.h`, host adapter implementations, target adapter stubs under `platforms/stm32/bsp/`, `platforms/imx8/board/`, and `platforms/ti-dsp/hal/`, plus `docs/architecture/platform-boundaries.md`; modify CMake.

- [ ] Write a fake-clock test for `project_time_now_ms()` and confirm host linking fails before the adapter exists.
- [ ] Implement `uint32_t project_time_now_ms(void)` and `void project_memory_barrier(void)` behind the platform headers.
- [ ] Keep vendor files as adapter hooks only; portable code must not include STM32 HAL, NXP BSP, or TI CSL headers.
- [ ] Build the host preset and grep its compile commands for vendor headers; expect no matches.
- [ ] Commit: `arch: isolate vendor platform adapters from portable code`.

### Task 4: Add the IPC freshness contract

**Files:** create `include/project/ipc/freshness.h`, `include/project/ipc/message_contract.h`, `src/ipc/freshness.c`, `tests/unit/freshness_test.c`, and `docs/interfaces/ipc-freshness.md`; modify CMake.

- [ ] Write failing tests for a changed heartbeat, unchanged heartbeat timeout, heartbeat wraparound, and a busy IPC flag.
- [ ] Use these interfaces:

```c
void project_freshness_init(project_freshness_t *monitor, uint32_t timeout_ms);
project_data_state_t project_freshness_observe(project_freshness_t *, uint16_t heartbeat, uint32_t now_ms);
project_data_state_t project_freshness_state(const project_freshness_t *);
bool project_ipc_can_publish(bool flag_busy);
```

- [ ] Implement the smallest monitor: last heartbeat, last-observed time, initialization state, and monotone stale status. Treat any changed 16-bit heartbeat as fresh, including wraparound.
- [ ] Document owner, ordering, IPC set/ack, barriers/cache maintenance, restart behavior, timeout response, and last-known-good policy.
- [ ] Run CTest; expect all freshness tests PASS.
- [ ] Commit: `feat: add IPC freshness and backpressure contract`.

### Task 5: Add Unity integration and adversarial TDD checks

**Files:** create `tests/unity/unity.h`, `tests/unity/unity.c`, `tests/unity/test_runner.c`, `tests/unit/freshness_unity_test.c`, `scripts/run_tests.sh`, and `docs/testing/tdd-and-hostile-review.md`; modify CMake and README.

- [ ] Write a Unity runner that calls `setUp`, each test, and `tearDown`, returning nonzero on an assertion failure.
- [ ] Add `TEST_ASSERT_TRUE`, `TEST_ASSERT_FALSE`, and `TEST_ASSERT_EQUAL_UINT32` and port the freshness cases to Unity.
- [ ] Run `scripts/run_tests.sh` before wiring the target and confirm it fails.
- [ ] Wire the host Unity target. Keep the test API usable by an STM32 target linking the same Unity sources or approved Unity package.
- [ ] Run normal tests and a mutation copy with heartbeat observation removed; normal tests must pass and the mutation must fail.
- [ ] Commit: `test: add Unity runner and adversarial TDD workflow`.

### Task 6: Add CI, static analysis, and vendor build entry points

**Files:** create `.github/workflows/host.yml`, `.github/workflows/review.yml`, `scripts/format.sh`, `scripts/lint.sh`, `cmake/toolchains/stm32.cmake`, `cmake/toolchains/imx8.cmake`, `cmake/toolchains/ti-dsp.cmake`, and `docs/architecture/vendor-builds.md`; modify README.

- [ ] Add a CI contract check for CTest, warnings, and Unity.
- [ ] Make host CI configure a clean directory, build with warnings enabled, run CTest, run lint, and upload logs.
- [ ] Make review CI check formatting, changed-file scope, and required evidence.
- [ ] Make each vendor toolchain fail clearly when its SDK/compiler path is absent; never silently compile with the host toolchain.
- [ ] Run `scripts/format.sh --check && scripts/lint.sh && scripts/run_tests.sh`; expect PASS on the host and explicit SDK messages for unavailable vendor toolchains.
- [ ] Commit: `ci: add host gates and explicit vendor toolchain wrappers`.

### Task 7: Final inventory and verification

**Files:** modify README and the design document only if verification finds stale wording; delete any remaining obsolete Swift/SPM/Superpowers implementation artifacts.

- [ ] Search for `Swift`, `SwiftPM`, `Package.swift`, and obsolete app references; historical migration notes may remain only when clearly labeled.
- [ ] Run `cmake --preset host-debug && cmake --build --preset host-debug && ctest --preset host-debug --output-on-failure && scripts/lint.sh`.
- [ ] Perform a fresh review: remove freshness logic and confirm Unity goes red, inspect host/vendor dependency isolation, and verify every changed file traces to the approved design.
- [ ] Commit: `chore: finalize embedded C/C++ project template`.

