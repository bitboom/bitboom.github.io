---
layout: post
title: SGX Application Overview
tags: [sgx]
---

This document describe
the contents what SGX Application developer
should know.

## SGX Application Structure 
SGX application is consist of two components.
- Trusted Component: This is the enclave. It is just shared object aka *.so, *.dll.
- Untrusted Component: This is the rest of the application and any of its modules.


In intel sdk,
**Enclave directory** indicates trusted component and
**App directory** indicates untruste component.
```
 +-----+    +---------+
 | App | -- | Enclave |       <= Source code directory
 +-----+    +---------+
 =======    ===========
 object     shared obejct     <= Compile stage
 ========================
 application binary           <= Final SGX application
```

## Interface between TC and UC
Shared library(object) has exposed function called API.
Also, enclave have API called Ecall.
Developer should define **Ecall** that UC calls TC. 
The opposite also can be defined by **Ocall** that TC calls UC.

So, how can we define interface in intel's sdk? 

#### Enclave Definition Language (EDL file)
SGX application developer should define
interface between TC and UC in EDL file.
Refer, [sample edl file](https://github.com/intel/linux-sgx/blob/master/SampleCode/SampleEnclave/Enclave/Enclave.edl).

```
enclave {
  trusted {
    // Trusted function prototypes (ECALLs)
  };

  untrusted {
    // Untrusted function prototypes (OCALLs)
  };
};
```
After compile this,
Edger8r in the sdk makes glue code
about edl file
for TC and UC.  
If the name of edl file is "Enclave.edl", below files are generated.
- TC side: "Enclave_t.h" and "Enclave_t.cpp"
- UC side: "Enclave_u.h" and "Enclave_u.cpp"

```
 +-----+-----------+    +-----------+---------+
 | App | Enclave_u | -- | Enclave_t | Enclave |
 +-----+-----------+    +-----------+---------+
```

## UC side with App directory
The main() entry is exist in "[App/App.cpp](https://github.com/intel/linux-sgx/blob/master/SampleCode/SampleEnclave/App/App.cpp)".
There are too many skeleton code. 
The code developer needs to see is just four lines.

```cpp
#include "Enclave_u.h"

/* Utilize trusted libraries */
ecall_libc_functions();
ecall_libcxx_functions();
ecall_thread_functions();
```

If you follow the function implementation, the part that calls the **Ecall** comes out.

## TC side with Enclave directory
TC side consists of library's code wich run in Enclave.
The sample codes are here "[Enclave/TrustedLibrary](https://github.com/intel/linux-sgx/tree/master/SampleCode/SampleEnclave/Enclave/TrustedLibrary)".
There is the pair of *.cpp and *.edl.

## Enclave Signature
> Measurement: As an enclave is instantiated
> in a trusted environment,
> an accurate and protected recording of
> its identity is taken.

Intel SGX architecture provides [digital siginature](https://en.wikipedia.org/wiki/Digital_signatur)
for Enclave author (developer)
called Enclave Signature.

For digital signature, private key is needed.
The sample private key is
"[Enclave/Enclave_private.pem](https://github.com/intel/linux-sgx/blob/master/SampleCode/SampleEnclave/Enclave/Enclave_private.pem)".
While build the enclave,
sdk makes self-signed certificate using this.
It is known as the Enclave Signature (SIGSTRUCT).
The public key of author is registered
to the MRSIGNER register
after the enclave is initialized.

**Enclave Integrity**:
Using Enclave Signature,
Intel SGX architecture detects
whether any portion of the enclave file
has been tampered with.

After building sgx application,
we can find out
two shared library about enclave.
One is `enclave.so` and anthor is `enclave.signed.so`.
`enclave.signed.so` is signed by
author's private key.

## Reference
- [SGX 101 - Application design](http://www.sgx101.com/portfolio/application_design/)
- [SGX 101 - Enclave lifecycle](http://www.sgx101.com/portfolio/enclave_lifecycle/)
- [Intel Enclave Signature](https://software.intel.com/en-us/node/702979)
- [Intel sgx sdk developer reference](https://01.org/sites/default/files/documentation/intel_sgx_sdk_developer_reference_for_linux_os_pdf.pdf)
