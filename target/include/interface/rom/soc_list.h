/**
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: Doubly linked list.
 */

#ifndef __SOC_LIST_H__
#define __SOC_LIST_H__

#include <soc_types_rom.h>

// ****************************************************************************
EXT_START_HEADER

// ****************************************************************************
// *****************************************************************************
// Remarks: This file renames the linked list interface and reimplements it using the LTOS interface.
// *****************************************************************************
typedef struct EXT_LIST {
    struct EXT_LIST *prev;
    struct EXT_LIST *next;
} EXT_LIST;

typedef EXT_LIST ext_list;

/*
Function description:
Initialize the head node, note that this node is only used for management, not the data node entered by the user.
 */
td_void uapi_list_init(EXT_LIST *list);

/*
Function description:
UAPI_ListTailInsert inserts a new node after the last node
 */
td_void uapi_list_tail_insert(EXT_LIST *list, EXT_LIST *node);

/*
¹¦ÄÜÃèÊö£º
    UAPI_ListHeadInsert Insert a new node before the first node
 */
td_void uapi_list_head_insert(EXT_LIST *list, EXT_LIST *node);

/*
Function description:
Remove a node from the list
 */
td_void uapi_list_delete(EXT_LIST *node);

/*
Function description:
Determine if the linked list is empty
 */
td_bool uapi_is_list_empty(EXT_LIST *list);

/*
Function description:
To initialize the linked list, the management node is emptied, and the other member nodes are still connected in a
doubly linked list.
 */
td_void uapi_list_del_init(EXT_LIST *list);

/* Get the first node pointer */
#define ext_list_first(pstObject) ((pstObject)->next)

/* Get the last node pointer */
#define ext_list_last(pstObject) ((pstObject)->prev)


/*
Function description:
Get the pointer address of a struct containing a double-linked list.
Example:
Typedef struct
{
Td_u32 taskId
...
EXT_LIST_NODE stTimerList
}TASK_ST
When you want to find the PCB pointer of the task of the next timer through the TimerList, you can use the following
usage: ext_list_entry(pstTaskCB->stTimerList.next, LOS_TASK_CB, stTimerList)
 */
#define ext_list_entry(item, type, member) ((type *)((char *)item - ext_offset_of_member(type, member)))

/*
function list:
The pointer entry address of the structure of each member node is traversed by LIST.
 */
#define ext_list_foreach_entry(item, list, type, member)                             \
    for (item = ext_list_entry((list)->next, type, member); &item->member != (list); \
        item = ext_list_entry(item->member.next, type, member))

/*
function list:
The pointer entry address of the structure of each member node is traversed by LIST,
and the pointer of the next node is saved to avoid the scene deleted after the current node is processed.
*/
#define uapi_list_for_each_entry_safe(list, item, pnext, type, member)                                               \
    for (item = ext_list_entry((list)->next, type, member), pnext = ext_list_entry(item->member.next, type, member); \
        &item->member != (list); item = pnext, pnext = ext_list_entry(item->member.next, type, member))

#define ext_list_for_each_entry_continue_safe(pitem, list, item, pnext, type, member)                                \
    for (item = ext_list_entry((pitem)->next, type, member), pnext = ext_list_entry(item->member.next, type, member); \
        &((item)->member) != (list); item = pnext, pnext = ext_list_entry(pnext->member.next, type, member))

/* Simple implementation of doubly linked list */
#define ext_list_head(list) EXT_LIST list = { &(list), &(list) }

#define ext_list_for_each(item, list) for ((item) = (list)->next; (item) != (list); (item) = (item)->next)

#define ext_list_for_each_safe(item, pnext, list) \
    for (item = (list)->next, pnext = item->next; item != (list); item = pnext, pnext = item->next)

EXT_END_HEADER
#endif // __SOC_STDLIB_H__
