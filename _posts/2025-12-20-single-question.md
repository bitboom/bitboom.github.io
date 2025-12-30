---
layout: post
title: PCC; Born from a single question
tags: [pcc]
---

The rise of generative AI introduced a problem that did not previously exist.  
Model sizes and computational requirements grew rapidly,
and many workloads could no longer be handled by even high-end devices alone.

Engineers naturally turned to the cloud.  
Eventually, this led to a single question.

> “If user data leaves the device  
> and is processed on an AI server,  
> can we technically guarantee that it will not be exposed?”

This question goes far beyond a simple “can’t we just encrypt it?”  
Server operators, cloud administrators, service engineers, debugging tools, logs,
and crash dumps all become part of the threat surface.  
Every point where data may pass through must be considered.

In June 2024, Apple presented one answer to this question with **Private Cloud Compute**.  
In 2025, Meta introduced **Private Processing**, followed by Google’s **Private AI Compute**.  
The names differ, but all of these efforts originate from the same fundamental question.


## PCC (Private Cloud Compute)

Personally, this trend could be described as *Confidential AI Computing*.  
However, since **PCC** is the term most commonly used in the industry, this article adopts that terminology.

PCC involves three primary stakeholders.

- The user  
- The AI service provider  
- The cloud infrastructure provider  

The user sends a prompt from a device.  
The AI service provider executes a model on cloud infrastructure and returns a response.  
At a glance, this does not appear different from a conventional cloud service.

There is only one distinction.  
The system must answer the original question.


## New Requirements Introduced by AI Processing

The security requirements of traditional cloud services are no longer sufficient.  
During AI inference, user data must satisfy all of the following conditions.

- **Used only during inference**
  - It must not be reused for training, retraining, or log analysis.

- **Not persisted on the server**
  - It must not be written to disks, snapshots, or crash dumps.

- **Not exfiltrated over the network**
  - All egress paths other than the intended response channel must be blocked.

- **Inaccessible even to operators**
  - This includes cloud administrators, SREs, and internal service operators.

- **Verifiable rather than trust-based**
  - The guarantees must be technically provable and externally auditable.

Here, the term *insider* does not imply malicious intent.  
It includes any party that has legitimate technical access for development, operations, or incident response.

PCC aims to ensure that even these parties cannot access user data.

This is not a matter of tightening security configurations.  
It requires rethinking the system architecture as a whole.
