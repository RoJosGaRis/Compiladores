#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define INITIAL_CAPACITY 16
#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

static uint64_t hashKey(const char* key){
  uint64_t hash = FNV_OFFSET;

  for(const char* p = key; *p; p++){
    hash ^= (uint64_t)(unsigned char)(*p);
    hash *= FNV_PRIME;
  }

  return hash;
}

typedef struct {
  const char * key;
  void * value;
} htEntry;

typedef struct  {
  htEntry* entries;
  size_t capacity;
  size_t length;
}ht;

ht* htCreate(void){
  ht* table = malloc(sizeof(ht));
  if(table == NULL){
    return NULL;
  }

  table -> length = 0;
  table -> capacity = INITIAL_CAPACITY;

  table->entries = calloc(table->capacity, sizeof(htEntry));
  if(table->entries == NULL){
    free(table);
    return NULL;
  }
  return table;
}

void htDestroy (ht* table){
  for(size_t i = 0; i < table->capacity; i++){
    free((void*)table->entries[i].key);
  }

  free(table->entries);
  free(table);
}

int main(){
  return 0;
}