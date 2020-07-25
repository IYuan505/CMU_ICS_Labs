#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include "cachelab.h"
#include <string.h>

typedef struct Cache_item
{
    unsigned long tag;
    struct Cache_item* next;
    struct Cache_item* pre;
} cache_item;

typedef struct Queue
{
	cache_item* front;
	cache_item* last;
	int cur_size;
} queue;


void enqueue(queue* q, cache_item* item){
	item->next = NULL;
	item->pre = NULL;
	if (q->front == NULL){
		q->front = item;
		q->last = item;
		q->cur_size = 1;
	}
	else{
		q->last->next = item;
		item->pre = q->last;
		item->next = NULL;
		q->last = item;
		q->cur_size += 1;
	}
}

void dequeue(queue* q){
	cache_item* cur_front = q->front;
	q->front = cur_front->next;
	if(cur_front->next)
	    cur_front->next->pre = NULL;
	q->cur_size--;
	free(cur_front);
}

void empty_queue(queue* q){
	cache_item* cur = q->front;
	cache_item* next;
	while(cur){
		next = cur->next;
		free(cur);
		cur = next;
	}
}

int in_queue(queue* q, unsigned long tag){
	// printf("queue address: %p\n", q);
	cache_item* cur = q->front;
	// printf("cur %p\n", q->front);
	while(cur != NULL){
		if(tag==cur->tag){
			q->cur_size--;
			cache_item* cur_pre = cur->pre;
			if(cur_pre==NULL){
				q->front = cur->next;
				if(cur->next)
				    cur->next->pre = NULL;
				else{
				    q->front = NULL;
				    q->last = NULL;
				}
			}
			else{
				cur_pre->next = cur->next;
				if(cur->next)
				    cur->next->pre = cur_pre;
				else{
				    q->last = cur_pre;
				}
			}
			enqueue(q, cur);
			return 1;
		}
		else{
			cur = cur->next;
		}
	}
	return 0;
}


int main(int argc, char* argv[])
{	

    char c;

    int s, E, b;
    FILE * filePointer;

    int verbose=0;

    while( (c=getopt(argc,argv,"hvs:E:b:t:")) != -1){
        switch(c){
        case 'h':
            printf("This is a help function for the cache simulator.");
            break;
        case 'v':
            verbose=1;
	    break;
        case 's':
            s = atoi(optarg);
            break;
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 't':
            filePointer = fopen(optarg,"r");
            break;
        case '?':
        default:
            printf("./tracegen failed to parse its options.\n");
            exit(1);
        }
    }

    int bufferLength =255;
    char buffer[bufferLength];
    char instruction;
    unsigned long address;
    int size;

    int hit_cnt, miss_cnt, eviction_cnt;
    hit_cnt=miss_cnt=eviction_cnt=0;

    int set_mask = (1<<s)-1;

    queue* my_cache = malloc(sizeof(queue)*(1<<s));
    for(int i=0; i<(1<<s); ++i){
        my_cache[i].front = NULL;
	my_cache[i].last = NULL;
	my_cache[i].cur_size = 0;
    }

    while(fgets(buffer, bufferLength, filePointer)) {
        sscanf(buffer, " %c %lx,%d", &instruction, &address, &size);
        int set_number = (address>>b)&set_mask;
        unsigned long tag = (address>>b)>>s;
        queue* selected_set = &my_cache[set_number];
	if (instruction=='L'||instruction=='S'||instruction=='M'){
        	// printf("Set number: %d, Tag: %ld\n", set_number, tag);
		if(verbose) printf("%s", buffer+1);
		if(instruction=='L' || instruction=='S'){
			if(in_queue(selected_set, tag)){
        			hit_cnt+=1;
				if(verbose) printf("hit ");
        		}
        		else{
        			miss_cnt+=1;
				if(verbose) printf("miss ");
        			cache_item* new_item = malloc(sizeof(cache_item));
        			new_item->tag = tag;
				enqueue(selected_set, new_item);
			}
        	}
        	else{
        		if(in_queue(selected_set, tag)){
        			hit_cnt+=2;
        		        if(verbose) printf("hit hit ");
			}
        		else{
        			miss_cnt+=1;
        			hit_cnt+=1;
				if(verbose) printf("miss hit ");
        			cache_item* new_item = malloc(sizeof(cache_item));
        			new_item->tag = tag;
				enqueue(selected_set, new_item);
        		}
        	}
        	if(selected_set->cur_size>E){
        		eviction_cnt+=1;
			if(verbose) printf("eviction");
        		dequeue(selected_set);
        	}
		if(verbose) printf("\n\n");
        }
        else
            continue;
    }
    
    for(int i = 0; i<(1<<s); ++i){
        empty_queue(&my_cache[i]);
    }
    free(my_cache);
    fclose(filePointer);

    printSummary(hit_cnt, miss_cnt, eviction_cnt);
    return 0;
}

