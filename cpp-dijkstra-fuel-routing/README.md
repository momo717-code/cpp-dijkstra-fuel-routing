# 🚚 Distance / Fuel-Cost Route Planner (C++)

A C++ route-planning system for a Scottish goods-transport company. It models the road network as a **weighted directed graph**, finds the shortest route with **Dijkstra's algorithm**, and simulates a lorry's journey — fuel use, refuelling stops, and total cost — choosing where to refuel based on each city's fuel price.

![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![Tests](https://img.shields.io/badge/tests-9%20passing-brightgreen?style=flat-square)

> MSc *Advanced Programming Techniques* project (AC52002), University of Dundee.

---

## Overview

At startup the program loads **10 cities and 15 roads** from `graph.txt`. The user enters a start and destination and either picks one of three lorries or asks the program to **compare all three**. The output lists the route, every refuelling stop (litres + cost), the total distance, and the total fuel cost.

## Features

- **Dijkstra shortest-path** routing over an **adjacency-list** graph (chosen over a matrix because the road network is sparse).
- **Fuel simulation** per vehicle — three lorries with different tank sizes and efficiency (miles-per-litre).
- **Cost-aware refuelling** using each city's fuel price, with a full refuelling log.
- **Two optional extras:** continue an onward journey using remaining fuel, and **auto-recommend the cheapest lorry**.
- **Clean OOP** — `Graph` and `Vehicle` classes with private data, const accessors, and documented design rationale.
- **9-test suite** (`tests.cpp`) covering routing, fuel logic, and edge cases.

## Design

| File | Responsibility |
|---|---|
| `graph.h` / `graph.cpp` | `Graph` class — data loading, adjacency list, Dijkstra, city lookups |
| `vehicle.h` / `vehicle.cpp` | `Vehicle` class — tank/efficiency, `simulateJourney()` fuel + cost logic |
| `main.cpp` | Interactive CLI with robust input validation; wires Graph + Vehicle together |
| `tests.cpp` | `runTests()` — the 9 required unit tests |
| `graph.txt` | Network data: cities + fuel prices, then road distances |

## Build & run

```bash
git clone https://github.com/momo717-code/cpp-dijkstra-fuel-routing.git
cd cpp-dijkstra-fuel-routing

# Build the app
g++ -std=c++17 graph.cpp vehicle.cpp main.cpp -o route_planner
./route_planner          # keep graph.txt in the working directory
```

The 9-test suite lives in `tests.cpp` (`runTests()`); call it from a small driver/`main` to execute the checks. Full write-up in **`Route_Planner_Report.pdf`**.

## What I learned

- Implementing **Dijkstra's algorithm** and choosing the right graph representation for a real, sparse network.
- **Encapsulation and const-correctness** in C++ class design.
- Writing a **focused unit-test suite** and handling messy user input defensively.
