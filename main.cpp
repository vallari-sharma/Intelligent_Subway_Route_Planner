#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include "SubwayMap.h"
#include "CrowdManager.h"
#include "models.h"

// Sample data initialization
void initializeSampleData(SubwayMap& subwayMap, CrowdManager& crowdManager) {
    // Create stations
    Station s1{1, "Grand Central"};
    Station s2{2, "Times Square"};
    Station s3{3, "Union Square"};
    Station s4{4, "Brooklyn Bridge"};
    Station s5{5, "Central Park"};
    Station s6{6, "Wall Street"};
    
    // Add stations to the map
    subwayMap.addStation(s1);
    subwayMap.addStation(s2);
    subwayMap.addStation(s3);
    subwayMap.addStation(s4);
    subwayMap.addStation(s5);
    subwayMap.addStation(s6);
    
    // Add connections between stations (with travel time in minutes)
    subwayMap.addConnection(1, 2, 5);  // Grand Central to Times Square: 5 min
    subwayMap.addConnection(2, 3, 7);  // Times Square to Union Square: 7 min
    subwayMap.addConnection(3, 4, 10); // Union Square to Brooklyn Bridge: 10 min
    subwayMap.addConnection(1, 5, 8);  // Grand Central to Central Park: 8 min
    subwayMap.addConnection(5, 3, 6);  // Central Park to Union Square: 6 min
    subwayMap.addConnection(3, 6, 12); // Union Square to Wall Street: 12 min
    subwayMap.addConnection(4, 6, 4);  // Brooklyn Bridge to Wall Street: 4 min
    
    // Initialize crowd data (congestion level from 0-100)
    std::vector<CrowdData> crowdDataSources;
    
    // Sensor data
    crowdDataSources.push_back({1, 75}); // Grand Central is crowded
    crowdDataSources.push_back({2, 90}); // Times Square is very crowded
    crowdDataSources.push_back({3, 40}); // Union Square is moderately crowded
    crowdDataSources.push_back({4, 20}); // Brooklyn Bridge is not crowded
    crowdDataSources.push_back({5, 30}); // Central Park is not crowded
    crowdDataSources.push_back({6, 60}); // Wall Street is crowded
    
    // Mobile app data
    crowdDataSources.push_back({1, 70}); // Grand Central
    crowdDataSources.push_back({2, 85}); // Times Square
    crowdDataSources.push_back({3, 45}); // Union Square
    crowdDataSources.push_back({4, 25}); // Brooklyn Bridge
    crowdDataSources.push_back({5, 35}); // Central Park
    crowdDataSources.push_back({6, 55}); // Wall Street
    
    // Transit authority data
    crowdDataSources.push_back({1, 80}); // Grand Central
    crowdDataSources.push_back({2, 95}); // Times Square
    crowdDataSources.push_back({3, 50}); // Union Square
    crowdDataSources.push_back({4, 15}); // Brooklyn Bridge
    crowdDataSources.push_back({5, 25}); // Central Park
    crowdDataSources.push_back({6, 65}); // Wall Street
    
    // Process and merge crowd data
    crowdManager.processCrowdData(crowdDataSources);
}

// Display main menu
void displayMenu() {
    std::cout << "\n===== INTELLIGENT SUBWAY ROUTE PLANNER =====\n";
    std::cout << "1. Find shortest route\n";
    std::cout << "2. Find least crowded route\n";
    std::cout << "3. Display all stations\n";
    std::cout << "4. Display crowd levels\n";
    std::cout << "5. Simulate crowd change\n";
    std::cout << "6. Exit\n";
    std::cout << "Enter your choice: ";
}

