// This is the main driver program. It demonstrates:
//   - User interface with robust input handling
//   - Both optional extras (continue journey + all-lorries recommendation)
//   - Integration of Graph and Vehicle classes

#include "graph.h"
#include "vehicle.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <tuple>
#include <cctype>

int main() {
    Graph graph;
    if (!graph.loadData("graph.txt")) {
        std::cerr << "Cannot start: graph.txt missing or malformed.\n";
        return 1;
    }
    // Three lorries as specified in the assignment brief
    std::vector<Vehicle> lorries = {
        Vehicle("Small Lorry", 80.0, 12.0),
        Vehicle("Medium Lorry", 120.0, 9.0),
        Vehicle("Large Lorry", 180.0, 8.0)
    };
    
    graph.printAvailableCities();

    std::cout << "\nAvailable lorries:" << std::endl;
    for (size_t i = 0; i < lorries.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << lorries[i].getName()
                  << " (Tank: " << lorries[i].getTankCapacity() << "L, "
                  << lorries[i].getMpl() << " miles per litre)" << std::endl;
    } 

    double currentFuelLevel = 0.0;
    int currentCityIdx = -1;
    bool firstJourney = true;

    while (true) {
        std::string startStr, destStr;

        if (firstJourney) {
            std::cout << "\nPlease enter starting city point: ";
            std::cin >> startStr;
            int idx = graph.findCityIndex(startStr);
            if (idx < 0) {
                std::cout << "Invalid city name. Please try again." << std::endl;
                continue;
            }
            currentCityIdx = idx;            
        } else {
            std::cout << "\nCurrent location: " << graph.getCities()[currentCityIdx].name
                      << " (fuel remaining: " << std::fixed << std::setprecision(2)
                      << currentFuelLevel << "L)" << std::endl;
        }

        std::cout << " Please enter the destination city: ";
        std::cin >> destStr;
        int destIdx = graph.findCityIndex(destStr);
        if (destIdx < 0 || destIdx == currentCityIdx) {
            std::cout << "Invalid destination name. Please try again." << std::endl;
            continue;
        }

        std::cout << "\n1. Select one lorry\n2. Calculate for All lorries and get recommendation" << std::endl;
        int mode;
        std::cin >> mode;

        std::vector<int> bestPath = graph.getShortestPath(currentCityIdx, destIdx);
        if (bestPath.empty()) {
            std::cout << "No path exists between these cities !" << std::endl;
            continue;
        }

        int chosenLorryIdx = 0;
        double bestTotalCost = 1e18;
        
        if (mode == 1) {
            std::cout << "Select a lorry number (1-3): ";
            int choice;
            std::cin >> choice;
            if (choice < 1 || choice > 3) choice = 1;
            chosenLorryIdx = choice - 1;
        } else {
            // Extra feature: compare all three lorries
            std::cout << "\n=== Calculating costs for all lorries ===" << std::endl;
            for (int i = 0; i < 3; ++i) {
                auto [cost, dist, finalFuel, refuels] =
                    lorries[i].simulateJourney(graph, bestPath, firstJourney ? 0.0 : currentFuelLevel);
                
                std::cout << "\n" << lorries[i].getName() << ":" << std::endl;
                std::cout << "  Total distance: " << std::fixed << std::setprecision(2) << dist << " miles" << std::endl;
                std::cout << "  Total fuel cost: GBP " << cost << std::endl;

                if (cost < bestTotalCost) {
                        bestTotalCost = cost;
                        chosenLorryIdx = i;
                }
            }
            std::cout << "\n>>> Recommended: " << lorries[chosenLorryIdx].getName()
                      << " (GBP " << std::fixed << std::setprecision(2) << bestTotalCost << ")" << std::endl;
        }

        // Simulate the chosen (or recommended) lorry
        auto [totalCost, totalDist, finalFuel, refuelLogs] =
            lorries[chosenLorryIdx].simulateJourney(graph, bestPath, firstJourney ? 0.0 : currentFuelLevel);
        
        // Result Journey Summary (FINAL OUTPUT)
        std::cout << "\n=== Journey Summary ===" << std::endl;
        std::cout << "Route: ";
        for (size_t i = 0; i < bestPath.size(); ++i) {
            std::cout << graph.getCities()[bestPath[i]].name;
            if (i < bestPath.size() - 1) std::cout << " -> ";
        }
        std::cout << std::endl;

        std::cout << "Total distance: " << std::fixed << std::setprecision(2) << totalDist << " miles" << std::endl;
        std::cout << "Total fuel cost: GBP " << std::fixed << std::setprecision(2) << totalCost << std::endl;
        std::cout << "Fuel remaining: " << std::fixed << std::setprecision(2) << finalFuel << " litres" << std::endl;

        std::cout << "\nRefuelling stops:" << std::endl;
        if (refuelLogs.empty()) {
            std::cout << "  No refuelling required." << std::endl;
        } else {
            for (const auto& log : refuelLogs) {
                std::cout << "  At " << log.city << ": bought "
                          << std::fixed << std::setprecision(2) << log.litres
                          << "L for GBP " << log.cost << std::endl;
            }
        }

        currentFuelLevel = finalFuel;
        currentCityIdx = destIdx;
        firstJourney = false;
        
        // Extra feature: continue journey with current fuel level
        std::cout<< "\nContinue journey to another destination ? (y/n): ";
        char choice;
        std::cin >> choice;
        if (std::tolower(choice) != 'y') break;
    }

    std::cout << "\nThank you for using the Route Planner !" << std::endl;
    return 0;
}
