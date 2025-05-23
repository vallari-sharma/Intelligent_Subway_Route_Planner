#include "SubwayMap.h"
#include <queue>
#include <limits>
#include <algorithm>

SubwayMap::SubwayMap() {}

void SubwayMap::addStation(const Station& station) {
    stations[station.id] = station;
}

void SubwayMap::addConnection(int fromStationId, int toStationId, int travelTime) {
    // Check if both stations exist
    if (!stationExists(fromStationId) || !stationExists(toStationId)) {
        return;
    }
    
    // Add connection in both directions (assuming subway lines go both ways)
    connections[fromStationId].push_back({fromStationId, toStationId, travelTime});
    connections[toStationId].push_back({toStationId, fromStationId, travelTime});
}

bool SubwayMap::stationExists(int stationId) const {
    return stations.find(stationId) != stations.end();
}

std::string SubwayMap::getStationName(int stationId) const {
    auto it = stations.find(stationId);
    if (it != stations.end()) {
        return it->second.name;
    }
    return "Unknown Station";
}

std::vector<Station> SubwayMap::getAllStations() const {
    std::vector<Station> result;
    for (const auto& pair : stations) {
        result.push_back(pair.second);
    }
    return result;
}

std::vector<Connection> SubwayMap::getConnectionsFrom(int stationId) const {
    auto it = connections.find(stationId);
    if (it != connections.end()) {
        return it->second;
    }
    return {};
}

// Implementation of Dijkstra's algorithm to find the shortest route
Route SubwayMap::findShortestRoute(int startStationId, int endStationId) const {
    Route route;
    
    // Check if both stations exist
    if (!stationExists(startStationId) || !stationExists(endStationId)) {
        return route;
    }
    
    // Initialize distances with infinity
    std::map<int, int> distances;
    std::map<int, int> previous;
    for (const auto& pair : stations) {
        distances[pair.first] = std::numeric_limits<int>::max();
        previous[pair.first] = -1;
    }
    
    // Distance from start to itself is 0
    distances[startStationId] = 0;
    
    // Priority queue for Dijkstra's algorithm
    // Pair: (distance, stationId)
    std::priority_queue<std::pair<int, int>, 
                        std::vector<std::pair<int, int>>, 
                        std::greater<std::pair<int, int>>> pq;
    
    pq.push({0, startStationId});
    
    while (!pq.empty()) {
        int currentDistance = pq.top().first;
        int currentStationId = pq.top().second;
        pq.pop();
        
        // If we've reached the destination, we can stop
        if (currentStationId == endStationId) {
            break;
        }
        
        // If we've already found a shorter path to this station, skip
        if (currentDistance > distances[currentStationId]) {
            continue;
        }
        
        // Check all connections from the current station
        for (const auto& connection : getConnectionsFrom(currentStationId)) {
            int neighborId = connection.toStationId;
            int newDistance = currentDistance + connection.travelTime;
            
            // If we found a shorter path to the neighbor
            if (newDistance < distances[neighborId]) {
                distances[neighborId] = newDistance;
                previous[neighborId] = currentStationId;
                pq.push({newDistance, neighborId});
            }
        }
    }
    
    // If we couldn't reach the destination
    if (previous[endStationId] == -1) {
        return route;
    }
    
    // Reconstruct the path
    std::vector<int> path;
    for (int at = endStationId; at != -1; at = previous[at]) {
        path.push_back(at);
    }
    
    // Reverse the path to get it from start to end
    std::reverse(path.begin(), path.end());
    
    route.stations = path;
    route.totalTime = distances[endStationId];
    
    return route;
}