#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"

typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}

int solveCollision(HashMap *map, int pos){
  int posOriginal = pos;
    pos = (pos +1) % map->capacity;
    while(pos != posOriginal){
  
        if (map->buckets[pos] == NULL || map->buckets[pos]->key == NULL) {
          return pos;
      }
      pos = (pos + 1) % map->capacity;
    }
  return -1;
}

void insertMap(HashMap * map, char * key, void * value) {
int pos = hash(key, map->capacity);
if(map->buckets[pos] == NULL || strcmp(map->buckets[pos]->key, "-1") == 1){
Pair* nuevoElem = malloc(sizeof(Pair));
nuevoElem->value = value;
  nuevoElem->key = key;
  map->buckets[pos] = nuevoElem;
  map->size += 1;
} else{

  int nuevaPos = solveCollision(map, pos);
  Pair* nuevoElem = malloc(sizeof(Pair));
  nuevoElem->value = value;
  nuevoElem->key = key;
  map->buckets[nuevaPos] = nuevoElem;
  map->size += 1;
  }

}

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)
  Pair **oldArray = map->buckets;
  int oldCap = map->capacity;
  int nuevaCap = map->capacity * 2;
  map->capacity = nuevaCap;
  map->size = 0;
  
  Pair ** newArray = (Pair**) malloc(nuevaCap* sizeof(Pair*));

  for(int i = 0; i < nuevaCap; i++){
    newArray[i] = NULL;
  }
  map->buckets = newArray;
  for(int i = 0; i < oldCap; i++){
    Pair *currentP = oldArray[i];
    
    if(currentP != NULL){
      insertMap(map, currentP->key, currentP->value);
      free(currentP);
      }
  }
  free(oldArray);

}

HashMap * createMap(long capacity) {
   HashMap *mapa = (HashMap*) malloc(sizeof(HashMap));
   mapa->buckets = (Pair**) calloc(capacity, sizeof(Pair*));
   mapa->size = 0;
   mapa->capacity = capacity;
   mapa->current = -1;

    return mapa;
}

void eraseMap(HashMap * map,  char * key) {    

  Pair* removeP = searchMap(map, key);
  if(removeP != NULL){
    removeP->key = NULL;
    map->size--;
  }
}

Pair * searchMap(HashMap * map,  char * key) {   
  int index = hash(key, map->capacity);
  while(map->buckets[index] != NULL){
    
    if(is_equal(map->buckets[index]->key, key)){
      map->current = index;
      return map->buckets[index];
    }
    if(map->buckets[index]->key == NULL){
      map->current = -1;
      return NULL;
    }
    index = (index + 1) % map->capacity;
  }
  map->current = -1;
  return NULL;
}

Pair * firstMap(HashMap * map) {
  
  if(map == NULL || map->buckets == NULL) return NULL;
  for(int i = 0; i < map->capacity; i++){
    if(map->buckets[i] != NULL && map->buckets[i]->key != NULL){
      map->current = i;
      return map->buckets[i];
    } 
  }
    return NULL;
}

Pair * nextMap(HashMap * map) {
  if(map == NULL || map->buckets == NULL) return NULL;

  for(int i = map->current + 1; i < map->capacity; i++){
    if(map->buckets[i] != NULL && map->buckets[i]->key != NULL){
      map->current = i;
      return map->buckets[i];
    }

  }
    return NULL;
}
