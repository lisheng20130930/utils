#include "Idmap.h"
#include "libos.h"

#define IDMAP_DEFAULT_SIZE 128

/*
 * This algorithm (k=33) was first reported by dan bernstein many years ago
 * in comp.lang.c. Another version of this algorithm (now favored by bernstein)
 * uses xor: hash(i) = hash(i - 1) * 33 ^ str[i]; the magic of number 33
 * (why it works better than many other constants, prime or not)
 * has never been adequately explained.
 *
 * Source: http://www.cse.yorku.ca/~oz/hash.html
 */
static unsigned int hash_function(const char *str, int size)
{
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash % size;
}

/*
 * This algorithm was created for sdbm (a public-domain reimplementation of ndbm)
 * database library. It was found to do well in scrambling bits,
 * causing better distribution of the keys and fewer splits. It also happens
 * to be a good general hashing function with good distribution.
 * The actual function is hash(i) = hash(i - 1) * 65599 + str[i];
 * what is included below is the faster version used in gawk.
 * [there is even a faster, duff-device version] the magic constant 65599
 * was picked out of thin air while experimenting with different constants,
 * and turns out to be a prime. this is one of the algorithms used in
 * berkeley db (see sleepycat) and elsewhere.
 *
 * Source: http://www.cse.yorku.ca/~oz/hash.html
 */
/*
static unsigned int hash_function(const char *str, int size)
{
	unsigned long hash = 0;
	int c;

	while (c = *str++)
		hash = c + (hash << 6) + (hash << 16) - hash;

	return hash % size;
}
*/

/*
 * Free memory allocated by item.
 */
static void free_item(struct Idmap_item *p)
{
	free(p->key);
	free(p);
}

/*
 * Create new Idmap.
 * Size is the size of the number of map slots. Leave it 0
 * if you want to use a default value.
 */
struct Idmap *Idmap_create(unsigned int size)
{
	struct Idmap *map;

	if (size == 0)
			size = IDMAP_DEFAULT_SIZE;

	map = (struct Idmap *)malloc(sizeof(struct Idmap));
	if (map == NULL)
		return NULL;

	int table_size = sizeof(struct Idmap_item *) * size;

	map->size = size;
	map->table = (struct Idmap_item **)malloc(table_size);
	if (map->table == NULL) {
		free(map);
		return NULL;
	}
	memset(map->table, 0x00, table_size);

	return map;
}

/*
 * Destroy Idmap.
 */
void Idmap_free(struct Idmap *map)
{
	struct Idmap_item *p, *q;
	int i;

	for (i = 0; i < map->size; i++) {
		p = map->table[i];

		while (p) {
			q = p;
			p = p->next;
			free_item(q);
		}
	}

	free(map->table);
	free(map);
}

/*
 * Add value to the  map.
 * If value assigned to the given key already exist it will be replaced with
 * new value.
 */
void Idmap_put(struct Idmap *map, const char *key, void *value)
{
	unsigned int index;
	struct Idmap_item *new_item;

	if (key == NULL)
		return;

	new_item = (struct Idmap_item *)malloc(sizeof(struct Idmap_item));
	new_item->key = cmmn_strdup(key);
	new_item->value = value;
	new_item->next = NULL;

	index = hash_function(key, map->size);

	if (map->table[index] == NULL) {
		map->table[index] = new_item;
	} else {
		struct Idmap_item *p = map->table[index];

		do {
			if (strcmp(p->key, key) == 0) {
				p->value = value;
				return;
			}
		} while(p->next && (p = p->next));

		p->next = new_item;
	}
}

/*
 * Returns value from map by its key, or NULL if value assigned to this key
 * not found.
 */
void *Idmap_get(struct Idmap *map, const char *key)
{
	unsigned int index;
	struct Idmap_item *p;

	if (key == NULL)
		return NULL;

	index = hash_function(key, map->size);
	p = map->table[index];

	// Value was not setted.
	if (p == NULL)
		return NULL;

	do {
		if (strcmp(p->key, key) == 0)
			return p->value;
	} while ((p = p->next) != NULL);

	return NULL;
}

/*
 * Remove key assigned value from the map.
 * Returns value assigned for the deleted key.
 */
void *Idmap_remove(struct Idmap *map, const char *key)
{
	if (key == NULL)
		return NULL;

	unsigned int index = hash_function(key, map->size);
	struct Idmap_item *p, *q;
	void *val;

	p = map->table[index];	// Current item.
	q = p;					// Previous item. We need it for deletion.

	while (p) {
		if (strcmp(p->key, key) == 0) {
			val = p->value;

			if (p == q) { // If p is the first item in the list.
				map->table[index] = p->next;
			} else { // p item at the middle or at the end of the list.
				q->next = p->next;
			}

			free_item(p);

			return val;
		}

		q = p;
		p = p->next;
	}

	return NULL;
}

/*
 * Returns number of elements presented in the map.
 */
int Idmap_size(struct Idmap *map)
{
	int i;
	int size = 0;
	struct Idmap_item *p;

	for (i = 0; i < map->size; i++) {
		p = map->table[i];

		while (p) {
			size++;
			p = p->next;
		}
	}

	return size;
}

/*
 * Returns iterator for the given map.
 */
Idmap_iterator *hashmap_get_iterator(struct Idmap *map)
{
	static Idmap_iterator g_stIterator;
	if (!map){
		return NULL;
	}

	Idmap_iterator *iterator = &g_stIterator;
	if (!iterator){
		return NULL;
	}

	iterator->map = map;
	iterator->index = 0;
	iterator->item = NULL;

	return iterator;
}

/*
 * Returns true if we have at least one item for next iteration.
 */
int hashmap_has_next(Idmap_iterator *iterator)
{
	Idmap_item *item = NULL;
	int index = iterator->index;

	if(iterator->item){
		if(iterator->item->next){
			return 1;
		}
		index++;
	}

	/* Find next slot with items and returns the first one. */
	for(; index < iterator->map->size; index++){
		if(iterator->map->table[index] && iterator->map->table[index]->key){
			return 1;
		}
	}

	return item != NULL;
}

/*
 * Returns next item in the given iteration.
 */
void *hashmap_get_next(Idmap_iterator *iterator)
{
	if(iterator->item){
		if (iterator->item->next) {
			iterator->item = iterator->item->next;
			return iterator->item->value;
		}
		iterator->item = NULL;
		iterator->index++;
	}

	/* Find next slot with items and returns the first one. */
	for(;iterator->index<iterator->map->size; iterator->index++){
		if(iterator->map->table[iterator->index]){
			iterator->item = iterator->map->table[iterator->index];
			return iterator->item->value;
		}
	}

	return NULL;
}

void Idmap_removeAll(struct Idmap *map)
{
	int i;
	for (i = 0; i < map->size; i++) {
		struct Idmap_item *p = map->table[i];		
		while(p){
			struct Idmap_item *q = p;
			p = p->next;
			free_item(q);
		}
		map->table[i]=NULL;
	}
}