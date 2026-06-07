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

This exercise explores the fundamental paradigms of multitasking (*višezadaćni rad*) within Unix-like systems, deeply contrasting the execution, memory footprints, and synchronization requirements of multi-threading versus multi-processing.

### 👥 Lab 2a: Multi-Threading & Strict Alternation (`lab2_dretve`)
This sub-exercise implements lightweight concurrency within a single process address space using the POSIX Threads (`pthread`) library to demonstrate predictable, coordinated execution between threads.

* **Memory Architecture:** Demonstrates how threads inherently share global data, heap segments, and open file descriptors, while maintaining their own isolated stack segments (`STACK`).
* **Strict Alternation:** Employs a shared state variable (`red_na_potezu`) to synchronize a producer thread (generating random data into a shared array) and a consumer thread (calculating the sum).
* **Synchronization & Reentrancy:** Implements synchronization via **busy waiting (spin-locking)**. Incorporates the concept of thread-safety (*multi-thread-safe*) and explains why variables accessed across threads should be declared using atomic types (`atomic` / `volatile`) to prevent compiler reordering optimizations.

**Compilation & Execution:**
```bash
gcc -pthread lab2_dretve.c -o lab2_dretve
./lab2_dretve <N_elements> <M_iterations>
```

### 👥 Lab 2b: Multi-Processing & Shared Memory (`lab2_dretve`)
This sub-exercise implements true parallel multi-processing across completely isolated execution environments.

* **Process Isolation:** Spawns independent child processes using the (`fork()`) system call, resulting in distinct address spaces separated by a Copy-on-Write (COW) mechanism.
* **Shared Memory Segment:** Allocates and maps a System V shared memory block (zajednički adresni prostor) using kernel system calls (`shmget, shmat, shmdt, shmctl`) to bypass process isolation.
* **Mutual Exclusion:** Implements Lamport's Bakery Algorithm entirely at the software level to protect the Critical Section (C.S.) without relying on hardware-level atomic locks.

**Compilation & Execution:**
```bash
gcc lab2_procesi.c -o lab2_procesi
./lab2_procesi <number_of_processes>
```

> 💡 **Interview Takeaway:** Gained a deep architectural understanding of the trade-offs between lightweight threads (zero memory isolation overhead, high risk of race conditions) and heavyweight processes (hardware-isolated address spaces requiring kernel-level IPC structures). Successfully managed persistent kernel memory allocations and prevented data corruption by implementing low-level software synchronization primitives.

---

### 🔒 Lab 3: Thread Synchronization
🔒 Lab 3: Thread & Process Synchronization

This exercise deeply explores classic synchronization problems in concurrent programming. It showcases how to prevent data corruption, race conditions, and deadlocks using semaphores across two different paradigms: multi-threading (POSIX) and multi-processing (System V IPC).

👥 Lab 3a: Multi-Threaded Producer-Consumer (lab3_dretve)

This sub-exercise implements a variant of the classic Producer-Consumer problem using POSIX threads within a single process address space, utilizing POSIX semaphores for thread coordination.

* **Architecture & Flow:** A single producer thread dynamically generates random tasks (numbers) and places them into a shared variable (`globalni_broj`), while multiple worker threads ($m$ instances) compete to safely fetch and process (calculate the sum of numbers) the tasks.
* **Synchronization Primitives:** * `sem_prazan` & `sem_pun`: Used as signaling mechanisms to coordinate the production and consumption phases, ensuring the producer doesn't overwrite data before it's read, and workers don't read stale data.
  * `sem_mutex`: Acts as a binary semaphore (mutex) protecting the Critical Section (C.S.) when worker threads check the global termination flag (`kraj`) and copy the shared task locally.
* **Compilation & Execution:**
  ```bash
  gcc -pthread lab3_dretve.c -o lab3_dretve
  ./lab3_dretve <number_of_workers> <number_of_tasks>

  
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
