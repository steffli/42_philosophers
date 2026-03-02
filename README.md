# Philosophers

*This project has been created as part of the 42 curriculum by stliu.*

---

## Description

**Philosophers** is a classic concurrency problem — a C implementation of Dijkstra's *Dining Philosophers* problem.

**The scenario:** A number of philosophers sit around a circular table. A large bowl of spaghetti sits in the center. Each philosopher alternates between three states: **thinking**, **eating**, and **sleeping**. Between every two adjacent philosophers lies a single fork. To eat, a philosopher must pick up *both* the fork to their left and the fork to their right simultaneously. After eating, they put both forks down and sleep, then think again. If a philosopher goes too long without eating, they die of starvation.

**The goal:** Simulate this scenario using POSIX threads (`pthread`) and mutexes, ensuring:
- No philosopher starves (given feasible timing parameters).
- No **deadlock** occurs (circular wait is prevented via consistent fork-acquisition ordering).
- No **data race** occurs on shared state (protected by `death_lock` and `write_lock` mutexes).
- Output is clean and correctly timestamped.

### Key design choices

| Problem | Solution |
|---|---|
| Deadlock from circular fork acquisition | Philosophers always lock the **lower-addressed** fork first (resource hierarchy) |
| Data race on `last_meal`, `meals_eaten`, `dead` | All reads/writes guarded by `death_lock` |
| Interleaved output from concurrent `printf` | All prints serialised through `write_lock` |
| `usleep` inaccuracy | Busy-wait loop in `ft_usleep` polling every 100 µs |
| Initial contention | Even-numbered philosophers delayed 1 ms at startup |

---

## Instructions

### Requirements

- Linux or macOS (POSIX threads)
- `make`

### Compilation

```bash
cd philo
make
```

This produces the `philo` executable. To clean up:

```bash
make clean   # remove object files
make fclean  # remove object files and binary
make re      # fclean + compile
```

### Usage

```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

| Argument | Description |
|---|---|
| `number_of_philosophers` | Number of philosophers (and forks) at the table |
| `time_to_die` (ms) | Time after last meal before a philosopher dies |
| `time_to_eat` (ms) | Time it takes to eat (holds both forks during this) |
| `time_to_sleep` (ms) | Time spent sleeping after eating |
| `number_of_times_each_philosopher_must_eat` | *(Optional)* Simulation stops once all philosophers have eaten at least this many times |

### Examples

```bash
# 1 philosopher — will always die (only one fork)
./philo 1 800 200 200
```

### Expected output format

```
<timestamp_in_ms> <philosopher_id> <state>
```

States: `has taken a fork` | `is eating` | `is sleeping` | `is thinking` | `died`

---

## Resources

### Concurrency & POSIX threads

- [POSIX Threads Programming — Lawrence Livermore National Laboratory](https://hpc-tutorials.llnl.gov/posix/) — comprehensive `pthread` reference
- [`pthread_mutex_lock(3)` man page](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
- [`gettimeofday(2)` man page](https://man7.org/linux/man-pages/man2/gettimeofday.2.html)

### Dining Philosophers

- [Dijkstra's original paper (EWD310)](https://www.cs.utexas.edu/users/EWD/transcriptions/EWD03xx/EWD310.html) — the origin of the problem
- [Wikipedia — Dining Philosophers Problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem) — overview of solutions and deadlock conditions

### Data races & memory model

- [Coffman conditions for deadlock — Wikipedia](https://en.wikipedia.org/wiki/Deadlock#Necessary_conditions)

### AI usage

GitHub Copilot (Claude Sonnet) was used during this project for the following tasks:

- **Code explanation** — analysing the mutex strategy, deadlock prevention logic, and identifying potential TOCTOU windows in `print_status`.
- **Debugging assistance** — reasoning through edge cases such as the single-philosopher scenario and even/odd startup staggering.
- **README generation** — drafting and structuring this document.

AI was not used to write core simulation logic, thread management, or the mutex architecture; those were implemented and reasoned through independently.
