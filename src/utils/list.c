#include "list.h"
#include <stdlib.h>

list_t* list_create(){
    list_t* list = malloc(sizeof(list_t));
    list->used = 0;
    list->list = malloc(sizeof(struct listItem)*1);
    list->size = 1;
    return list;
}

list_t*  list_create_ext(size_t used, size_t size){
    list_t* list = malloc(sizeof(list_t));
    list->used = used;
    list->list = malloc(sizeof(struct listItem)*size);
    list->size = size;
    return list;
}

void list_clear(list_t* list){
    for (int i = 0; i < list->used; i++){
        list->list[i].data = NULL;
    }
}

void list_destroy(list_t* list){
    list_clear(list);
    free(list->list);
    free(list);
}

bool list_is_empty(list_t* list){
    return list->used == 0;
}

void list_append(void* data, list_t* list){
    if (list->used == list->size){
        list->size *= 2;
        list->list = realloc(list->list, list->size * sizeof(struct listItem));
    }
    list->list[list->used++].data = data;
}


list_t* list_remove(void* data, list_t* list){
    list_t* new_list = list_create_ext(0, list->size);
    for (int i = 0; i < list->used; i++){
        if (list->list[i].data != data){
            list_append(list->list[i].data, new_list);
        }
    }
    list_destroy(list);
    return new_list;
}

list_t* list_remove_index(int index, list_t* list){
    list_t* new_list = list_create_ext(0, list->size);
    for (int i = 0; i < list->used; i++){
        if (i != index){
            list_append(list->list[i].data, new_list);
        }
    }
    list_destroy(list);
    return new_list;
}

void* list_find(void* data, list_t* list){
    for (int i = 0; i < list->used; i++){
        if (list->list[i].data == data){
            return list->list[i].data;
        }
    }
    return NULL;
}

void* list_find_at(int index, list_t* list){
    return list->list[index].data;
}