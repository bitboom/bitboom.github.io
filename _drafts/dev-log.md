## OS dev
- From bare-bones
  - Create bare metal binary
  - How to execute bare-metal binary?
    - Boot Image
      - Boot loader + Kernel(from bare-metal binary)
      - Boot Loader: Init RESET VECTOR, Stack init per Processor Mode, Call Kernel
    - Elf Binary
      - Linker
  - Where to execute?
    - QEMU w/ Boot Image
  - How to debug?
    - GDB