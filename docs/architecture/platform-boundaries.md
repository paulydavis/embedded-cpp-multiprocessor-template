# Platform Boundaries

Portable code depends only on headers under `include/project/`. Platform code
under `platforms/` supplies time, memory ordering, MMIO, interrupts, and SDK
integration. The i.MX8 application processor, STM32 MCU, and TI DSP may use
different RTOSes and vendor libraries; those differences must stay in adapters.

CubeIDE and CCS project files are wrappers that consume the same `src/` and
`include/` sources. They are not alternate implementations of state machines,
protocols, or safety policy.

