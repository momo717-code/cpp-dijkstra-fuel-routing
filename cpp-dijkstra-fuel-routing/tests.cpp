// PURPOSE:
// Implements the exact 9 tests described in the assignment guidance image.
// Demonstrates the learning outcome "To implement software testing".
// Each test is a single clear check(...) call as requested

#include "graph.h"
#include "vehicle.h"
#include <iostream>
#include <iomanip>
#include <cassert>
#include <cmath>

void runTests() {
    std::cout << "\n=== RUNNING 9 REQUIRED TESTS ===\n" << std::endl;

     Graph g;
    if (!g.loadData("graph.txt")) {
        std::cerr << "Cannot load graph.txt for tests.\n";
        return;
    }

    std::vector<Vehicle> lorries = {
        Vehicle("Small Lorry", 80.0, 12.0),
        Vehicle("Medium Lorry", 120.0, 9.0),
        Vehicle("Large Lorry", 180.0, 7.0)
    };

    int passed = 0;
    int total = 9;

    auto check = [&](const std::string& testName, bool condition) {
        if (condition) {
            std::cout << "[PASS] " << testName << std::endl;
            passed++;
        } else {
            std::cout << "[FAIL] " << testName << std::endl;
        }
    };

    // Test 1
    check("Graph loads exactly 10 cities", g.getCities().size() == 10);

    // Test 2
    int wickIdx = g.findCityIndex("Wick");
    check("Wick's fuel price is 2.90", std::abs(g.getCities()[wickIdx].fuelPrice - 2.90) < 0.001);

    // Test 3
    check("Dundee is in the lookup table", g.hasCity("Dundee"));

    // Test 4
    check("Atlantis is NOT in the lookup table", !g.hasCity("Atlantis"));

    // Test 5
    int dundeeIdx  = g.findCityIndex("Dundee");
    int glasgowIdx = g.findCityIndex("Glasgow");
    auto path = g.getShortestPath(dundeeIdx, glasgowIdx);
    check("Dundee to Glasgow shortest path has 3 cities", path.size() == 3);

    // Test 6
    double totalDist = 0.0;
    for (size_t i = 0; i + 1 < path.size(); ++i) {
        for (const auto& edge : g.getAdj()[path[i]]) {
            if (edge.first == path[i+1]) {
                totalDist += edge.second;
                break;
            }
        }
    }
    check("Dundee->Glasgow total distance is 84 miles", std::abs(totalDist - 84.0) < 0.001);

    // Test 7 & 8 & 9 - Small Lorry Dundee to Glasgow
    auto [cost, dist, finalFuel, refuels] = 
        lorries[0].simulateJourney(g, path, 0.0);

    check("Small Lorry Dundee->Glasgow cost is ~148.00", std::abs(cost - 148.0) < 1.0);
    check("Fuel remaining after journey is ~73.0 litres", std::abs(finalFuel - 73.0) < 1.0);
    check("Exactly one refuel stop at Dundee", refuels.size() == 1 && refuels[0].city == "Dundee");

    std::cout << "\n=== TEST SUMMARY: " << passed << "/" << total << " tests passed ===\n" << std::endl;
}

int main() {
    runTests();
    return 0;
}