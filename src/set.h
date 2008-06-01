
/*

Copyright (c) 2005, Simon Howard
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions 
are met:

 * Redistributions of source code must retain the above copyright 
   notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright 
   notice, this list of conditions and the following disclaimer in 
   the documentation and/or other materials provided with the 
   distribution.
 * Neither the name of the C Algorithms project nor the names of its 
   contributors may be used to endorse or promote products derived 
   from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE.

*/

/**
 * @file set.h
 *
 * @brief Set of values.
 *
 * A set stores a collection of values.  Each value can only exist once in 
 * the set.
 *
 * To create a new set, use @ref set_new.  To destroy a set, use 
 * @ref set_free.
 *
 * To add a value to a set, use @ref set_insert.  To remove a value
 * from a set, use @ref set_remove.
 *
 * To find the number of entries in a set, use @ref set_num_entries.
 *
 * To query if a particular value is in a set, use @ref set_query.
 *
 * To iterate over all values in a set, use @ref set_iterate to initialise
 * a @ref SetIterator structure, with @ref set_iter_next and 
 * @ref set_iter_has_more to read each value in turn.
 *
 * Two sets can be combined (union) using @ref set_union, while the
 * intersection of two sets can be generated using @ref set_intersection.
 */

#ifndef ALGORITHM_SET_H
#define ALGORITHM_SET_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Represents a set of values.  Created using the @ref set_new function and
 * destroyed using the @ref set_free function.
 */

typedef struct _Set Set;

/**
 * An object used to iterate over a set.
 *
 * @see set_iterate
 */

typedef struct _SetIterator SetIterator;

/**
 * Internal structure representing an entry in the set.
 */

typedef struct _SetEntry SetEntry;

/**
 * A value stored in a @ref Set.
 */

typedef void *SetValue;

/**
 * Definition of a @ref SetIterator.
 */

struct _SetIterator {
	Set *set;
	SetEntry *next_entry;
	int next_chain;
};

/**
 * A null @ref SetValue.
 */

#define SET_NULL ((void *) 0)

/** 
 * Hash function.  Generates a hash key for values to be stored in a set.
 */

typedef unsigned long (*SetHashFunc)(SetValue value);

/** 
 * Equality function.  Compares two values to determine if they are
 * equivalent.
 */

typedef int (*SetEqualFunc)(SetValue value1, SetValue value2);

/**
 * Function used to free values stored in a set.  See
 * @ref set_register_free_function.
 */

typedef void (*SetFreeFunc)(SetValue value);

/**
 * Create a new set.
 *
 * @param hash_func     Hash function used on values in the set.
 * @param equal_func    Compares two values in the set to determine
 *                      if they are equal.
 * @return              A new set, or NULL if it was not possible to 
 *                      allocate the memory for the set.
 */

Set *set_new(SetHashFunc hash_func, SetEqualFunc equal_func);

/**
 * Destroy a set.
 *
 * @param set           The set to destroy.
 */

void set_free(Set *set);

/**
 * Register a function to be called when values are removed from 
 * the set.
 *
 * @param set           The set.
 * @param free_func     Function to call when values are removed from the
 *                      set.
 */

void set_register_free_function(Set *set, SetFreeFunc free_func);

/**
 * Add a value to a set.
 *
 * @param set           The set.
 * @param data          The value to add to the set.
 * @return              Non-zero (true) if the value was added to the set,
 *                      zero (false) if it already exists in the set, or 
 *                      if it was not possible to allocate memory for the
 *                      new entry.
 */

int set_insert(Set *set, SetValue data);

/**
 * Remove a value from a set.
 *
 * @param set           The set.
 * @param data          The value to remove from the set.
 * @return              Non-zero (true) if the value was found and removed
 *                      from the set, zero (false) if the value was not
 *                      found in the set.
 */

int set_remove(Set *set, SetValue data);

/** 
 * Query if a particular value is in a set.
 *
 * @param set           The set.
 * @param data          The value to query for.
 * @return              Zero if the value is not in the set, non-zero if the
 *                      value is in the set.
 */

int set_query(Set *set, SetValue data);

/**
 * Retrieve the number of entries in a set
 *
 * @param set           The set.
 * @return              A count of the number of entries in the set.
 */

int set_num_entries(Set *set);

/**
 * Create an array containing all entries in a set.
 *
 * @param set              The set.
 * @return                 An array containing all entries in the set,
 *                         or NULL if it was not possible to allocate
 *                         memory for the array.
 */

SetValue *set_to_array(Set *set);

/**
 * Perform a union of two sets.  
 *
 * @param set1             The first set.
 * @param set2             The second set.
 * @return                 A new set containing all values which are in the 
 *                         first or second sets, or NULL if it was not
 *                         possible to allocate memory for the new set.
 */

Set *set_union(Set *set1, Set *set2);

/**
 * Perform an intersection of two sets.
 *
 * @param set1             The first set.
 * @param set2             The second set.
 * @return                 A new set containing all values which are in both
 *                         set, or NULL if it was not possible to allocate
 *                         memory for the new set.
 */

Set *set_intersection(Set *set1, Set *set2); 

/**
 * Initialise a @ref SetIterator structure to iterate over the values
 * in a set.
 *
 * @param set              The set to iterate over.
 * @param iter             Pointer to an iterator structure to initialise.
 */

void set_iterate(Set *set, SetIterator *iter);

/**
 * Determine if there are more values in the set to iterate over.
 * It should be noted that a set iterator is not freed when iterating
 * has finished.  This should be done using @ref set_iter_free.
 *
 * @param iterator         The set iterator object.
 * @return                 Zero if there are no more values in the set
 *                         to iterate over, non-zero if there are more
 *                         values to be read.
 */

int set_iter_has_more(SetIterator *iterator);

/**
 * Using a set iterator, retrieve the next value from the set.
 *
 * @param iterator         The set iterator.
 * @return                 The next value from the set, or @ref SET_NULL if no
 *                         more values are available.
 */

SetValue set_iter_next(SetIterator *iterator);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef ALGORITHM_SET_H */

