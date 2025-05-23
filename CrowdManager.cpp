#include "CrowdManager.h"
#include <queue>
#include <limits>
#include <algorithm>

CrowdManager::CrowdManager(SubwayMap* map) : subwayMap(map) {}

void CrowdManager::processCrowdData(const std::vector<CrowdData>& crowdDataSources) {
    // Group crowd data by station ID
    std::map<int, std::vector<int>> stationCrowdData;
    
    for (const auto& data : crowdDataSources) {
        stationCrowdData[data.stationId].push_back(data.congestionLevel);
    }
    
    // Merge crowd data for each station using optimal merge pattern
    for (const auto& pair : stationCrowdData) {
        int stationId = pair.first;
        const std::vector<int>& congestionLevels = pair.second;
        
        // Use MergeUtil to merge the congestion levels
        int mergedCongestion = mergeUtil.mergeCrowdData(congestionLevels);
        stationCongestion[stationId] = mergedCongestion;
    }
}

int CrowdManager::getStationCongestion(int stationId) const {
    auto it = stationCongestion.find(stationId);
    if (it != stationCongestion.end()) {
        return it->second;
    }
    return 0; // Default to no congestion if not found
}

void CrowdManager::updateStationCongestion(int stationId, int newCongestionLevel) {
    if (subwayMap->stationExists(stationId)) {
        stationCongestion[stationId] = newCongestionLevel;
    }
}

int CrowdManager::calculateWeightedTravelTime(int baseTime, int congestionLevel) const {
    // Apply a penalty to travel time based on congestion level
    // Higher congestion = higher travel time
    double congestionFactor = 1.0 + (congestionLevel / 100.0);
    return static_cast<int>(baseTime * congestionFactor);
}

// Implementation of a greedy algorithm to find the least crowded route
Route CrowdManager::findLeastCrowdedRoute(int startStationId, int endStationId) const {
    Route route;
    
    // Check if both stations exist
    if (!subwayMap->stationExists(startStationId) || !subwayMap->stationExists(endStationId)) {
        return route;
    }
    
    // Initialize distances with infinity
    std::map<int, int> distances;
    std::map<int, int> previous;
    std::map<int, double> routeCongestion; // Track average congestion for each route
    
    for (const auto& station : subwayMap->getAllStations()) {
        distances[station.id] = std::numeric_limits<int>::max();
        previous[station.id] = -1;
        routeCongestion[station.id] = 0.0;
    }
    
    // Distance from start to itself is 0
    distances[startStationId] = 0;
    routeCongestion[startStationId] = getStationCongestion(startStationId);
    
    // Priority queue for the greedy algorithm
    // Pair: (weighted distance, stationId)
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
        
        // If we've already found a better path to this station, skip
        if (currentDistance > distances[currentStationId]) {
            continue;
        }
        
        // Check all connections from the current station
        for (const auto& connection : subwayMap->getConnectionsFrom(currentStationId)) {
            int neighborId = connection.toStationId;
            int neighborCongestion = getStationCongestion(neighborId);
            
            // Calculate weighted travel time based on congestion
            int weightedTime = calculateWeightedTravelTime(connection.travelTime, neighborCongestion);
            int newDistance = distances[currentStationId] + weightedTime;
            
            // If we found a better path to the neighbor
            if (newDistance < distances[neighborId]) {
                distances[neighborId] = newDistance;
                previous[neighborId] = currentStationId;
                
                // Calculate new average congestion for this route
                int pathLength = 0;
                double totalCongestion = 0.0;
                for (int at = neighborId; at != -1; at = previous[at]) {
                    totalCongestion += getStationCongestion(at);
                    pathLength++;
                }
                
                routeCongestion[neighborId] = totalCongestion / pathLength;
                
                // Use both distance and congestion for priority
                int priority = newDistance;
                pq.push({priority, neighborId});
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
    route.averageCongestion = routeCongestion[endStationId];
    
    return route;
}