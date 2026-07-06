#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timeline.h"

TimelineNode* create_timeline(TimelineNode* parent, int id) {
    // 1. Allocate memory for a new TimelineNode.
    // 2. Allocate memory for the events array using INITIAL_CAPACITY.
    // 3. Allocate memory for the branches array using INITIAL_CAPACITY.
    // 4. Initialize counts to 0 and capacities to INITIAL_CAPACITY.
    // 5. Set the parent pointer and timeline_id.
    // 6. Return the newly created node.
    
    //Malloc ile hafizada yer aciyorum. Task 1
    TimelineNode* nNode = (TimelineNode*)malloc(sizeof(TimelineNode));
    if (!nNode) {
	    
	    return NULL;

    }

    //ID ile üst bagıanimladim. Task 5
    nNode->timeline_id = id;
    nNode->parent = parent;

    //Baslangic degerlerini atadim. Task 3 ve 4
    nNode->event_capacity = INITIAL_CAPACITY;
    nNode->event_count = 0;
    //Task 2
    nNode->events = (Event**)malloc(sizeof(Event*) * nNode->event_capacity);

    //Yine baslangic degerlerini atadim. Task 3 ve 4
    nNode->branch_capacity = INITIAL_CAPACITY;
    nNode->branch_count = 0;
    //Task 3
    nNode->branches = (TimelineNode**)malloc(sizeof(TimelineNode*) * nNode->branch_capacity);
    
    //Task 6
    return nNode; // Placeholder
}

void add_event(TimelineNode* node, int event_id, const char* desc, int impact) {
    // 1. Check if the events array is full (event_count == event_capacity).
    // 2. If full, use realloc() to double the capacity.
    // 3. Allocate memory for a new Event struct.
    // 4. CRITICAL: Perform a DEEP COPY of the 'desc' string (malloc exact space + strcpy).
    // 5. Set event_id and impact_score.
    // 6. Add the new event pointer to the events array and increment the count.
    
    if (!node) return;

    //Task 1 ve 2
    if (node->event_count == node->event_capacity) {
        node->event_capacity *= 2;
        node->events = (Event**)realloc(node->events, sizeof(Event*) * node->event_capacity);
    }

    //Task 3
    Event* nEvent = (Event*)malloc(sizeof(Event));

    //Task 5
    nEvent->event_id = event_id;
    nEvent->impact_score = impact;

    //Task 4
    nEvent->description = (char*)malloc(strlen(desc) + 1);
    strcpy(nEvent->description, desc);

    //Task 6
    node->events[node->event_count++] = nEvent;
}

TimelineNode* branch_timeline(TimelineNode* parent_node, int new_timeline_id) {
    // 1. Check if the parent_node's branches array is full.
    // 2. If full, use realloc() to double the capacity.
    // 3. Create a new timeline node using create_timeline().
    // 4. Add the new timeline pointer to the parent's branches array.
    // 5. Increment the parent's branch_count.
    // 6. Return the new timeline node.

    if (!parent_node) return NULL;

    //Task 1 ve 2
    if (parent_node->branch_count == parent_node->branch_capacity) {
        parent_node->branch_capacity *= 2;
        parent_node->branches = (TimelineNode**)realloc(parent_node->branches, sizeof(TimelineNode*) * parent_node->branch_capacity);
    }

    //Task 3 
    TimelineNode* nNode = create_timeline(parent_node, new_timeline_id);

    //Task 4 ve 5
    parent_node->branches[parent_node->branch_count++] = nNode;
    
    //Task 6
    return nNode; // Placeholder
}

// Optional helper function to find a specific node
TimelineNode* find_timeline(TimelineNode* root, int target_timeline_id) {
    // Implement a search (e.g., Depth-First Search) to find the node.

    if (!root) return NULL;

    //Mevcut dugumun ID'si hedefle eslenirse bu dugumu dondururum.
    if (root->timeline_id == target_timeline_id) return root;

    //Recursive
    for (int i = 0; i < root->branch_count; i++) {
        TimelineNode* found = find_timeline(root->branches[i], target_timeline_id);
        if (found) return found;
    }
    
    return NULL; // Placeholder
}

void prune_branch(TimelineNode* root, int target_timeline_id) {
    // 1. Find the target timeline node using its ID.
    // 2. If found, locate its pointer in its parent's branches array.
    // 3. Remove it from the parent's array.
    // 4. CRITICAL: Shift the remaining elements in the parent's array to the left to close the gap.
    // 5. Call destroy_timeline() on the target node to free all its memory.

    //Task 1
    TimelineNode* target = find_timeline(root, target_timeline_id);
    if (!target || !target->parent) return; 

    //Task 2
    TimelineNode* p = target->parent;
    int index = -1;

    for (int i = 0; i < p->branch_count; i++) {
        if (p->branches[i]->timeline_id == target_timeline_id) {
            index = i;
            break;
        }
    }

    //Task 3
    if (index != -1) {
        
	//Task 5, serbest birakma
        destroy_timeline(target);

	//Task 4, eleman kaydirma
        for (int i = index; i < p->branch_count - 1; i++) {
            p->branches[i] = p->branches[i + 1];
        }
	//Branch sayisini azalt.
        p->branch_count--;
    }
}

void destroy_timeline(TimelineNode* node) {
    // 1. Recursively call destroy_timeline() on all child branches.
    // 2. Free the branches array.
    // 3. Iterate through all events in the events array:
    //    a. Free the dynamically allocated description string.
    //    b. Free the Event struct itself.
    // 4. Free the events array.
    // 5. Free the TimelineNode itself.
    
    if (!node) return;

    //Task 1 recursive destroy_timeline
    for (int i = 0; i < node->branch_count; i++) {
        destroy_timeline(node->branches[i]);
    }
    //Task 2 dal dizisini bellekten serbest birak
    free(node->branches);

    //Task 3 olay dizisindeki her eleman icin
    for (int i = 0; i < node->event_count; i++) {
        free(node->events[i]->description); //burasi a kismi
        free(node->events[i]); //burasi b kismi
    }
    //Task 4 olay dizisini bellekten serbest birak
    free(node->events);
    //Task 5 Timeline Node'un kendisini bellekten serbest birak
    free(node);
}
