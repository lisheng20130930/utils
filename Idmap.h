#ifndef IDMAP_H
#define IDMAP_H

typedef struct Idmap_item {
	char *key;
	void *value;
	struct Idmap_item *next;
}Idmap_item;

typedef struct Idmap {
	// Size of the table in slots.
	int size;
	// Pointer to the first slot.
	struct Idmap_item **table;
}Idmap;

typedef struct Idmap_iterator {
	struct Idmap *map;
	/* Index of active slot. */
	int index;
	/* Pointer to current item. */
	struct Idmap_item *item;
}Idmap_iterator;

struct Idmap *Idmap_create(unsigned int size);
void Idmap_free(struct Idmap *map);
/* If override, I will return the old value */
void* Idmap_put(struct Idmap *map, const char *key, void *value);
void *Idmap_get(struct Idmap *map, const char *key);
void *Idmap_remove(struct Idmap *map, const char *key);
int Idmap_size(struct Idmap *map);
Idmap_iterator *hashmap_get_iterator(struct Idmap *map);
int hashmap_has_next(Idmap_iterator *iterator);
void *hashmap_get_next(Idmap_iterator *iterator);
void Idmap_removeAll(struct Idmap *map);

#endif /* Idmap_H_ */
