#ifndef SUBWAY_MAP_H
#define SUBWAY_MAP_H

#include <vector>
#include <map>
#include <string>
#include "models.h"

class SubwayMap {
private:
    // Map of station ID to Station object
    std::map<int, Station> stations;
    
    // Adjacency list representation of the subway map
    // Key: station ID, Value: vector of connections from this station
    std::map<int, std::vector<Connection>> connections;

public:
    SubwayMap();
    
    // Add a station to the map
    void addStation(const Station& station);
    
    // Add a connection between two stations
    void addConnection(int fromStationId, int toStationId, int travelTime);
    
    // Check if a station exists
    bool stationExists(int stationId) const;
    
    // Get station name by ID
    std::string getStationName(int stationId) const;
    
    // Get all stations
    std::vector<Station> getAllStations() const;
    
    // Get all connections from a station
    std::vector<Connection> getConnectionsFrom(int stationId) const;
    
    // Find the shortest route between two stations using Dijkstra's algorithm
    Route findShortestRoute(int startStationId, int endStationId) const;
};

#endif // SUBWAY_MAP_H