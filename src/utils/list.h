#include <stddef.h>
#include <stdbool.h>

#define list_foreach(data, list) for (int i = 0, void* data = NULL; i < list->used ; i++, data = list->list[i].data)

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _LIST_HEADER_
#define _LIST_HEADER_

struct listItem {
    void* data;
};

typedef struct List {
    size_t used;
    size_t size;
    struct listItem* list;
} list_t;

list_t* list_create();
list_t*  list_create_ext(size_t used, size_t size);
void list_clear(list_t* list);
void list_destroy(list_t* list);
bool list_is_empty(list_t* list);
void list_append(void* data, list_t* list);
list_t* list_remove(void* data, list_t* list);
list_t* list_remove_index(int index, list_t* list);
void* list_find(void* data, list_t* list);
void* list_find_at(int index, list_t* list);

#endif

#ifdef __cplusplus
}
#endif