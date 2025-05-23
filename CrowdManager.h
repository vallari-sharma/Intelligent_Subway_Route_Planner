#ifndef CROWD_MANAGER_H
#define CROWD_MANAGER_H

#include <map>
#include <vector>
#include "models.h"
#include "SubwayMap.h"
#include "MergeUtil.h"

class CrowdManager {
private:
    // Reference to the subway map
    SubwayMap* subwayMap;
    
    // Map of station ID to congestion level
    std::map<int, int> stationCongestion;
    
    // MergeUtil for merging crowd data
    MergeUtil mergeUtil;
    
    // Calculate a weighted travel time based on congestion
    int calculateWeightedTravelTime(int baseTime, int congestionLevel) const;

public:
    CrowdManager(SubwayMap* map);
    
    // Process and merge crowd data from multiple sources
    void processCrowdData(const std::vector<CrowdData>& crowdDataSources);
    
    // Get congestion level for a station
    int getStationCongestion(int stationId) const;
    
    // Update congestion level for a station
    void updateStationCongestion(int stationId, int newCongestionLevel);
    
    // Find the least crowded route between two stations using a greedy approach
    Route findLeastCrowdedRoute(int startStationId, int endStationId) const;
};

#endif // CROWD_MANAGER_H