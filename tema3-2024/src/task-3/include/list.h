#include <stdint.h>

/// @brief Linked list node.
typedef struct list_node_t {
  /// @brief Pointer to the data stored in the node.
  void *elem;

  /*
   * @brief Pointer to the next node in the list.
   * @note Is NULL if it's the last node of the list.
  */
  struct list_node_t *next;
} list_node_t;

/// @brief Linked list structure.
typedef struct list_t {
  /// @brief The number of elements in the list.
  uint32_t size;
  /// @brief The size (in bytes) of the elements.
  uint32_t type_size;
  
  /*
   * @brief Pointer to the head of the list.
   * @note Is NULL if the list is empty.
  */
  struct list_node_t *head;
} list_t;

typedef list_node_t *LNode;
typedef list_t *List;

/*
 * @brief Creates an empty list.
 * @param type_size the size (in bytes) of the elements in the list.
*/
List list_create(uint32_t type_size);

/*
 * @brief Inserts an element in a linked list.
 * @param l Pointer to the list.
 * @param elem Pointer to the element to insert.
 * @note It makes a deep copy of the element.
*/
void list_insert(List l, void *elem);

/*
 * @brief Inserts an element in a linked list by making a shallow copy.
 * @param l Pointer to the list.
 * @param elem Pointer to the element to insert.
*/
void list_insert_shallow(List l, void *elem);

/*
 * @brief Frees a linked list.
 * @note `free` is called on the elements of the list.
*/ 
void list_free(List l);

/*
 * @brief Empties a linked list.
 * @note `free` is called on the elements of the list.
 */
void list_clear(List l);
