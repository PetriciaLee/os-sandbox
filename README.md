# os-sandbox
A collection of laboratory exercises focusing on core Operating System concepts, implemented in C++ within a Linux environment.


# 🖥️ Operating Systems - Core Concepts & Lab Exercises

[![Language](https://img.shields.io/badge/Language-C%2B%2B-blue.svg)](https://isocpp.org/)
[![OS](https://img.shields.io/badge/OS-Linux%20%2F%20Ubuntu-orange.svg)](https://ubuntu.com/)
[![Environment](https://img.shields.io/badge/Environment-Virtual%20Machine-purple.svg)](https://www.virtualbox.org/)

Welcome to my Operating Systems repository! This space contains practical implementations of core OS concepts developed during my university laboratory exercises. The focus is on low-level programming, system calls, and understanding how the kernel manages hardware and processes.

---

## 🛠️ Tech Stack & Environment

* **Operating System:** Linux (Ubuntu) running inside a **Virtual Machine** (VirtualBox/VMware).
* **Language:** C++ (utilizing POSIX standards for system programming).
* **Tools:** `gcc/g++`, `make`, `gdb` for debugging, and standard Linux CLI utilities.

---

## 📚 Laboratory Exercises Overview

| Exercise | Topic | Core Concepts Covered |
| :--- | :--- | :--- |
| **Lab 1** | [Interrupts & Signals](#-lab-1-interrupts--signals) | Hardware vs Software interrupts, POSIX signals, Reentrancy |
| **Lab 2** | [Process Management](#-lab-2-process-management) | *TBD (e.g., fork, exec, wait)* |
| **Lab 3** | [Thread Synchronization](#-lab-3-thread-synchronization) | *TBD (e.g., Semaphores, Mutexes)* |
| **Lab 4** | [Memory Management](#-lab-4-memory-management) | *TBD (e.g., Page replacement algorithms)* |
| **Lab 5** | [File Systems](#-lab-5-file-systems) | *TBD (e.g., Simulated file systems)* |

---

## 🔍 Detailed Exercise Breakdown

### 🚨 Lab 1: Interrupts & Signals

This exercise focuses on how operating systems handle asynchronous events through software interrupts (signals) and the critical architectural differences between real-time hardware interrupts and software-level abstractions.

#### Key Learning Outcomes & Techniques:
* **Signal Handling:** Implementing custom behaviors for POSIX signals using `sigset_t` and modern system calls (`sigaction`, `sigprocmask`) instead of deprecated legacy functions.
* **Signal Types:** Deep dive into synchronous (e.g., `SIGSEGV`, `SIGFPE`) vs. asynchronous signals (e.g., `SIGINT`, `SIGTERM`).
* **Reentrancy (Višeulaznost):** Understanding why functions like `printf` are dangerous inside signal handlers and mastering the use of async-signal-safe functions (like `write`).
* **Interrupt Simulation:** Simulating 32-bit hardware interrupt vector tables (Win32 context comparison) and nesting interrupts based on priorities.

> 💡 **Interview Takeaway:** Managed to successfully prevent race conditions within signal handlers by blocking critical signals during non-reentrant sections of the code.

---

### 🧵 Lab 2: Process Management
*(Popuni detalje kada završiš vježbu - npr. Kreiranje procesa pomoću `fork()`, zamjena slika procesa kroz `exec`, i sinkronizacija roditelj-dijete procesa)*

---

### 🔒 Lab 3: Thread Synchronization
*(Popuni detalje kada završiš vježbu - npr. Problem proizvođača i potrošača, filozofi na večeri, korištenje mutexa i semafora)*

---

### 🧠 Lab 4: Memory Management
*(Popuni detalje kada završiš vježbu - npr. Simulacija stranične organizacije memorije ili LRU algoritma)*

---

### 📁 Lab 5: File Systems
*(Popuni detalje kada završiš vježbu)*

---

## 🚀 How to Run

1. Clone the repository to your Linux environment:
   ```bash
   git clone [https://github.com/your-username/os-sandbox.git](https://github.com/your-username/os-sandbox.git)
