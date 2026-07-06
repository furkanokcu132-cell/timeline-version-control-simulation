#ifndef TIMELINE_H
#define TIMELINE_H

#define INITIAL_CAPACITY 2

typedef struct {
    int event_id;
    char* description; 
    int impact_score;
} Event;

typedef struct TimelineNode {
    int timeline_id;
    
    Event** events; 
    int event_count;
    int event_capacity;

    struct TimelineNode** branches; 
    int branch_count;
    int branch_capacity;

    struct TimelineNode* parent; 
} TimelineNode;

// Core Operations
TimelineNode* create_timeline(TimelineNode* parent, int id);
void add_event(TimelineNode* node, int event_id, const char* desc, int impact);
TimelineNode* branch_timeline(TimelineNode* parent_node, int new_timeline_id);
void prune_branch(TimelineNode* root, int target_timeline_id);
void destroy_timeline(TimelineNode* node);

#endif