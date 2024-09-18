/********************************************/
/*   list.h                                 */
/*   a dynamic  linked list with a header   */
/********************************************/


#ifndef _LIST_
#define _LIST_

#include "def.h"


/*** Definitions ***/

// List
typedef struct 
{
	NODE head;		// a header
}LIST;


/*** Function prototypes ***/

BOOL L_init(LIST* pList);					// create new list

NODE* L_insert(NODE* pNode, DATA Value, int(*compare)(const void*, const void*));	// add new node after *pNode

BOOL L_delete(NODE* pNode, void (*freeKey)(void*));	// erase node after *pNode

const void* L_find(const NODE* pNode, DATA Value, int(*compare)(const void*, const void*));

void* L_free(LIST* pList, void (*freeKey)(void*)); // free all nodes

void* L_print(const LIST* pList, void(*print)(const void*));	// print all nodes

#endif
