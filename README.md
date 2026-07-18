# Operating Systems I

**CCPS 590 -- Toronto Metropolitan University**

A hands-on collection of C programs demonstrating fundamental operating system concepts: process creation, signal handling, inter-process communication, threading, and synchronization primitives.

## Topics Covered

### Process Management
| File | Description |
|------|-------------|
| `p1.c` | Character printing with timed intervals |
| `pids1.c` / `pids2.c` | `fork()` demonstration -- parent/child PID relationships |
| `clone.c` | Heavyweight vs lightweight process creation with `clone()` |
| `arr_add.c` | Parallel array addition using 4 clone'd threads |

### Signal Handling
| File | Description |
|------|-------------|
| `p2.c` | Signal handlers for SIGINT/SIGTERM |
| `p3.c` | Sending SIGINT to a target PID |
| `sigcatcher.c` | Catching SIGINT, SIGTERM, SIGCONT, SIGSTOP |
| `sigsender.c` | Sending arbitrary signals to processes |

### Threading & Synchronization
| File | Description |
|------|-------------|
| `thread_race.c` | Race condition demonstration with unprotected shared variable |
| `thread_sem.c` | Race condition resolved using POSIX semaphores |
| `prodcon.c` | Producer-consumer pattern with empty/full/access semaphores |

### Inter-Process Communication
| File | Description |
|------|-------------|
| `kirk.c` / `spock.c` | Message queue IPC (sender/receiver) |
| `writeP.c` / `readP.c` | Shared memory IPC via System V (`shmget`/`shmat`) |

## Technologies

- **C** (compiled with `gcc`)
- **POSIX APIs** -- `fork()`, `clone()`, `pthread`, `signal()`, `kill()`
- **System V IPC** -- shared memory (`shmget`, `shmat`, `shmctl`), message queues (`msgget`, `msgsnd`, `msgrcv`)
- **POSIX Semaphores** -- `sem_init`, `sem_wait`, `sem_post`

## Building

```bash
# Compile individual programs
gcc -o pids1 pids1.c
gcc -pthread -o thread_race thread_race.c
gcc -pthread -D_GNU_SOURCE -o clone clone.c
gcc -o kirk kirk.c
gcc -o writeP writeP.c
```

## Project Structure

```
Operating-Systems-I/
  p*.c               # Process and signal programs
  thread_*.c         # Threading demos
  prodcon.c          # Producer-consumer
  kirk.c / spock.c   # Message queue IPC
  writeP.c / readP.c # Shared memory IPC
  clone.c / arr_add.c # clone() demos
```
