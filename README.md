*This project has been created as part of the 42 curriculum by hgawhari.*

---

## Table of Contents

- [Description](#description)
- [Instructions](#instructions)
- [Command-line arguments](#command-line-arguments)
- [Concurrency and synchronization](#concurrency-and-synchronization)
- [Deadlock prevention](#deadlock-prevention)
- [Testing](#testing)
- [Resources](#resources)

---

## 🔍 Description

**Codexion** is a concurrency simulation inspired by the classic
**Dining Philosophers problem**.

Instead of philosophers competing for forks, Codexion models **coders competing
for USB dongles**.

Each coder:

1. Requests two dongles.
2. Waits until the required dongles can be acquired.
3. Compiles.
4. Releases the dongles.
5. Debugs.
6. Refactors.
7. Repeats until the required number of compilations is reached.

The simulation demonstrates how multiple concurrent threads can safely share
limited resources while handling:

- Resource contention
- Scheduling policies
- Condition variables
- Mutex synchronization
- Deadlock prevention
- Resource cooldown periods
- Burnout deadlines
- Clean thread shutdown
- Serialized logging


## 🚀 Instructions

Clone the repository and enter the project directory.

### Build

```bash
make
```

### Clean object files

```bash
make clean
```

### Remove all generated files

```bash
make fclean
```

### Rebuild from scratch

```bash
make re
```

### Run

```bash
./codexion <number_of_coders> <time_to_burnout> \
<time_to_compile> <time_to_debug> <time_to_refactor> \
<number_of_compiles_required> <dongle_cooldown> <scheduler>
```
| Argument                      | Description                                        |
| ----------------------------- | -------------------------------------------------- |
| `number_of_coders`            | Number of coder threads                            |
| `time_to_burnout`             | Maximum time (ms) before a coder burns out         |
| `time_to_compile`             | Compilation duration (ms)                          |
| `time_to_debug`               | Debugging duration (ms)                            |
| `time_to_refactor`            | Refactoring duration (ms)                          |
| `number_of_compiles_required` | Number of compilations required per coder          |
| `dongle_cooldown`             | Time (ms) a dongle stays unavailable after release |
| `scheduler`                   | Scheduling policy: `fifo`, `lifo`, or `edf`        |


# 📊 Schedulers

Each dongle maintains its own waiting queue.

Every request contains:

```c
typedef struct s_task
{
    unsigned int    coder_id;
    long            deadline;
    unsigned long   arrival_order;
} t_task;
```

The scheduler comparator determines which waiting request reaches the
front of the queue.

---

## FIFO — First In, First Out

FIFO gives priority to the request that arrived first.

Conceptually:

```text
Arrival:

Coder 1 → Coder 2 → Coder 3

Grant:

Coder 1 → Coder 2 → Coder 3
```

The implementation compares `arrival_order`.


## EDF — Earliest Deadline First

EDF gives priority to the request with the earliest deadline.

Each request stores:

```c
long deadline;
```

The scheduler compares the deadlines to determine priority.

When deadlines are equal, the scheduler uses the project's defined
tie-breaking rule.

# 🔧 Concurrency and synchronization

Codexion uses POSIX synchronization primitives to coordinate access to shared
resources.

## `pthread_mutex_t`

Mutexes protect shared state.

### Coder mutex

Protects coder-specific state such as:

```text
last_compile_ms
compiles_done
```

### Dongle mutex

Protects:

```text
available
last_release_ms
wait_queue
```

### Log mutex

Ensures that log messages from different threads do not interleave.

### Simulation mutex

Protects the global `running` state.

### Counter mutex

Protects the global request counter used to assign
`arrival_order`.

---

## `pthread_cond_t`

Each dongle has a condition variable:

```c
pthread_cond_t cond;
```

A coder that cannot currently acquire a dongle waits on the condition variable
instead of continuously polling.

When a dongle is released:

```c
pthread_cond_broadcast(&dongle->cond);
```

waiting coders are awakened and re-evaluate the scheduler.

This avoids unnecessary busy-waiting.

---

# 🛡️ Deadlock prevention

A classic deadlock requires the four Coffman conditions:

| Condition        | Meaning                                               |
| ---------------- | ----------------------------------------------------- |
| Mutual exclusion | A resource can only be held by one thread             |
| Hold and wait    | A thread holds one resource while waiting for another |
| No preemption    | A resource cannot be forcibly taken                   |
| Circular wait    | Threads form a circular dependency                    |

Codexion prevents **circular wait** by imposing a global ordering on dongle
acquisition.

Each coder determines its two dongles and acquires them in ascending dongle
index order.

For example:

```text
Coder 1:
    dongle 0 → dongle 1

Coder 2:
    dongle 1 → dongle 2

Coder 3:
    dongle 2 → dongle 3
```

This prevents a circular dependency such as:

```text
Coder A → waiting for B
Coder B → waiting for A
```

because all coders follow the same global ordering.

---

# ⏱️ Dongle cooldown

After a dongle is released, it records:

```c
last_release_ms
```

The next request cannot use the dongle until:

```text
current_time - last_release_ms >= dongle_cooldown
```

This is handled through the dongle's condition variable and avoids unnecessary
busy-waiting.

Example:

```bash
./codexion 5 3000 200 200 200 5 800 fifo
```

Here, every released dongle must remain unavailable for `800 ms`.

---

# 💀 Burnout detection

A dedicated monitor thread watches coder deadlines.

Each coder records the timestamp of its latest compilation:

```c
long last_compile_ms;
```

If the time since the coder's last successful compilation exceeds
`time_to_burnout`, the coder burns out and the simulation is stopped.

Example:

```text
3000 1 burned out
```

The monitor coordinates shutdown so that waiting coder threads are awakened
and can terminate cleanly.

---

# 🧵 Thread lifecycle

The simulation consists of:

```text
Main thread
    │
    ├── Coder thread 1
    ├── Coder thread 2
    ├── Coder thread 3
    ├── ...
    └── Monitor thread
```

A coder repeatedly performs:

```text
        ┌───────────────┐
        │ Acquire 2     │
        │ dongles        │
        └───────┬───────┘
                │
                ▼
           Compiling
                │
                ▼
          Release dongles
                │
                ▼
            Debugging
                │
                ▼
           Refactoring
                │
                ▼
        More compilations?
           /          \
         yes           no
          │             │
          └──────┐      ▼
                 │     exit
                 ▼
             Acquire
```

---

# 🧪 Testing

Useful manual tests include:

### Basic FIFO

```bash
./codexion 5 2000 200 200 200 2 0 fifo
```

### EDF contention

```bash
./codexion 5 3000 200 200 200 5 200 edf
```

# 📚 Resources

* [Dining Philosophers Problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
* [POSIX Threads](https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html)
* `pthread_mutex_lock`
* `pthread_cond_wait`
* `pthread_cond_broadcast`
* POSIX thread synchronization documentation
