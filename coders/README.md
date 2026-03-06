*This project has been created as part of the 42 curriculum by nbilyj.*

# Codexion

## Description

**Codexion** is a multithreaded simulation program that models the classic **dining philosophers problem**, adapted to a modern coding environment. In this scenario, multiple coders share limited USB dongles required to compile quantum code. The project focuses on **concurrency**, **synchronization**, and **resource scheduling** using POSIX threads.

### Goal

The main objectives are to:
- Implement a **deadlock-free** and **starvation-free** resource allocation system
- Support two scheduling algorithms: **FIFO** (First In, First Out) and **EDF** (Earliest Deadline First)
- Detect and handle **burnout** (starvation) when a coder cannot compile within a specified timeout
- Ensure **thread-safe logging** and **precise timing** for all events

### Overview

Coders sit in a circular arrangement, with one USB dongle between each pair. To compile, a coder must acquire **two dongles** (left and right). After compiling, they debug, refactor, and repeat the cycle until either:
- All coders have compiled the required number of times (**success**)
- A coder burns out due to starvation (**failure**)

---

## Instructions

### Compilation

Compile the project using the provided Makefile:
```bash
make
```

This generates the `codexion` executable.

### Execution

Run the program with the following arguments:
```bash
./codexion <nb_coders> <time_to_burnout> <time_to_compile> <time_to_debug> \
           <time_to_refactor> <nb_compiles_required> <dongle_cooldown> <scheduler>
```

**Arguments:**
- `nb_coders`: Number of coders (and dongles)
- `time_to_burnout`: Maximum time (ms) a coder can wait before burning out
- `time_to_compile`: Time (ms) spent compiling
- `time_to_debug`: Time (ms) spent debugging
- `time_to_refactor`: Time (ms) spent refactoring
- `nb_compiles_required`: Minimum compiles each coder must complete
- `dongle_cooldown`: Cooldown period (ms) after a dongle is released
- `scheduler`: `fifo` or `edf`

**Example:**
```bash
./codexion 5 2000 200 100 100 3 50 fifo
```

This runs a simulation with 5 coders, a 2-second burnout timeout, and FIFO scheduling.

### Expected Output

The program logs each state change:
```
0 1 has taken a dongle
0 1 has taken a dongle
0 1 is compiling
202 1 is debugging
404 1 is refactoring
...
```

The simulation ends when:
- All coders complete the required compiles (success)
- A coder burns out (failure, logged within 10ms of actual burnout)

---

## Resources

### Documentation and References

