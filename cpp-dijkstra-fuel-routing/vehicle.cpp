#include "vehicle.h"
#include <iostream>
#include <iomanip>

std::tuple<double, double, double, std::vector<RefuelLog>>
Vehicle::simulateJourney(const Graph& g,
                        const std::vector<int>& path,
                        double initialFuel) const {

    if (path.empty()) return {0.0, 0.0, 0.0, {}};

    double currentFuel = initialFuel;
    double totalCost = 0.0;
    double totalDistance = 0.0;
    std::vector<RefuelLog> refuels;

    int currentCityIdx = path[0];

    // === ASSIGNMENT REQUIREMENT ===
    // Vehicle always starts with an empty tank at the very first city.
    // We therefore force a full fill-up before any travel begins.
    if (currentFuel == 0.0) {
        double buy = tankCapacity;
        totalCost += buy * g.getCities()[currentCityIdx].fuelPrice;
        currentFuel = tankCapacity;
        refuels.push_back({g.getCities()[currentCityIdx].name, buy, buy * g.getCities()[currentCityIdx].fuelPrice});
    }

    // Find the exact edge distance (we know it exists because path came from Dijkstra)
    for (size_t i = 0; i + 1 < path.size(); ++i) {
        int fromIdx = path[i];
        int toIdx = path[i + 1];

        // Find the edge distance
        double edgeDist = 0.0;
        bool found = false;
        for (const auto& p : g.getAdj()[fromIdx]) {
            if (p.first == toIdx) {
                edgeDist = p.second;
                found = true;
                break;
            }
        }
        if (!found) {
            std::cerr << "Error: No direct road between adjacent cities !" << std::endl;
            return {0.0, 0.0, 0.0, {}};
        }

        double litresRequired = edgeDist / milesPerLitre;
        totalDistance += edgeDist;

        // Refuel to full tank if we cannot reach the next city
        if (currentFuel < litresRequired) {
            double buy = tankCapacity - currentFuel;
            totalCost += buy * g.getCities()[fromIdx].fuelPrice;
            currentFuel = tankCapacity;
            refuels.push_back({g.getCities()[fromIdx].name, buy, buy * g.getCities()[fromIdx].fuelPrice});
        }

        // Consume fuel for this leg of the journey
        currentFuel -= litresRequired;
        if (currentFuel < 0.0) currentFuel = 0.0;   // safety
    }

    return {totalCost, totalDistance, currentFuel, refuels};
}
