---
layout: post
title: Three Pillars of Private Cloud Compute
tags: [pcc]
---

Private Cloud Compute (PCC) is a security model
designed to isolate user data from untrusted parties, even in a cloud environment.

To achieve this goal, PCC is built on three mutually reinforcing pillars.  
Each pillar provides an independent security property,
but the PCC trust model only holds when all three are applied together.


## 1. Confidential Computing

The server does not permit any external party,
including the cloud operator, to access or observe data under processing,
the computation state, or the memory regions in which such state resides.

This constraint is enforced through hardware-based TEE isolation and a measured, attested boot chain.  
As a result, access to data and computation state during processing is structurally prevented.


## 2. Request Unlinkability

The server cannot identify a specific user or infer relationships between incoming requests.

This constraint is enforced by ensuring that requests are delivered exclusively via blind relays,
and that the server only accepts requests through an anonymizing ingress.  
As a result, from the server’s perspective, each request exists as an independent and unlinkable unit.


## 3. Hardened OS

The server cannot execute software that has not been explicitly approved,
and it cannot possess paths through which unintended data exfiltration may occur.

This constraint is enforced at the operating system level,
and applied through mandatory security controls across execution, filesystem access, and networking.

