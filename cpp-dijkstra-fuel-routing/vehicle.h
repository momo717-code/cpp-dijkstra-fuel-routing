// PURPOSE:
// Defines the Vehicle class that encapsulates lorry attributes and the
// complete fuel/refuelling simulation logic.

// DESIGN RATIONALE:
// - Encapsulating the simulation inside the Vehicle class follows good OOP
//   principles (learning outcome: "To consider the design of classes").
// - Each vehicle knows its own tank size and efficiency → clean separation
//   of concerns from the Graph class.
// - simulateJourney() is a const member function because it does not modify
//   the vehicle itself; the fuel level is passed in as a parameter.
// - Data members are private; accessed via const getters (encapsulation).

#ifndef VEHICLE_H
#define VEHICLE_H

#include "graph.h"
#include <vector>
#include <string>
#include <tuple>

class Vehicle {
public:
    Vehicle(const std::string& n, double tank, double mpl)
        : name(n), tankCapacity(tank), milesPerLitre(mpl) {}

    // Read-only accessors
    const std::string& getName()         const { return name; }
    double             getTankCapacity() const { return tankCapacity; }
    double             getMpl()          const { return milesPerLitre; }

    // Simulates the entire journey along a given shortest path.
    // Returns (totalCost, totalDistance, finalFuel, refuelLogs).
    std::tuple<double, double, double, std::vector<RefuelLog>>
    simulateJourney(const Graph& graph,
                    const std::vector<int>& path,
                    double initialFuel = 0.0) const;

private:
    std::string name;
    double tankCapacity;       // litres
    double milesPerLitre;      // efficiency (mpl)
};

#endif