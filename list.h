/*
    AUTHOR
    Doppelt verkette Ringliste
    requires #include <stdbool.h>
*/

// === data structures ===
typedef struct lcnct {
    struct lcnct* pPrev;
    struct lcnct* pNext;
    void* pData;
} tLCnct;

typedef struct {
    tLCnct head;
    tLCnct* pCurr;
} tList;


// === API ===
tList* list_create(); // returns NULL on error
bool list_delete(tList* pList); // deletes empty list

bool list_insert_before(tList* pList, void* pData); // inserts item before current item
bool list_insert_behind(tList* pList, void* pData); // inserts item behind current item
bool list_insert_first(tList* pList, void* pData);
bool list_insert_last(tList* pList, void* pData);
bool list_insert_last_test(tList* pList, void* pData); // TODO

// inserts pData before first item with compare(item, pData) >= 0 or at the end
bool list_insert_sorted(tList* pList, void* pData, int(*compare)(void *, void *));

// list_remove_* doesn't free data pointer!
bool list_remove_curr(tList* pList);
bool list_remove_prev(tList* pList);
bool list_remove_next(tList* pList);
bool list_remove_first(tList* pList);
bool list_remove_last(tList* pList);

void* list_get_curr(tList* pList);          // returns NULL if there's no current item
void* list_get_prev(tList* pList);          // returns NULL if there's no current item, sets previous item as new current item
void* list_get_next(tList* pList);          // returns NULL if there's no current item, sets next item as new current item
void* list_get_first(tList* pList);         // sets first item as new current item
void* list_get_last(tList* pList);          // sets last item as new current item
void* list_get_at(tList* pList, int index); // sets this item as new current item or list head

bool list_is_empty(tList* pList);

// executes action for every item, doesn't set current item
bool list_foreach(tList* pList, bool(*action)(tList* list, void* item, int index));