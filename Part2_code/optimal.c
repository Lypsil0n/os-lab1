#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct trace{
    int reference;
    int next_hit;
};

int main(int argc, char** argv) {
    int no_physical_pages = atoi(argv[1]);
    int page_size = atoi(argv[2]);
    char *filename = argv[3];
    struct trace *buffer = (struct trace*)malloc(no_physical_pages * sizeof(struct trace));
    int page_faults = 0;
    int current_row = 0;
    unsigned int address;

    FILE *file = fopen(filename, "r");

    for (int i = 0; i < no_physical_pages; i++) {
        buffer[i].reference = -1;
        buffer[i].next_hit = -1;
    }

    while (fscanf(file, "%d", &address) != EOF) {
        int page = address / page_size;
        int found = 0;
        int current_pointer = ftell(file);
        printf("%d", current_pointer);
        for(int i = 0; i < no_physical_pages; i++) {
            if(buffer[i].reference == page) {
                found = 1;
                break;
            }
        }

        if(!found){
            page_faults++;
            int empty_slot_index;
            int empty_slots = 0;

            for(empty_slot_index = 0; empty_slot_index < no_physical_pages; empty_slot_index++) {
                if(buffer[empty_slot_index].reference == -1) {
                    empty_slots = 1;
                    break;
                }
            }

            int row = 0;
            int value;

            if(empty_slots){
                buffer[empty_slot_index].reference = page;
                fseek(file, 0, SEEK_SET);
                while (fscanf(file, "%d", &value) != EOF) {
                    if(value == page && current_row < row){
                        break;
                    }
                    row++;
                }
                buffer[empty_slot_index].next_hit = row;
            } else {
                int furthest_index = 0;
                int furthest_entry = buffer[0].next_hit;

                for(int i = 1; i < no_physical_pages; i++){
                    if(buffer[i].next_hit > furthest_entry){
                        furthest_index = i;
                        furthest_entry = buffer[i].next_hit;
                    }
                }
                
                buffer[furthest_index].reference = page;
                fseek(file, 0, SEEK_SET);
                while (fscanf(file, "%d", &value) != EOF) {
                    if(value == page && current_row < row){
                        break;
                    }
                    row++;
                }
                buffer[furthest_index].next_hit = row;
            }
        }
        current_row++;
        fseek(file, current_pointer, SEEK_SET);
    }

    free(buffer);
    fclose(file);

    printf("No physical pages = %d, page size = %d\n", no_physical_pages, page_size);
    printf("Number of page faults: %d\n", page_faults);

	return 0;
}
