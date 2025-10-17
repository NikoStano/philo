# Philo

---
# Dining Philosophers Simulation
**This project simulates the classic Dining Philosophers problem using multithreading in C. It demonstrates the use of mutexes to manage concurrent access to shared resources (forks) and ensures that philosophers can eat without causing deadlocks or starvation.**

---

## Features
- Simulates multiple philosophers who alternate between thinking and eating.
- Uses mutexes to control access to forks.
- Configurable parameters for the number of philosophers, time to die, time to eat, time to sleep, and optional number of meals.
- Graceful handling of philosopher death and termination of the simulation.

---

## Compilation
To compile the project, use the provided Makefile. Simply run:
```make
```
This will generate an executable named `philo`.

---

## Usage
Run the program with the following command:
```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_meals]
```
- `number_of_philosophers`: The total number of philosophers (and forks).
- `time_to_die`: Time (in milliseconds) a philosopher can go without eating before dying.
- `time_to_eat`: Time (in milliseconds) a philosopher takes to eat.
- `time_to_sleep`: Time (in milliseconds) a philosopher spends sleeping.
- `number_of_meals` (optional): Number of times each philosopher must eat before the simulation ends.

---

## Example
```bash
./philo 5 800 200 200 7
```
This command starts a simulation with 5 philosophers, where each philosopher dies if they don't eat within 800ms, takes 200ms to eat, sleeps for 200ms, and must eat 7 times.

---

## Statistics project :

|  Directory  |  Lines of Code  | Comments | Blank Lines |
|-------------|-----------------|----------|-------------|
| srcs/       |             345 |      102 |          45 |
| include/    |              50 |       20 |          10 |
| **Total**   |         **395** |   **122**|      **55** |