// Get valid station ID from user
int getValidStationId(const SubwayMap& subwayMap, const std::string& prompt) {
    int stationId;
    bool validInput = false;
    
    do {
        std::cout << prompt;
        if (!(std::cin >> stationId)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        if (!subwayMap.stationExists(stationId)) {
            std::cout << "Station ID does not exist. Please try again.\n";
            continue;
        }
        
        validInput = true;
    } while (!validInput);
    
    return stationId;
}

int main() {
    // Initialize subway map and crowd manager
    SubwayMap subwayMap;
    CrowdManager crowdManager(&subwayMap);
    
    // Load sample data
    initializeSampleData(subwayMap, crowdManager);
    
    int choice;
    bool running = true;
    
    while (running) {
        displayMenu();
        
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        switch (choice) {
            case 1: {
                // Find shortest route
                int startId = getValidStationId(subwayMap, "Enter start station ID: ");
                int destId = getValidStationId(subwayMap, "Enter destination station ID: ");
                
                Route route = subwayMap.findShortestRoute(startId, destId);
                
                if (route.stations.empty()) {
                    std::cout << "No route found between these stations.\n";
                } else {
                    std::cout << "\n----- Shortest Route -----\n";
                    std::cout << "From: " << subwayMap.getStationName(startId) << " to " 
                              << subwayMap.getStationName(destId) << "\n";
                    std::cout << "Total travel time: " << route.totalTime << " minutes\n";
                    
                    std::cout << "Route: ";
                    for (size_t i = 0; i < route.stations.size(); i++) {
                        std::cout << subwayMap.getStationName(route.stations[i]);
                        if (i < route.stations.size() - 1) {
                            std::cout << " -> ";
                        }
                    }
                    std::cout << "\n";
                }
                break;
            }
            case 2: {
                // Find least crowded route
                int startId = getValidStationId(subwayMap, "Enter start station ID: ");
                int destId = getValidStationId(subwayMap, "Enter destination station ID: ");
                
                Route route = crowdManager.findLeastCrowdedRoute(startId, destId);
                
                if (route.stations.empty()) {
                    std::cout << "No route found between these stations.\n";
                } else {
                    std::cout << "\n----- Least Crowded Route -----\n";
                    std::cout << "From: " << subwayMap.getStationName(startId) << " to " 
                              << subwayMap.getStationName(destId) << "\n";
                    std::cout << "Total travel time: " << route.totalTime << " minutes\n";
                    std::cout << "Average congestion level: " << route.averageCongestion << "%\n";
                    
                    std::cout << "Route: ";
                    for (size_t i = 0; i < route.stations.size(); i++) {
                        int stationId = route.stations[i];
                        std::cout << subwayMap.getStationName(stationId) 
                                  << " (" << crowdManager.getStationCongestion(stationId) << "%)";
                        if (i < route.stations.size() - 1) {
                            std::cout << " -> ";
                        }
                    }
                    std::cout << "\n";
                }
                break;
            }
            case 3: {
                // Display all stations
                std::cout << "\n----- All Stations -----\n";
                auto stations = subwayMap.getAllStations();
                for (const auto& station : stations) {
                    std::cout << "ID: " << station.id << " - " << station.name << "\n";
                }
                break;
            }
            case 4: {
                // Display crowd levels
                std::cout << "\n----- Current Crowd Levels -----\n";
                auto stations = subwayMap.getAllStations();
                for (const auto& station : stations) {
                    int congestion = crowdManager.getStationCongestion(station.id);
                    std::string level;
                    
                    if (congestion < 30) level = "Low";
                    else if (congestion < 60) level = "Moderate";
                    else if (congestion < 80) level = "High";
                    else level = "Very High";
                    
                    std::cout << station.name << ": " << congestion << "% (" << level << ")\n";
                }
                break;
            }
            case 5: {
                // Simulate crowd change
                int stationId = getValidStationId(subwayMap, "Enter station ID to change crowd level: ");
                int newLevel;
                
                do {
                    std::cout << "Enter new congestion level (0-100): ";
                    if (!(std::cin >> newLevel) || newLevel < 0 || newLevel > 100) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid input. Please enter a number between 0 and 100.\n";
                    } else {
                        break;
                    }
                } while (true);
                
                crowdManager.updateStationCongestion(stationId, newLevel);
                std::cout << "Congestion level updated for " 
                          << subwayMap.getStationName(stationId) << " to " << newLevel << "%\n";
                break;
            }
            case 6:
                // Exit
                std::cout << "Thank you for using the Intelligent Subway Route Planner!\n";
                running = false;
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }
    }
    
    return 0;
}