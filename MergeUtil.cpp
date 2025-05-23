#include "MergeUtil.h"
#include <numeric>
#include <algorithm>

MergeUtil::MergeUtil() {}

// Implementation of optimal merge pattern algorithm to merge crowd data
int MergeUtil::mergeCrowdData(const std::vector<int>& crowdLevels) const {
    if (crowdLevels.empty()) {
        return 0;
    }
    
    if (crowdLevels.size() == 1) {
        return crowdLevels[0];
    }
    
    // Create a min heap (priority queue) for the optimal merge pattern
    std::priority_queue<DataNode, std::vector<DataNode>, std::greater<DataNode>> pq;
    
    // Add all crowd levels to the priority queue
    for (int level : crowdLevels) {
        pq.push({level});
    }
    
    // Merge the two smallest elements until only one remains
    while (pq.size() > 1) {
        DataNode first = pq.top();
        pq.pop();
        
        DataNode second = pq.top();
        pq.pop();
        
        // Merge the two nodes and add back to the queue
        // For crowd data, we take the weighted average
        int mergedValue = (first.value + second.value) / 2;
        pq.push({mergedValue});
    }
    
    // The final element is our merged result
    return pq.top().value;
}