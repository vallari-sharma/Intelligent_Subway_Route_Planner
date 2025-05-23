#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <vector>

// Station structure to represent subway stations
struct Station {
    int id;
    std::string name;
};

// Connection structure to represent connections between stations
struct Connection {
    int fromStationId;
    int toStationId;
    int travelTime;  // in minutes
};

// CrowdData structure to represent crowd congestion data
struct CrowdData {
    int stationId;
    int congestionLevel;  // 0-100 scale (0: empty, 100: extremely crowded)
};

// Route structure to represent a subway route
struct Route {
    std::vector<int> stations;  // Ordered list of station IDs in the route
    int totalTime;              // Total travel time in minutes
    double averageCongestion;   // Average congestion level along the route
    
    Route() : totalTime(0), averageCongestion(0.0) {}
};

#endif // MODELS_H