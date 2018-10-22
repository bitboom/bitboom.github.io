---
layout: post
title:  Control Flow Integrity
tags: [cfi]
---

# Introduction
- Programmer -> Meomory management 
	- The feature of low-level language (c, c++)
- Attacker -> Memory corruption attack
	- Buffer overflow, integer overflow
	- Control-flow hijack
- Common defence mechanisms
	- Address Space Layout Randomization (ASLR)
	- Data Execution Prevention (DEP)
- Extended defence mechanisms
	- Control Flow Integrity
	- CPS/CPI - Safe Stack
	- Sanitizer

---

# What is control-flow? transfer? hijack? (1)
- Instruction set architecture (ISA)
	- Computational instructions (one after the other)
	- Control transfer instructions (transfer to target location)

- Control transfer instructions
	- Direct transter instruction
		- Target address is fixed at compile-time, launch-time
	- Indirect transfer instruction
		- Target address is fixed at runtime
		- Dependency with run-time value (register, memory)

---

# What is control-flow? transfer? hijack? (2)
- Control-flow
	- The order in which individual instructions executed

- Control-flow graph (CFG)
	- Using graph notation, all paths that might be traversed through a program during its execution.

---

# Control transfer instruction protection
- Direct transter instruction
	- Not complicated
	- Memory pages(SW), MMU(HW) -> read-only permission
- Indirect transfer instruction
	- Complicated....
	- Because of
		- Indirect function call (function pointer)
		- Virtual table call

---

>## [Attack model with memory corruption attack][1]

Exploit types | Defence mechanism
---|---
Code corruption attack | Instruction Set Randomize
<span style="color:red">Control-flow hijack attack</span> | ASLR, <span style="color:red">Control-flow Integrity</span>
Data-only attack | Data Integrity, Data-flow Integrity
Information attack | Data Space Randomization

- CFI is needed for protecting advanced ROP, heap spraying

[1]:https://people.eecs.berkeley.edu/~dawnsong/papers/Oakland13-SoK-CR.pdf

---

# Control-flow hijack attack
1. Modify a code pointer to the address of shellcode/gadget
	- Defense by ASLR
2. Use pointer by indirect transfer instruction(call/jump or return)
	- Defense by Control-flow Integrity

---

## Attacker tries memory corruption for control-flow hijacking

---

# Control-Flow Integrity
- <span style="color:red">Ensure that the control-flow</span> of the application stays within the CFG.
- <span style="color:red">Restrict the set of targets</span> to which an attacker might redirect the control-flow of a program.

---

# How to enfoce the CFI?

1. Generate CFG
2. <span style="color:red"> Enumerate all possible targets </span>
3. Enforce CFI
4. Execute a runtime monitor

---

# Before enforcing CFI

```cpp
void bar(); void baz(); void buz(); void bez(int, int);

void foo(int usr) {
  void (*func)();

  if (usr == MAGIC)
    func = bar;
  else
    func = baz;

  func();
}
```

---

# After enforcing CFI

```cpp
void bar(); void baz(); void buz(); void bez(int, int);

void foo(int usr) {
  void (*func)();

  if (usr == MAGIC)
    func = bar;
  else
    func = baz;

  // a) all functions {bar, baz, buz, bez, foo} are allowed
  // b) all functions with prototype "void (*)()" are allowed
  //    i.e., {bar, baz, buz}
  // c) only address taken functions are allowed
  //    i.e., {bar, baz}
  CHECK_CFI_FORWARD(func);
  func();

  // backward edge CFI check
  CHECK_CFI_BACKWARD();
}
```

---

# Kind of CFI
## Forward-edge CFI
- Monitor a new location by using indirect jump and indirect call instructions
	- Indirect function call, virtual table call, switch statement

## Backward-edge CFI
- Monitor the location that was used in a forward-edge earlier by using return instructions

---

## [CFI implementations comparison][2]

[2]:https://nebelwelt.net/publications/files/17CSUR.pdf

---

# [Enforcing Forward-Edge Control-Flow Integrity in GCC & LLVM (Google 2014)][3]
> CFI implementations have been research prototypes . . .
> Implementations of **fine-grained, forward-edge CFI enforcement** and analysis for GCC and LLVM.

- VTV: Virtual-Table Verification - GCC
- IFCC: Indirect Function-Call Checks - LLVM
- FSan: Indirect-Call-Check Analysis - LLVM

[3]:https://static.googleusercontent.com/media/research.google.com/ko//pubs/archive/42808.pdf

---

# Think over..
- Cpp Exceptions
- Interrupts
- Dynamic loading library
- . . .
 
---

# Refereces
- Enforcing Forward-Edge Control-Flow Integrity in GCC & LLVM, Google Inc, USENIX 2014
- A Fine-Grained Control Flow Integrity Approach Against Runtime Memory Attacks for Embedded Systems, IEEE 2016
- SoK: Eternal War in Memory, ACM 2013
- Code-Pointer Integrity, USENIX 2014
- Control-Flow Integrity: Precision, Security, and Performance