- [POSIX Threads Programming](https://computing.llnl.gov/tutorials/pthreads/) - Lawrence Livermore National Laboratory
- [The Little Book of Semaphores](http://greenteapress.com/semaphores/) - Allen B. Downey
- [Dining Philosophers Problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem) - Wikipedia
- [Priority Queue (Binary Heap)](https://en.wikipedia.org/wiki/Binary_heap) - Wikipedia
- `man pthread_create`, `man pthread_mutex_lock`, `man pthread_cond_wait`

### AI Usage

AI tools (Claude, ChatGPT) were used for:
- **Understanding concurrency concepts**: Explaining deadlock conditions, priority queues, and EDF scheduling
- **Debugging assistance**: Identifying race conditions and synchronization bugs
- **Code review**: Checking norminette compliance and suggesting refactoring for cleaner code
- **Algorithm explanations**: Understanding binary heap operations (bubble up, bubble down) for priority queue implementation

**All code logic, architecture decisions, and final implementation were done manually.** AI was used as a learning assistant, not a code generator.

---

## Blocking Cases Handled

### 1. Deadlock Prevention

**Problem:** Circular wait can occur if each coder holds one dongle and waits for another.

**Solution:** **Ordered resource acquisition** — coders always acquire dongles in a consistent order (by memory address). This breaks the circular wait condition (Coffman's 4th condition).

**Implementation:**
```c
if (coder->left_dongle < coder->right_dongle)
{
    first = coder->left_dongle;
    second = coder->right_dongle;
}
else
{
    first = coder->right_dongle;
    second = coder->left_dongle;
}
```

### 2. Starvation Prevention

**Problem:** Some coders may never acquire dongles under unfair scheduling.

**Solution:** **Priority queue** with fair arbitration:
- **FIFO:** First-come, first-served ensures fairness
- **EDF:** Coders closest to burnout are prioritized

**Implementation:** Each dongle has a min-heap priority queue. When multiple coders request a dongle, the one with the highest priority is served first.

### 3. Cooldown Handling

**Problem:** Dongles must remain unavailable for `dongle_cooldown` milliseconds after release.

**Solution:** Each dongle stores a `cooldown_end` timestamp. Before granting a dongle, the thread checks if `current_time >= cooldown_end`. If not, it releases the mutex, sleeps, and rechecks.

### 4. Precise Burnout Detection

**Problem:** Burnout must be logged within 10ms of the actual deadline.

**Solution:** A dedicated **monitor thread** checks all coders every 10ms:
```c
while (is_running(sim))
{
    usleep(10000);  // 10ms interval
    now = get_time_ms();
    // Check each coder's deadline
    if (now > deadline)
        log_burnout();
}
```

### 5. Log Serialization

**Problem:** Concurrent printf calls from multiple threads can interleave output.

**Solution:** A dedicated **log mutex** protects all logging operations:
```c
pthread_mutex_lock(&sim->log_mutex);
printf("%ld %d %s\n", timestamp, coder_id, msg);
pthread_mutex_unlock(&sim->log_mutex);
```

---

## Thread Synchronization Mechanisms

### 1. `pthread_mutex_t` (Mutual Exclusion)

**Used for:**
- Protecting each dongle's state (`mutex` per dongle)
- Protecting global simulation state (`sim->mutex`)
- Serializing log output (`sim->log_mutex`)

**Example (dongle protection):**
```c
pthread_mutex_lock(&dongle->mutex);
// Critical section: check cooldown, modify queue, take dongle
dongle->is_taken = 1;
pthread_mutex_unlock(&dongle->mutex);
```

### 2. `pthread_cond_t` (Condition Variables)

**Used for:**
- Efficient waiting when a dongle is unavailable
- Waking threads when dongle state changes

**How it works:**
```c
// Thread waiting for dongle
while (!(queue_top == me && !dongle->is_taken))
{
    pthread_cond_wait(&dongle->cond, &dongle->mutex);  // Atomically unlocks mutex and sleeps
}
// Thread wakes up with mutex re-locked

// Thread releasing dongle
dongle->is_taken = 0;
pthread_cond_broadcast(&dongle->cond);  // Wake all waiting threads
```

**Why broadcast instead of signal?**
- Multiple threads may be waiting
- We don't know which one is now the highest priority
- All threads wake up, re-check their condition, and the priority one proceeds while others sleep again

### 3. Priority Queue (Custom Implementation)

**Structure:**
- Binary min-heap stored in an array
- Each node contains: `{coder*, priority}`
- Priority = timestamp (FIFO) or deadline (EDF)

**Thread-safe access:**
- All queue operations (`push`, `pop`) are performed **inside the dongle's mutex lock**
- Ensures atomic insertion/removal and heap property maintenance

**Example (FIFO):**
```c
pthread_mutex_lock(&dongle->mutex);
priority = get_time_ms();  // Lower timestamp = higher priority
queue_push(dongle->queue, coder, priority);
// Wait until coder is at the top
while (queue->nodes[0].coder != coder || dongle->is_taken)
    pthread_cond_wait(&dongle->cond, &dongle->mutex);
queue_pop(dongle->queue);
pthread_mutex_unlock(&dongle->mutex);
```

### 4. Race Condition Prevention

**Shared variables protected by mutex:**
- `coder->last_compile_start` and `coder->compile_count` (protected by `sim->mutex`)
- `dongle->is_taken`, `dongle->cooldown_end`, `dongle->queue` (protected by `dongle->mutex`)
- `sim->running` (protected by `sim->mutex`)

**Example (updating coder state):**
```c
pthread_mutex_lock(&coder->sim->mutex);
coder->last_compile_start = get_time_ms();
coder->compile_count++;
pthread_mutex_unlock(&coder->sim->mutex);
```

### 5. Thread-Safe Communication (Coder ↔ Monitor)

**Challenge:** Monitor must read coder state without causing data races.

**Solution:** Monitor reads shared variables under mutex protection:
```c
pthread_mutex_lock(&sim->mutex);
last_compile = sim->coders[i].last_compile_start;
compile_count = sim->coders[i].compile_count;
pthread_mutex_unlock(&sim->mutex);
// Now safe to use these values outside the lock
```

**Coder threads update the same variables atomically:**
```c
pthread_mutex_lock(&coder->sim->mutex);
coder->compile_count++;
pthread_mutex_unlock(&coder->sim->mutex);
```

This ensures the monitor always reads consistent state (no partial updates).

---

## Technical Highlights

- **Deadlock-free** by design (ordered resource acquisition)
- **Starvation-free** with fair scheduling (FIFO/EDF)
- **O(log n)** priority queue operations (binary heap)
- **Thread-safe** logging and state management
- **Precise timing** with `gettimeofday()` and 10ms monitor polling
- **Norminette compliant** (42 coding standards)

---

## License

This project is part of the 42 school curriculum and follows the school's academic policies.