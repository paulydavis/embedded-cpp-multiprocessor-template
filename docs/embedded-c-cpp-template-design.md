# Embedded C/C++ Project Template Design

## Purpose

Convert this local scaffold into a reusable professional embedded-software
template for projects that share data and behavior across:

- an NXP i.MX8 application processor;
- an STM32 microcontroller; and
- a Texas Instruments DSP.

The template must support host-side development and review without requiring
vendor IDEs, while preserving vendor-native projects for flashing, debugging,
and hardware-specific integration.

## Constraints and principles

1. The repository is C/C++ first; no desktop-language or desktop-app scaffold
   is part of the template.
2. CMake is the common entry point for host builds, static analysis, and tests.
   STM32CubeIDE/Make and TI Code Composer Studio projects remain optional
   wrappers under their respective platform directories.
3. Unity is the unit-test framework for STM32-facing code. Host tests may use
   the same Unity assertions and test runner so behavior can be exercised
   without hardware.
4. Hardware access is isolated behind small interfaces. Core logic must be
   testable with fakes; tests must not require a board, vendor SDK, or lab
   connection unless explicitly marked as hardware validation.
5. Work follows test-driven development: reproduce or specify behavior with a
   failing test, implement the smallest change, then run the relevant checks.
6. Reviews are adversarial as well as conformance-based. Reviewers should try
   to falsify claims, inspect the boundary where data crosses CPUs, and reject
   tests that can pass while the claimed mechanism is absent.

## Proposed repository shape

```text
.
├── CMakeLists.txt
├── CMakePresets.json
├── CLAUDE.md                         # project behavior and review rules
├── README.md
├── LICENSE
├── cmake/
│   ├── toolchains/
│   │   ├── host.cmake
│   │   ├── stm32.cmake
│   │   ├── imx8.cmake
│   │   └── ti-dsp.cmake
│   └── modules/
├── config/
│   ├── project_config.h.in
│   └── linker/                       # target-specific linker templates
├── include/project/
│   ├── api/
│   └── platform/
├── src/
│   ├── core/                         # portable state machines and algorithms
│   ├── ipc/                          # message contracts and freshness rules
│   └── services/
├── platforms/
│   ├── imx8/
│   │   ├── app/
│   │   └── board/
│   ├── stm32/
│   │   ├── app/
│   │   ├── bsp/
│   │   └── cubeide/                  # optional generated/vendor project
│   └── ti-dsp/
│       ├── app/
│       ├── hal/
│       └── ccs/                      # optional CCS project metadata
├── tests/
│   ├── unit/                         # host-runnable Unity tests
│   ├── fakes/                        # deterministic hardware doubles
│   └── hardware/                     # explicitly gated board tests
├── scripts/
│   ├── format.sh
│   ├── lint.sh
│   └── run_tests.sh
├── docs/
│   ├── architecture/
│   ├── interfaces/
│   ├── testing/
│   └── decisions/
└── .github/workflows/
    ├── host.yml
    └── review.yml
```

## Build and test flow

The default developer path is vendor-neutral:

```text
cmake --preset host-debug
cmake --build --preset host-debug
ctest --preset host-debug --output-on-failure
```

CI must build with warnings enabled, run Unity tests, run static analysis, and
report coverage where the host toolchain supports it. Hardware builds are
separate jobs and must state the required SDK, compiler, board, and runner.

Vendor projects consume the same `include/`, `src/`, and platform adapter code;
they must not become a second implementation of core behavior.

## IPC and freshness contract

Every cross-CPU data structure gets an explicit contract documenting:

- owner of each field and its write/read ordering;
- sequence or heartbeat value;
- hardware IPC flag/acknowledgement behavior;
- timeout and stale-data response;
- whether the consumer may process the last-known-good value;
- memory barriers/cache maintenance required on that processor pair.

The template includes a small reference freshness monitor, but it is an
example—not a universal timing policy. Production code must choose thresholds
from system requirements and test boundary cases: unchanged heartbeat,
wraparound, missed acknowledgement, producer restart, and consumer overrun.

## TDD and review workflow

Each change should contain:

1. a short problem statement and observable acceptance criteria;
2. a failing Unity test or a documented reason a test cannot be deterministic;
3. the smallest implementation change;
4. host test, static-analysis, and build evidence;
5. an adversarial review that attempts to make the new test pass with the
   production mechanism removed;
6. explicit evidence for any hardware-only claim.

`CLAUDE.md` will encode the linked hostile-review guidance: state assumptions,
prefer simple and surgical changes, define verifiable goals, avoid speculative
abstractions, and do not confuse a green test with proof of the mechanism.

## Error handling and observability

The template favors explicit result/status types over hidden global state.
Logs and telemetry must identify source CPU, destination CPU, message sequence,
timestamp, and stale/timeout reason. Fault paths must be testable without
waiting on real time by injecting a clock.

## Out of scope for the first template pass

- selecting a particular STM32 family or i.MX8 variant;
- shipping vendor SDKs or generated CubeMX/CCS output;
- imposing one RTOS across all processors;
- production bootloader, secure-boot, or field-update policy;
- a finished application feature beyond a small buildable example and tests.
