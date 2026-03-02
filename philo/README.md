# Philosophers – Concurrency Design Notes

This document explains the key concurrency concepts in the Philosophers simulation:
**threads**, **mutexes**, **deadlock prevention**, and **data races**.

---

## 1. Threads

| Thread | Who creates it | What it does |
|--------|---------------|--------------|
| Philosopher thread (×N) | `start_sim` via `pthread_create` | Runs `philo_routine`: eat → sleep → think loop |
| Monitor thread | Main thread itself | Runs `monitor`: checks if any philosopher died or all have eaten enough |

Every philosopher has its own `pthread_t` stored in `t_philo.thread`.
After spawning all philosopher threads the main thread becomes the monitor and
only calls `pthread_join` once the simulation is over.

---

## 2. Mutexes

Three kinds of mutexes exist in `t_table`:

### 2a. Fork mutexes – `t_table.forks[]`

One `pthread_mutex_t` per fork (N forks for N philosophers).
Locking a fork mutex means "I am holding this fork".
A philosopher must hold **both** its left and right fork to eat, then releases
them when done.

```
forks[0]  forks[1]  forks[2]  ...  forks[N-1]
   |  \      |  \      |  \            |  \
  P1   P2   P2   P3   P3   P4  ...   PN   P1
```

### 2b. `write_lock`

Protects **all calls to `printf`** in `print_status` and `died`.
Without this lock, output lines from different threads interleave and produce
garbled timestamps.

### 2c. `death_lock`

Protects every piece of **shared mutable state** that is read by the monitor
and written by philosopher threads (or vice-versa):

| Field | Writer | Reader |
|-------|--------|--------|
| `t_table.dead` | monitor (`died`, `ate`) | philosopher threads (`should_stop`, `ft_usleep`, `print_status`) |
| `t_philo.last_meal` | philosopher thread (`philo_eat`) | monitor (`died`) |
| `t_philo.meals_eaten` | philosopher thread (`philo_eat`) | monitor (`ate`), philosopher (`should_stop`) |

---

## 3. Deadlock Prevention

### The classic deadlock scenario

Without any ordering, every philosopher could simultaneously pick up their
**left** fork and then wait forever for their **right** fork – a circular wait
that never resolves.

### The fix: consistent lock ordering via pointer address (`get_forks_order`)

```c
// routine.c
static void get_forks_order(t_philo *philo,
                             pthread_mutex_t **first,
                             pthread_mutex_t **second)
{
    if (philo->left_fork < philo->right_fork)
    {
        *first  = philo->left_fork;
        *second = philo->right_fork;
    }
    else
    {
        *first  = philo->right_fork;
        *second = philo->left_fork;
    }
}
```

Every philosopher always locks the mutex with the **lower memory address
first**.  Because the fork array is a contiguous `malloc`'d block, lower index
⟺ lower address.  The last philosopher (N) therefore picks up fork N-1 before
fork 0 – the opposite order from all its neighbours.  This breaks the circular
dependency, so a global deadlock is impossible.

This is an instance of the well-known **resource ordering** (Dijkstra)
strategy: assign a total order to all resources and always acquire them in that
order.

---

## 4. Data Races

A data race occurs when two threads access the same memory location
concurrently and at least one access is a write, **without** synchronisation.

### 4a. `t_table.dead`

- Written by the monitor (in `died` / `ate`) and by philosopher threads (none –
  they only *read* it in `should_stop` and `ft_usleep`).
  Actually the monitor is the sole *writer*; philosopher threads are sole
  *readers*.  Even so, a plain read of an int is not atomically safe on all
  platforms, and the compiler/CPU may reorder or cache the value.  All accesses
  are therefore wrapped in `death_lock`.

### 4b. `t_philo.last_meal`

- Written by the philosopher's own thread inside `philo_eat`.
- Read by the monitor thread inside `died`.
- Protected by `death_lock` on both sides.

### 4c. `t_philo.meals_eaten`

- Incremented by the philosopher's own thread inside `philo_eat`.
- Read by the monitor in `ate` and by the philosopher itself in `should_stop`.
- Protected by `death_lock` on all accesses.

### 4d. `t_table.start_time`

- Written by the main thread **before** `pthread_create` is called.
- Read-only after that point.
- No mutex needed: the write happens-before all thread creations
  (sequential in `start_sim`).

### 4e. stdout (`printf`)

- Not thread-safe by default.
- Protected by `write_lock` in every call site (`print_status`, `died`).

---

## 5. The Single-Philosopher Edge Case

With only one philosopher there is only one fork.  `take_forks` locks that fork,
prints "has taken a fork", then unlocks it immediately and returns – the
philosopher can never eat and will starve after `time_to_die` ms.  The monitor
(`died`) handles this correctly; `philo_eat` and `philo_routine` both short-
circuit on `n_philos == 1`.

---

## 6. Even/Odd Staggering

```c
// routine.c – philo_routine
if (philo->id % 2 == 0)
    ft_usleep(1, philo->table);
```

Even-numbered philosophers wait 1 ms before their first fork attempt.  This
cheap stagger lets odd philosophers grab their forks first, reducing contention
at startup and avoiding near-simultaneous fork grabs that would be resolved
(harmlessly, but wastefully) by the lock-ordering alone.

---

## 7. Periodic Death-Check in `ft_usleep`

The custom sleep function polls `table->dead` every ~5 ms (50 iterations × 100 µs each):

```c
// time.c
while ((get_time() - start) < time)
{
    usleep(100);
    tmp++;
    tmp = tmp % 50;
    pthread_mutex_lock(&table->death_lock);
    if (tmp == 0 && table->dead)
    {
        pthread_mutex_unlock(&table->death_lock);
        return (1);   // early exit: simulation is over
    }
    pthread_mutex_unlock(&table->death_lock);
}
```

This avoids a philosopher sleeping through a death event for a full
`time_to_eat` or `time_to_sleep` period, keeping thread shutdown prompt.
