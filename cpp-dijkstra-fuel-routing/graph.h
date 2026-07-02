// PURPOSE:
// This header defines the Graph class and supporting structures (City) used to
// represent the Scottish road network as a weighted directed graph.

// DESIGN RATIONALE:
// - Use an adjacency list (vector of vectors of pairs) because the road
//   network is sparse (few roads compared to possible city pairs). This is
//   far more memory-efficient than an adjacency matrix for this real-world
//   scenario.
// - std::map<std::string, int> provides fast O(log N) city name lookups
//   while keeping the internal representation as integer indices (required
//   for efficient Dijkstra).
// - All data is loaded once at startup from graph.txt as required by the
//   assignment specification.
// - Data members are PRIVATE and accessed through const accessor methods.
//   This is proper encapsulation: external code can read the graph but
//   cannot modify its internals after construction.

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <map>
#include <utility>           // for std::pair

// Represents a single city/town with its fuel station price
struct City {
    std::string name;        // e.g. "Dundee"
    double fuelPrice;        // GBP per litre (read from graph.txt)
};

struct RefuelLog {
    std::string city;
    double litres;
    double cost;
};

class Graph {
public:
    // Loads the entire map from the text file specified in the assignment.
    // Returns true on success, false on failure so the caller can react.
    bool loadData(const std::string& filename);

    // Returns the shortest path (by total distance) from startIdx to endIdx
    // as a vector of city indices. Uses Dijkstra's algorithm (see .cpp).
    // Returns empty vector if no path exists.
    std::vector<int> getShortestPath(int startIdx, int endIdx) const;

    // Utility: prints all available cities with their fuel prices.
    void printAvailableCities() const;

    // --- Read-only accessors (encapsulation) ---
    const std::vector<City>& getCities() const { return cities; }
    const std::vector<std::vector<std::pair<int, double>>>& getAdj() const { return adj; }

    // Helper lookups: avoid exposing the internal map.
    int  findCityIndex(const std::string& name) const;    // -1 if not found
    bool hasCity(const std::string& name)       const;

private:
    std::vector<City> cities;
    std::vector<std::vector<std::pair<int, double>>> adj;
    std::map<std::string, int> cityToIndex;
};

#endif