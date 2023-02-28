// Author: Christoph Born (53034, 22/041/62)
// Module list: Circular Doubly Linked List, for use in module dict
// based on https://www.informatik.htw-dresden.de/~beck/PSPI/DownloadPraktikum/list/list.h
// requires #include <stdbool.h> !

// --------- data structures ---------

typedef struct lcnct { // list element, consisting of pointers to neighbors & data
    struct lcnct* pPrev;
    struct lcnct* pNext;
    void* pData;
} tLCnct;

typedef struct { // list (head), consisting head element & pointer to current element
    tLCnct head;
    tLCnct* pCurr; // for iterating
} tList;


// --------- API ---------
// In all functions below tList* pList is expected to be obtained with list_create()

// ====== list creation & destruction ======

tList* list_create();
/* creates an empty list or returns NULL on error */

bool list_delete(tList* pList);
/* deletes empty list & returns true, or false if the list is not empty */

// ====== element insertion ======

bool list_insert_before(tList* pList, void* pData);
/* inserts element (containing pData) before current element
   - returns true on success, false otherwise (e.g. when there's no current element) */

bool list_insert_behind(tList* pList, void* pData);
/* inserts element (containing pData) behind current element
   - returns true on success, false otherwise (e.g. when there's no current element) */

bool list_insert_first(tList* pList, void* pData);
/* inserts element (containing pData) as the first element of the list
   - returns true on success, false otherwise */

bool list_insert_last(tList* pList, void* pData);
/* inserts element (containing pData) as the last element of the list
   - returns true on success, false otherwise */

bool list_insert_sorted(tList* pList, void* pData, int(*compare)(void *, void *));
/* inserts element (containing pData) before first element with compare(element, pData) >= 0
     or as the last element of the list
   - doesn't change which element is the current
   - returns true on success, false otherwise */

// ====== element removal ======
// list_remove_* doesn't free data pointer, that's the callers responsibility!

bool list_remove_curr(tList* pList);
/* removes current element (afterwards there is no current element)
   - returns true on success, false otherwise (e.g. when there's no current element) */

bool list_remove_prev(tList* pList);
/* removes element before current element
   - returns true on success, false otherwise (e.g. when there's no current element
     or when the current element is the first in the list) */

bool list_remove_next(tList* pList);
/* removes element behind current element
   - returns true on success, false otherwise (e.g. when there's no current element
     or when the current element is the last in the list) */

bool list_remove_first(tList* pList);
/* removes the first element from the list
   - returns true on success, false otherwise (e.g. when the list is empty) */

bool list_remove_last(tList* pList);
/* removes the last element from the list
   - returns true on success, false otherwise (e.g. when the list is empty) */

// ====== element retrieval ======

// list_get_* returns NULL if the demanded element exceeds the bounds of the list
void* list_get_curr(tList* pList);
/* returns the current element or NULL if there's no current element */

void* list_get_prev(tList* pList);
/* returns the element before the current element or NULL if there's no current element
   - sets this previous element as new current element */

void* list_get_next(tList* pList);
/* returns the element before the current element or NULL if there's no current element
   - sets next element as new current element */

void* list_get_first(tList* pList);
/* returns the first element of the list
   - sets this element as new current element */

void* list_get_last(tList* pList);
/* returns the last element of the list
   - sets this element as new current element */

void* list_get_at(tList* pList, int index);
/* returns element at index, starting from 0
   - sets this element as new current element */

// ====== other ======

bool list_is_empty(tList* pList);
/* returns true if the list contains no elements, false otherwise */

bool list_foreach(tList* pList, bool(*action)(tList* list, void* item, int index));
/* executes action for every element in the list
   - doesn't change which element is the current
   - action is provided with the list, one element respectively and its index
   - if action returns false, the iteration is stopped and false is returned
   - returns true otherwise */