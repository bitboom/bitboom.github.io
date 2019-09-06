---
layout: post
title: An overview of kpatch-build
tags: [kptach]
---

# kptach-build
[Kpatch](http://bitboom.github.io/anatomy-of-kpatch) is a feature of the Linux kernel for livepatching made by Red Hat.  
kpatch-build is one of the kpatch modules that converts patches files into kernel module.
```c
+---------+    +---------------------+    +--------------+
| patch   |    | kpatch-build        |    | patch module |
+---------+ => | ============        | => +--------------+
| *.patch |    | Create patch module |    | *.ko         |
+---------+    +---------------------+    +--------------+
```
Look at the last section "Kernel patch example" first for easy understanding.

## How to make kernel module
1.  Download and unpack kernel source matching with patches's distro.
2.  Test patch file with option [dry-run](https://www.gnu.org/software/diffutils/manual/html_node/Dry-Runs.html)
3.  Read special section data with command (readelf -wi "$VMLINUX")
	- alt_instr, bug_entry size,  jump_entry size ...
4. Build original source with compile options "-ffunction-sections and -fdata-sections"
5. Build patched source with compile options "-ffunction-sections and -fdata-sections"
6. Extract new and modified ELF sections
	- Compare #4's output and #5's output at a section level
	- Result: Elf object included {.kpatch.strings, .kpatch.symbols, .kpatch.relocations}
8. Build patch module with #6's output

## Core data structure: [kpatch-elf]([https://github.com/dynup/kpatch/blob/master/kpatch-build/kpatch-elf.h](https://github.com/dynup/kpatch/blob/master/kpatch-build/kpatch-elf.h))
kpatch-build uses own data structure which added special data structures to elf format. The special data structures are able to include difference section between the origin object and the patched object.  
The intermediate objects of kpatch-build are used in the form of kpatch-elf.
```c
struct kpatch_elf {
  Elf *elf;
  struct list_head sections;
  struct list_head symbols;
  struct list_head strings;
  int fd;
};
```

## Core module: [create-diff-object.c]([https://github.com/dynup/kpatch/blob/master/kpatch-build/create-diff-object.c](https://github.com/dynup/kpatch/blob/master/kpatch-build/create-diff-object.c))
This file contains the heart of the ELF object differencing engine.
- The tool takes two ELF objects from two versions of the same source file.
	-  a "base" object and a "patched" object
-  These object need to have been compiled with the GCC options.
	-  -ffunction-sections and -fdata-sections 
- The tool compares the objects at a section level to determine what sections have changed.
- Once a list of changed sections has been generated, various rules are applied.


## [Kernel patch example](https://weekly-geekly.github.io/articles/317034/index.html)
```sh
# Install kpatch and kpatch build
$ sudo apt-get install kpatch kpatch-build`

# Clone the Ubuntu source code
$ git clone git://kernel.ubuntu.com/ubuntu/ubuntu-xenial.git

# Make a copy
$ mkdir ubuntu-xenial-kpatch
$ cp -r ubuntu-xenial ubuntu-xenial-kpatch

# Modify the source code
$ vim ubuntu-xenial-kpatch/ubuntu-xenial/fs/proc/version.c
  ...
  static int version_proc_show(struct seq_file *m, void *v) {
    seq_printf(m, linux_proc_banner, "This has been patched!",
               utsname()->sysname, utsname()->release, utsname()->version);
    return 0;
  }
  ...

# Create a patch
$ diff -u ubuntu-xenial/fs/proc/version.c ubuntu-xenial.kpatch/ubuntu-xenial/proc.version.c > version.patch

# Make kernel module by kpatch-build
$ kpatch-build -t vmlinux --skip-gcc-check version.patch 
  Debian/Ubuntu distribution detected
    Downloading the kernel source for 4.4.0-51-generic
    Unpacking kernel source
    Testing patch file
    checking file fs/proc/version.c
    Reading special section data
    Building original kernel
    Building patched kernel
    Detecting changed objects
    Rebuilding changed objects
    Extracting new and modified ELF sections version.o:
       changed function: version_proc_show
    Building patch module: kpatch-version.ko SUCCESS`

# Apply a patch
$ sudo insmod kpatch-version.ko`

# Test
$ cat /proc/version
  This has been patched! version Linux
```
