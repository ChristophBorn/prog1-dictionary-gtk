#include <stdlib.h>
#include <stdbool.h>

#include "list.h"

tList* list_create() {
    tList* pList = malloc(sizeof(tList));
    if(!pList)  return NULL;

    pList->head.pPrev = &pList->head;
    pList->head.pNext = &pList->head;
    pList->head.pData = NULL;

    pList->pCurr = NULL;

    return pList;
}
bool list_delete(tList* pList) {
    if(!list_is_empty(pList))  return false;
    
    free(pList);
    return true;
}

/*private*/ bool insert_behind_cnct(tLCnct* pRef, void* pData) {
    tLCnct* pCnct = malloc(sizeof(tLCnct));
    if(!pCnct)  return false;

    pCnct->pData = pData;
    pCnct->pPrev = pRef;
    pCnct->pNext = pRef->pNext;

    pRef->pNext->pPrev = pCnct;
    pRef->pNext = pCnct;

    return true;
}
bool list_insert_before(tList* pList, void* pData) {
    if(!pList->pCurr)  return false;

    return insert_behind_cnct(pList->pCurr->pPrev, pData);
}
bool list_insert_behind(tList* pList, void* pData) {
    if(!pList->pCurr)  return false;

    return insert_behind_cnct(pList->pCurr, pData);
}
bool list_insert_first(tList* pList, void* pData) {
    return insert_behind_cnct(&pList->head, pData);
}
bool list_insert_last(tList* pList, void* pData) {
    return insert_behind_cnct(pList->head.pPrev, pData);
}

bool list_insert_sorted(tList* pList, void* pData, int(*compare)(void *, void *)) {
    tLCnct* pCurr = pList->pCurr;
    void* item;
    bool ret;

    for(item=list_get_first(pList); (item && compare(item, pData) < 0); item=list_get_next(pList));
    ret = (item ? list_insert_before(pList, pData) : list_insert_last(pList, pData));
    
    pList->pCurr = pCurr; // restore previous pCurr
    return ret;
}

/*private*/ bool remove_cnct(tLCnct* pCnct) {
    if(!pCnct->pData) return false; // prevent removing head
    
    pCnct->pPrev->pNext = pCnct->pNext;
    pCnct->pNext->pPrev = pCnct->pPrev;
    free(pCnct);

    return true;
}
bool list_remove_curr(tList* pList) {
    if(!pList->pCurr)  return false;

    if(!remove_cnct(pList->pCurr))  return false;
    
    pList->pCurr = NULL;
    return true;
}
bool list_remove_prev(tList* pList) {
    if(!pList->pCurr)  return false;

    return remove_cnct(pList->pCurr->pPrev);
}
bool list_remove_next(tList* pList) {
    if(!pList->pCurr)  return false;

    return remove_cnct(pList->pCurr->pNext);
}
bool list_remove_first(tList* pList) {
    return remove_cnct(pList->head.pNext);
}
bool list_remove_last(tList* pList) {
    return remove_cnct(pList->head.pPrev);
}

void* list_get_curr(tList* pList) {
    if(!pList->pCurr)  return NULL;
    
    return pList->pCurr->pData;
}
void* list_get_prev(tList* pList) {
    if(!pList->pCurr)  return NULL;
    
    pList->pCurr = pList->pCurr->pPrev;
    return list_get_curr(pList);
}
void* list_get_next(tList* pList) {
    if(!pList->pCurr)  return NULL;

    pList->pCurr = pList->pCurr->pNext;
    return list_get_curr(pList);
}
void* list_get_first(tList* pList) {
    pList->pCurr = pList->head.pNext;
    return list_get_curr(pList);
}
void* list_get_last(tList* pList) {
    pList->pCurr = pList->head.pPrev;
    return list_get_curr(pList);
}
void* list_get_at(tList* pList, int index) {
    void* item;
    int i;
    for(
        item=list_get_first(pList), i=0;
        item                     && i < index;
        item=list_get_next(pList),  i++
    );

    return item;
}

bool list_is_empty(tList* pList) {
    return (pList->head.pNext == &pList->head);
}

bool list_foreach(tList* pList, bool(*action)(tList* pList, void* item, int index)) {
    tLCnct* pCurr = pList->pCurr;
    void* item;
    int i;
    bool ret = true;

    for(
        item=list_get_first(pList), i=0;
        item;
        item=list_get_next(pList),  i++
    ) {
        if(!action(pList, item, i)) {
            ret = false;
            break;
        }
    }

    pList->pCurr = pCurr; // restore previous pCurr
    return ret;
}