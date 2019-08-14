---
layout: post
title: Anatomy of kpatch
tags: [kernel]
---

## [kpatch](https://github.com/dynup/kpatch): dynamic kernel patching
- A feature of the Linux kernel (mainline: [livepatch](https://elixir.bootlin.com/linux/latest/source/include/linux/livepatch.h))
- **Live patching** of a running kernel
	- Apply critical security patches
- **Without rebooting or restarting any processes**
	-  Give more control over uptime without sacrificing security

---

## [Why Live Patching Required?](https://events.static.linuxfound.org/sites/events/files/slides/LinuxConNA-kpatch-without-stopmachine_fixed.pdf)
- Major updates needs rebooting
- Some appliances can't accept 10ms downtime
- In between the major updates, live patching will be used
- **Live patching and major update are complement each other**
	- Live patching temporarily fixes small critical incidents
	- Major update permanently fixes all bugs

```sh
=== x ===== xx ======>> Major Update >>====================
  minor  critical
  --------------->
   live patching     
```

---

## History
- Red Hat announced kpatch in February 2014 under GPLv2
	- shortly before SUSE released kGraft
- kpatch was submitted in 2014 ([submission](https://lwn.net/Articles/597407/))
- kpatch has been included in RHEL Linux 7.0 in 2014
- livepatch were merged into the kernel 4.0 in 2015
	- livepatch: minimalistic foundations for live kernel patching
	- [Only the x86 architecture is supported](https://kernelnewbies.org/Linux_4.0#head-9aa7c8499b42911a48c02b24f367bf2bc6db8606)

---

## kpatch components
1. patch: a file to be patched (.patch file)
2. kpatch-build: a tool for building patch modules
3. patch module: a kernel module (.ko file)
4. [kpatch core module](https://github.com/dynup/kpatch/blob/master/kmod/core/core.c): a kernel module (.ko file)

---

## kpatch workflow
```sh
+---------+    +-----------------------+
| patch   | => | kpatch-build          |
+---------+    | ============          |
| *.patch |    | * Apply patch         |
+---------+    | * Create patch module |
               +-----------------------+
                       V
               +--------------+    +-------------------+
               | patch module | => | kpatch(core)      |
               +--------------+    | ===========       |
               | *.ko         |    | * Register module |
               +--------------+    | * Do livepatch    |
                                   +-------------------+
                                   | kpatch.ko         |
                                   +-------------------+
```

---

## kpatch internal: working at a function granularity
```sh
                    foo()                   ftrace
+------------+    +-------------+ hook +-----------------+
| call foo() | => | call fentry | ===> | save regs       |
+------------+    +-------------+      +-----------------+   
| ...        |    | // ...      |      | call ftrace_ops | --
|            |    +-------------+      +-----------------+  |
|            |    | // ret      |      | restore regs    |  |
|            |    +-------------+      +-----------------+  |
|            |                         | ret regs->ip    |  |
|            | <=   new foo()     <=== | * new foo()     |  |
|            |    +-------------+      +-----------------+  |
+------------+    | ...         |                           |
                  +-------------+      +-----------------+  |
                  | ret         |      | get new ip      |<--
                  +-------------+      | from hash table |
                                       +-----------------+
                                       | change regs->ip |
                                       +-----------------+
                                       | ret             |
                                       +-----------------+
```

---

## Conflict between old and new functions
We should ensure old functions are not excuted while patching
- Executing functions are on the stack

## Active Safeness Check
- Check stack to finding old functions
- This needs to call stop_machine
 
---

## Function registering
 1. Call [stop_machine](https://elixir.bootlin.com/linux/latest/source/include/linux/stop_machine.h)
 2. Ensure that no task has the old function in its call stack
 3. Register old function to ftrace entry
 4. Add the old and new function to hash table

---

## Function redirecting
 1. The old function calls into kpatch_ftrace_handler()
 2. Find the new function in kpatch_func_hash table
 3. Update the return instruction pointer
 	-  ftrace will return to the new function

---

# Anatomy of KPATCH - Code level

---

# Coding convention
For using livepatch kernel API
1. Read in kpatch_patch structures
2. Arranges them into the scaffold structures (internal)
3. Creates a livepatch structure

| header | type | variable |
|:---:|:---:|:---:| 
| livepatch.h | klp_* | being with "l" (e.g. lfunc) |
| kpatch-patch.h | kpatch_patch_* | being with "k" (e.g. kfunc) |

---

##  Main kernel functions used
- [semahpore](https://elixir.bootlin.com/linux/latest/source/include/linux/semaphore.h#L28)
- [hashtable](https://elixir.bootlin.com/linux/latest/source/include/linux/hashtable.h)
- [rcu](https://www.kernel.org/doc/Documentation/RCU/whatisRCU.txt)
- [container_of](http://bitboom.github.io/analyze-containerof)
- [do_each_thread](https://elixir.bootlin.com/linux/latest/source/include/linux/sched/signal.h#L571)
- [stop_machine](https://elixir.bootlin.com/linux/latest/source/include/linux/stop_machine.h#L152)
- [smp_wmb](https://elixir.bootlin.com/linux/latest/source/tools/include/asm/barrier.h#L43)

---

## Data Structure - [Patch function](https://github.com/dynup/kpatch/blob/master/kmod/core/kpatch.h)
```c
struct kpatch_func {
  /* public */
  unsigned long new_addr;
  unsigned long new_size;
  unsigned long old_addr;
  unsigned long old_size;
  
  const char *name;
  struct list_head list;  // double linked list
  
  /* private */
  struct hlist_node node; // hashmap
  enum kpatch_op op;      // function status
  struct kobject kobj;    // sysfs
};
```

---

## Data Structure - [Patch module](https://github.com/dynup/kpatch/blob/master/kmod/core/kpatch.h)
```c
struct kpatch_object {
  struct list_head list;
  const char *name;
  // entries for functions to be patched in the object
  struct list_head funcs; 
  struct list_head dynrelas;

  // struct klp_callbacks 
  // functions to be executed pre/post (un)patching
  int (*pre_patch_callback)(...);
  void (*post_patch_callback)(...);
  void (*pre_unpatch_callback)(...);
  void (*post_unpatch_callback)(...);

  /* private */
  struct module *mod;  // kernel module
  struct kobject kobj; // sysfs
};
```

---

## Data Structure - [kpatch - Core module](https://github.com/dynup/kpatch/blob/master/kmod/core/kpatch.h)
```c
struct kpatch_module {
  /* public */
  struct module *mod;
  struct list_head objects; // entries for objects

  /* public read-only */
  bool enabled;

  /* private */
  struct list_head list;
  struct kobject kobj;
};
```

---

## Pseudocode - [kpatch_register #1](https://github.com/dynup/kpatch/blob/master/kmod/core/core.c)
```c
int kpatch_register(struct kpatch_module *kpmod, ...)
{
  // pre-condition
  down(&kpatch_mutex);
  try_module_get(kpmod->mod);

  // iterate patch modules
  list_for_each_entry(object, &kpmod->objects, list) {
    // Prepare patch module to be patched
    kpatch_link_object(kpmod, object);
    
    // set function state
    list_for_each_entry(func, &object->funcs, list)
      func->op = KPATCH_OP_PATCH;
  }
 
// continue...
```

---

## Pseudocode - [kpatch_register #2](https://github.com/dynup/kpatch/blob/master/kmod/core/core.c)
```c
  // memory barrier between func hash and state write
  smp_wmb();

  kpatch_state_updating();

  // verify activeness safety, and atomically make the new
  // functions visible to the ftrace handler.
  stop_machine(kpatch_apply_patch, kpmod, NULL);
  
  kpatch_state_idle();

  up(&kpatch_mutex);
  return 0;
}
```
---

## Pseudocode - kpatch_link_object
Link to a to-be-patched object in preparation for patching it.
```c
static int kpatch_link_object(struct kpatch_module *kpmod,
                              struct kpatch_object *object)
{
  mutex_lock(&module_mutex);
  // find the object module
  find_module(object->name);
  try_module_get(mod);
  mutex_unlock(&module_mutex)
  
  list_for_each_entry(func, &object->funcs, list) {
    // lookup the old address by kallsyms_on_each_symbol 
    kpatch_find_object_symbol(...&func->old_addr);
    
    // add to ftrace filter and register handler if needed
    kpatch_ftrace_add_func(func->old_addr);
  }
}
```

---

## Pseudocode - kpatch_ftrace_add_func
Register function to ftracy by setting filter
```c
static struct ftrace_ops kpatch_ftrace_ops __read_mostly = {
  .func = kpatch_ftrace_handler,
  .flags = FTRACE_OPS_FL_SAVE_REGS | FTRACE_OPS_FL_IPMODIFY,
}

int kpatch_ftrace_add_func(unsigned long ip)
{
  ftrace_set_filter_ip(&kpatch_ftrace_ops, ip, 0, 0);
}
```

---

## Pseudocode - kpatch_ftrace_handler
This is where the magic happens.  
Update regs->ip to tell ftrace to return to the new function.
```c
void kpatch_ftrace_handler(unsigned long ip ...,
                           struct pt_regs *regs)
{
  preempt_disable_notrace();
  
  // find func from hash (hash_for_each_possible_rcu)
  func = kpatch_get_func(ip);
  regs->ip = func->new_addr + MCOUNT_INSN_SIZE;
  
  preempt_enable_notrace();
}
```

- [MCOUNT_INSN_SIZE](https://elixir.bootlin.com/linux/v4.6/source/arch/arm/include/asm/ftrace.h#L6) -  sizeof mcount call

---

## Pseudocode - kpatch_apply_patch
```c
static int kpatch_apply_patch(void *data)
{
  kpatch_verify_activeness_safety(kpmod);
  
  // run any user-defined pre-patch callbacks
  list_for_each_entry(object, &kpmod->objects, list)
    pre_patch_callback();
  
  do_for_each_linked_func(kpmod, func)
    hash_add_rcu(khash, &func->node, func->old_addr);
  
  // memory barrier between func hash add and state change
  smp_wmb();
  kpatch_state_finish(KPATCH_STATE_SUCCESS);
  
  // run any user-defined post-patch callbacks
  list_for_each_entry(object, &kpmod->objects, list)
    post_patch_callback(object);
}
```

---

## Pseudocode - kpatch_verify_activeness_safety
Verify activeness safety, that none of the to-be-patched functions are on the stack of any task.
This function is called from stop_machine() context.
```c
int kpatch_verify_activeness_safety(... *kpmod)
{
  struct task_struct *g, *t;
  
  // check the stacks of all tasks
  do_each_thread(g, t) {
    save_stack_trace_tsk(t, &trace);
    for (i = 0; i < trace.nr_entries; i++)
      // iterate kpatch functions compare to second argument
      kpatch_backtrace_address_verify(kpmod,
                                      trace.entries[i]);
  } while_each_thread(g, t);
}
```

---

# Ref #1
https://www.kernel.org/doc/Documentation/livepatch/livepatch.txt
https://www.kernel.org/doc/Documentation/memory-barriers.txt
https://www.kernel.org/doc/html/v4.11/driver-api/basics.html
https://www.kernel.org/doc/Documentation/trace/ftrace.txt

---

# Ref #2
[redhet #1](https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/7/html-single/kernel_administration_guide/index#chap-Documentation-Kernel_Administration_Guide-Working_With_Kpatch)
[redhet #2](https://www.redhat.com/en/blog/introducing-kpatch-dynamic-kernel-patching?source=blogchannel&channel=blog/channel/red-hat-enterprise-linux&search=kpatch)
[redhet #3](https://www.redhat.com/en/blog/live-kernel-patching-update?source=blogchannel&channel=blog/channel/red-hat-enterprise-linux&search=kpatch)

[linuxfound](https://events.static.linuxfound.org/sites/events/files/slides/kpatch-linuxcon_3.pdf)
