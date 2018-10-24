---
layout: post
title: An Overview of GCC plugin
tags: [gcc]
---

# GCC plugin infrastructure
- GCC plugins are loadable modules that provide extra features to the compiler.
- GCC plugins provide developers with a rich subset of the GCC API to allow them to extend GCC as they see fit. (Not kindly API documents and refereces)
- Whether it is writing an additional optimization pass, transforming code, or analyzing information, plugins can be quite useful.
- **GCC plugins are useful for runtime instrumentation and static analysis.**
- We can **(1)analyse**, **(2)change and add further code** during compilation via callbacks, GIMPLE, IPA and RTL passes.

--- 

# Restriction
## Version
- The GCC plugin infrastructure of the kernel supports all gcc versions from 4.5 to 6.0.
- GCC versions 4.5 and 4.6 are compiled by a C compiler.
- GCC-4.7 can be compiled by a C or a C++ compiler
- GCC versions 4.8+ can only be compiled by a C++ compiler.

## Architecture
- Currently the GCC plugin infrastructure supports only the **x86, arm, arm64 and powerpc** architectures.

---

# Plugin API
- Plugins are activated by the compiler at specific events as defined in "gcc-plugin.h".
- For each event of interest, **the plugin should call register_callback** specifying the name of the event and address of the callback function that will handle that event.
- The header "gcc-plugin.h" must be the first gcc header to be included.

---

# Plugin license check
- Every plugin should define the global symbol *plugin_is_GPL_compatible*.
- *plugin_is_GPL_compatible* asserts that it has been licensed under a GPL-compatible license.

---

# Plugin initialization
- Every plugin should export a function called plugin_init that is called right after the plugin is loaded.
- This function is responsible for registering all the callbacks required by the plugin and do any other required initialization.
- This function is **called from *compile_file()* right before invoking the parser.**

---

# Plugin callbacks
```c
/* The prototype for a plugin callback function.
     gcc_data  - event-specific data provided by GCC
     user_data - plugin-specific data provided by the plug-in.  */
typedef void (*plugin_callback_func)(void *gcc_data, void *user_data);
```
- To register a callback, the plugin calls register_callback with the arguments.
- The PLUGIN_FINISH event is the last time that plugins can call GCC functions, notably emit diagnostics with warning, error etc.

---

# Main plugin event
- PLUGIN_START_PARSE_FUNCTION, /* Called before parsing a function. */
- PLUGIN_PASS_MANAGER_SETUP, /* To hook into pass manager.  */
- PLUGIN_PRE_GENERICIZE, /* Allows to see low level AST in C and C++ frontends.  */
- PLUGIN_ATTRIBUTES, /* Called during attribute registration */
- PLUGIN_NEW_PASS, /* Called when a pass is first instantiated.  */
- PLUGIN_OVERRIDE_GATE, /* Allows to override pass gate decision for current_pass.  */
- PLUGIN_PASS_EXECUTION, /* Called before executing a pass.  */

---

# Interacting with the pass manager
 - A plugin registers a new pass with GCC by calling register_callback with the PLUGIN_PASS_MANAGER_SETUP event and a pointer to a struct register_pass_info object defined as follows.
 
 ```c
     enum pass_positioning_ops
     {
       PASS_POS_INSERT_AFTER,  // Insert after the reference pass.
       PASS_POS_INSERT_BEFORE, // Insert before the reference pass.
       PASS_POS_REPLACE        // Replace the reference pass.
     };
     
     struct register_pass_info
     {
       struct opt_pass *pass;            /* New pass provided by the plugin.  */
       const char *reference_pass_name;  /* Name of the reference pass for hooking
                                            up the new pass.  */
       int ref_pass_instance_number;     /* Insert the pass at the specified
                                            instance number of the reference pass.  */
                                         /* Do it for every instance if it is 0.  */
       enum pass_positioning_ops pos_op; /* how to insert the new pass.  */
     };
     
     
     /* Sample plugin code that registers a new pass.  */
     int
     plugin_init (struct plugin_name_args *plugin_info,
                  struct plugin_gcc_version *version)
     {
       struct register_pass_info pass_info;
     
       ...
     
       /* Code to fill in the pass_info object with new pass information.  */
     
       ...
     
       /* Register the new pass.  */
       register_callback (plugin_info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL, &pass_info);
     
       ...
     }
 ```

---

# Registering custom attributes
 - The PLUGIN_ATTRIBUTES callback is called during attribute registration. Use the register_attribute function to register custom attributes.

```c
     /* Attribute handler callback */
     static tree
     handle_user_attribute (tree *node, tree name, tree args,
                            int flags, bool *no_add_attrs)
     {
       return NULL_TREE;
     }
     
     /* Attribute definition */
     static struct attribute_spec user_attr =
       { "user", 1, 1, false,  false, false, handle_user_attribute };
     
     /* Plugin callback called during attribute registration.
     Registered with register_callback (plugin_name, PLUGIN_ATTRIBUTES, register_attributes, NULL)
     */
     static void
     register_attributes (void *event_data, void *data)
     {
       warning (0, G_("Callback to register attributes"));
       register_attribute (&user_attr);
     }
```

---

# Reference
- http://www.infradead.org/~mchehab/kernel_docs/unsorted/gcc-plugins.html
- https://gcc.gnu.org/onlinedocs/gccint/Plugins.html
- https://gcc.gnu.org/onlinedocs/gccint/Plugin-API.html#Plugin-APi
- https://gcc.gnu.org/onlinedocs/gccint/GIMPLE.html
- https://gcc.gnu.org/onlinedocs/gccint/IPA.html
- https://gcc.gnu.org/onlinedocs/gccint/RTL.html
