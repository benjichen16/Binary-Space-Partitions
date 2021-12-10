#include <stdbool.h>
//----------------------------------------------------
// List.h
// Header file for List ADT
// ---------------------------------------------------
typedef struct ListObj* ListPtr;

// Constructors-Destructors --------------------------

/**
 * @brief Creates a new list
 * 
 * @param dataCompare Function used to order 2 elements. Returns > 0 if obj1 is greater, < 0 if obj2 is greater, and 0 if objects are equal
 * @param dataPrinter Function to use to print out a piece of data in the list
 * @param freeData Function used to free data from a ListObj
 * @return List The newly created list.
 */
ListPtr list_create(int (*dataCompare)(void *obj1, void*obj2), void (*dataPrinter)(void *data), void (*freeData)(void *data));

/**
 * @brief Frees the space taken up by the list
 * 
 * @param pL       A pointer to the list to deallocate. The pointer should be set to NULL
 * @param freeData If to also free the data contained within the list
 */
void list_destroy(ListPtr pL, bool freeData);

// Access functions ----------------------------------

/**
 * @brief Gets the length of a list
 * 
 * @param L The list for which the length should be returned.
 * @return int The length of the list, or -1 if error
 */
int list_length( ListPtr L );

/**
 * @brief Returns the index of the search data in the list
 * 
 * @param L The list to check
 * @param comp The data to search for
 * @return int The index of the data in the list, or -1 if not in list
 */
int list_find_element( ListPtr L, void *comp );

/**
 * @brief Returns the index of the search data in the list
 * 
 * @param i The index to fetch
 * @return int The data at the index, or NULL if out of range
 */
void *list_get_index( ListPtr L, int i);


// Manipulation functions ----------------------------

/**
 * @brief Deletes the entry at the specified index and returns the data from the entry.
 * 
 * @param L The list to delete the entry from.
 * @param i The index of the entry to remove
 * @return void* The data that was stored in that entry. Returns NULL if the entry does not exist.
 */
void *list_del_index( ListPtr L, int i );

/**
 * @brief Appends an entry to the list.
 * 
 * @param L    The list to append to
 * @param data The data to insert into the new entry.
 * @return true  If the append was succesfull
 * @return false If the entry could not be appended
 */
bool list_append( ListPtr L, void *data );

/**
 * @brief Inserts an entry into the list.
 * 
 * @param L      The list to insert the entry into
 * @param pos    The position into which to insert the entry
 * @param data   The data to insert into the entry
 * @return true  If the insert was succesfull
 * @return false If the insert could not be performed
 */
bool list_insert(ListPtr L, int pos, void *data);

// Other functions ----------------------------
/**
 * @brief Prints the data in the list in the format "%s\n" for each entry
 * 
 * @param L           The list to print
 * @param dataPrinter The function to use to print an element
 */
void list_print(ListPtr L);

/**
 * @brief Sorts a list according to dataCompare passed in the list constructor.
 * 
 * @param L The list to sort
 */
void list_sort(ListPtr L, bool ascending);
