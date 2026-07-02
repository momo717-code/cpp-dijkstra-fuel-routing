// This file contains the implementation of the Graph class.
// It demonstrates use of standard C++ data structures and algorithms
// (adjacency list + priority-queue Dijkstra) as required for the report.

#include "graph.h"
#include <iomanip>
#include <fstream>
#include <iostream>
#include <queue>
#include <limits>
#include <algorithm>    // for std::reverse

const double INF = 1e18;    // A very large number used as "infinity" in Dijkstra

bool Graph::loadData(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open graph.txt" << std::endl;
        return false;
    }

    int numCities;
    file >> numCities;
    cities.resize(numCities);
    cityToIndex.clear();

    // Read city names and fuel prices
    for (int i = 0; i < numCities; ++i) {
        file >> cities[i].name >> cities[i].fuelPrice; 
        cityToIndex[cities[i].name] = i;
    }

    int numEdges;
    file >> numEdges;
    adj.assign(numCities, {});      // initialise empty adjacency lists

    // Read directed edges (from -> to -> distance)
    for (int i = 0; i < numEdges; ++i) {
        std::string from, to;
        double dist;
        file >> from >> to >> dist;
        auto itFrom = cityToIndex.find(from);
        auto itTo = cityToIndex.find(to);
        if (itFrom != cityToIndex.end() && itTo != cityToIndex.end()) {
            adj[itTo->second].emplace_back(itFrom->second, dist);
            adj[itFrom->second].emplace_back(itTo->second, dist);
        }
    }
    file.close();
    std::cout << "Successfully loaded " << numCities << " Cities and " << numEdges << " roads." << std::endl;
    return true;
}

std::vector<int> Graph::getShortestPath(int startIdx, int endIdx) const {
    // === DIJKSTRA’S ALGORITHM IMPLEMENTATION ===
    // Why Dijkstra?
    //   - All edge weights (distances) are positive → guaranteed correct.
    //   - Priority queue gives O((V+E) log V) performance, excellent for
    //     this small Scottish road network.
    //   - We only need shortest path by distance (assignment requirement);
    //     fuel cost is calculated separately in the Vehicle class.

    int n = static_cast<int>(cities.size());
    std::vector<double> dist(n, INF);       // shortest distance to each node
    std::vector<int> prev(n, -1);           // predecessor for path reconstruction
    dist[startIdx] = 0.0;

    // Min-heap priority queue: {distance, node_index}
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<std::pair<double, int>>> pq;
    pq.emplace(0.0, startIdx);

    while (!pq.empty()) {
        auto [cost, u] = pq.top();
        pq.pop();
        if (cost > dist[u]) continue;       // outdated entry

        for (const auto& [v, weight] : adj[u]) {
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.emplace(dist[v], v);
            }
        }
    }

    if (dist[endIdx] == INF) return {};     // no path

    // Reconstruct path by following predecessors
    std::vector<int> path;
    for (int at = endIdx; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

void Graph::printAvailableCities() const {
    std::cout << "\nAvailable cities (with fuel prices):" << std::endl;
    for (const auto& city: cities) {
        std::cout << "  - " << city.name
                  << "  (GBP " << std::fixed << std::setprecision(2)
                  << city.fuelPrice << "/L)" << std::endl;
    }
}   

int Graph::findCityIndex(const std::string& name) const {
    auto it = cityToIndex.find(name);
    return (it == cityToIndex.end()) ? -1 : it->second;
}

bool Graph::hasCity(const std::string& name) const {
    return cityToIndex.find(name) != cityToIndex.end();
}
