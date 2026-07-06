#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timeline.h"

#define MAX_LINE_LENGTH 1024
// Furkan, Okcu
// --- PROTOTYPES ---
void process_commands(const char* filename);
void display_timeline_tree(TimelineNode* node, int level);

// --- MAIN ---
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <commands_file.txt>\n", argv[0]);
        return 1;
    }

    process_commands(argv[1]);

    return 0;
}
void display_timeline_tree(TimelineNode* node, int level) {

    if (!node) {
	   
	    return;

    }

    int indent = level * 4;

    for (int i = 0; i < indent; i++) {
	   
	    printf(" ");

    }

    printf("Timeline [%d]:\n", node->timeline_id);

    for (int i = 0; i < node->event_count; i++) {
	    
	    for (int j = 0; j < indent + 2; j++) {
		   
		    printf(" ");

	    }

            printf("Event [%d]: %s (Impact: %d)\n",
               node->events[i]->event_id,
               node->events[i]->description,
               node->events[i]->impact_score);
    }

    for (int i = 0; i < node->branch_count; i++) {
	    
	    for (int j = 0; j < indent + 2; j++) {
		   
		    printf(" ");

	    }

            printf("-> Branch:\n");
            display_timeline_tree(node->branches[i], level + 1);
    }
}

// --- PARSER IMPLEMENTATION ---
void process_commands(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open file %s\n", filename);
        return;
    }

    char line_buffer[MAX_LINE_LENGTH];
    TimelineNode* root_timeline = NULL; // Pointer to the Prime Timeline

    while (fgets(line_buffer, sizeof(line_buffer), file)) {
        // Remove trailing newline characters
        line_buffer[strcspn(line_buffer, "\r\n")] = 0;

        // Skip empty lines
        if (strlen(line_buffer) == 0 || line_buffer[0] == '#') continue;

        // --- STUDENT IMPLEMENTATION REQUIRED HERE ---
        
        // Step 1: Identify the command (INIT, ADD_EVENT, BRANCH, PRUNE, PRINT)
        // Step 2: Extract the arguments from the string.
        // HINT for ADD_EVENT: Extracting the string between double quotes can be done by manual pointer manipulation (strchr).
        // Step 3: Find the target timeline (you may need a helper search function).
        // Step 4: Call the appropriate function from timeline.h.

        char command[32];
        sscanf(line_buffer, "%s", command);

        if (strcmp(command, "INIT") == 0) {
            // int id;
            // Extract ID and create root_timeline
	    int id;
	    sscanf(line_buffer, "INIT %d", &id);
	    root_timeline = create_timeline(NULL, id);
	}

        else if (strcmp(command, "ADD_EVENT") == 0) {
            // Extract timeline_id, event_id, description string, and impact_score
            // BE CAREFUL: The description contains spaces!

	    int t_id, e_id, impact;
	    char desc[1024];

	    char *q_open = strchr(line_buffer, '\"');
            char *q_close = strrchr(line_buffer, '\"');

            if (q_open && q_close && q_open != q_close) {
                int desc_len = q_close - q_open - 1;
                strncpy(desc, q_open + 1, desc_len);
                desc[desc_len] = '\0';

                sscanf(line_buffer, "ADD_EVENT %d %d", &t_id, &e_id);
                sscanf(q_close + 1, "%d", &impact);

                TimelineNode* target = find_timeline(root_timeline, t_id);
                if (target) {
                    add_event(target, e_id, desc, impact); 
                } else {
                    printf("Timeline %d not found\n", t_id);
                }
            }
        }
        else if (strcmp(command, "BRANCH") == 0) {
            // Extract parent_id and new_id
            // Find parent timeline, then call branch_timeline

	    int p_id, n_id;
            sscanf(line_buffer, "BRANCH %d %d", &p_id, &n_id);
            TimelineNode* parent = find_timeline(root_timeline, p_id);
            if (parent) {
                
		   branch_timeline(parent, n_id); 
	}
	
	}
        else if (strcmp(command, "PRUNE") == 0) {
            // Extract target_id
            // Call prune_branch
	    
	    int t_id;
            sscanf(line_buffer, "PRUNE %d", &t_id);
            if (root_timeline && root_timeline->timeline_id == t_id) {
                destroy_timeline(root_timeline);
                root_timeline = NULL;
            } else {
                prune_branch(root_timeline, t_id); 
            }

        }
        else if (strcmp(command, "PRINT") == 0) {
            // Implement a tree traversal to print the timeline state

            printf("--- BTA TIMELINE REPORT ---\n");
            display_timeline_tree(root_timeline, 0); 
	    printf("----------------------------\n");
        }
    }
    // --- FINAL CLEANUP ---
    // Make sure to completely destroy the root_timeline here before the program exits
    // Otherwise, Valgrind will report memory leaks!

    if (root_timeline) {

	    destroy_timeline(root_timeline); 

    }

    fclose(file);
}
