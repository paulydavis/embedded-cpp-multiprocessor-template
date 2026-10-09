# Vendor Builds

Host CMake is the required review gate. A vendor build is an additional
qualification step with explicit SDK variables:

```sh
cmake --preset host-debug
cmake --preset stm32 --toolchain cmake/toolchains/stm32.cmake \
  -DSTM32_TOOLCHAIN_ROOT=/opt/gcc-arm-none-eabi
```

The STM32 wrapper may be opened in CubeIDE, the i.MX8 wrapper in the chosen
Yocto/SDK environment, and the DSP wrapper in CCS. Generated vendor metadata
must identify its SDK version and remain outside portable logic.

