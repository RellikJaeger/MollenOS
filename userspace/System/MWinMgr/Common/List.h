/* MollenOS
*
* Copyright 2011 - 2016, Philip Meulengracht
*
* This program is free software : you can redistribute it and / or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation ? , either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.If not, see <http://www.gnu.org/licenses/>.
*
*
* MollenOS Common - List Implementation
*/

#ifndef _LIST_H_
#define _LIST_H_

/* List Includes */
#include <crtdefs.h>
#include <stddef.h>

/* List Structures */
typedef struct _list_node 
{
	/* Identifier */
	int identifier;

	/* Payload */
	void *data;

	/* Link */
	struct _list_node *link;
	
} list_node_t;


typedef struct _list_main 
{
	/* Head and Tail */
	list_node_t *head, *tailp;

	/* Attributes */
	int attributes;

	/* Length */
	int length;

} list_t;

/* List Definitions */
#define LIST_NORMAL		0x0
#define LIST_SAFE		0x1

/* Foreach Macro */
#define _foreach(i, list) for (i = list->head; i != NULL; i = i->link)
#define foreach(i, list) list_node_t *i; for (i = list->head; i != NULL; i = i->link)


/* List Prototypes */
EXTERN list_t *list_create(int attributes);
EXTERN void list_destroy(list_t *list);
EXTERN int list_length(list_t *list);

EXTERN list_node_t *list_create_node(int id, void *data);

EXTERN void list_insert(list_t *list, list_node_t *node, int position);
EXTERN void list_insert_front(list_t *list, list_node_t *node);
EXTERN void list_append(list_t *list, list_node_t *node);

EXTERN list_node_t *list_pop_front(list_t *list);
EXTERN list_node_t *list_pop_back(list_t *list);

EXTERN int list_get_index_by_data(list_t *list, void *data);
EXTERN int list_get_index_by_id(list_t *list, int id);
EXTERN int list_get_index_by_node(list_t *list, list_node_t *node);

EXTERN list_node_t *list_get_node_by_id(list_t *list, int id, int n);
EXTERN void *list_get_data_by_id(list_t *list, int id, int n);

EXTERN void ListExecuteOnId(list_t *List, void(*Function)(void*, int, void*), int Id, void *UserData);
EXTERN void list_execute_all(list_t *list, void(*func)(void*, int));

EXTERN void list_remove_by_node(list_t *list, list_node_t* node);
EXTERN void list_remove_by_index(list_t *list, int index);
EXTERN void list_remove_by_id(list_t *list, int id);

#endif // !_LIST_H_