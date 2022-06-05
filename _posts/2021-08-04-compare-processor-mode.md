---
layout: post
title: Compare processor modes between Cortex-m & -a
tags: [embedded]
---

## Common executing mode

Both Cortex-M33 & A9 processor support two executing mode. (code execution)
- Privileged mode
  - Privileged execution has access to all resources available to the security state
  - All resources => Banked registers in the System Control Space
- Unprivileged mode
  - Unprivileged execution limits or excludes access to **some resources** appropriate to the current security state
  - Some resources => registers

## [Cortex-m33: Modes of operation and execution](https://developer.arm.com/documentation/100230/0002/functional-description/programmers-model/modes-of-operation-and-execution)

The Cortex-M33 processor supports two operating modes.
- Thread operating mode
  - Condition
    - The processor enters Thread mode **on reset**
    - As a result of an exception return
  - Privileged and Unprivileged code can run
- Handler operating mode
  - Condition
    - As a result of an exception return
  -  All code is privileged in Handler mode

## [Cortex-A9: ARMv7-A; ARM processor modes](https://developer.arm.com/documentation/ddi0406/b/System-Level-Architecture/The-System-Level-Programmers--Model/ARM-processor-modes-and-core-registers/ARM-processor-modes?lang=en)

The Cortex-A9 processor supports eight processor modes.
- User(Unprivileged): Execute application code
- FIQ: Entered as a result of a fast interrupt
- IRQ: Entered as a result of a normal interrupt
- Supervisor: 
  - Execute kernel code
  - Entered on Reset
  - Execution of a Supervisor Call (SVC) instruction; System Call
- Monitor:
  - A Secure mode that enables change between Secure and Non-secure states
  - Entered on execution of a Secure Monitor Call (SMC) instruction
- Abort: Entered as a result of a Data Abort exception or Prefetch Abort exception
- Undefined: Entered as a result of an instruction-related error
- System: Application code that requires privileged access
