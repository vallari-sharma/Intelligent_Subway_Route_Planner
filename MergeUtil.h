#ifndef MERGE_UTIL_H
#define MERGE_UTIL_H

#include <vector>
#include <queue>

class MergeUtil {
private:
    // Helper struct for priority queue
    struct DataNode {
        int value;
        
        // Overload operator for priority queue (min heap)
        bool operator>(const DataNode& other) const {
            return value > other.value;
        }
    };

public:
    MergeUtil();
    
    // Merge crowd data using optimal merge pattern
    int mergeCrowdData(const std::vector<int>& crowdLevels) const;
};

#endif // MERGE_UTIL_H