#include <zenilib.h>

#include "Checkpoint.h"
#include "Buggy.h"

void Checkpoint::collide_with_buggy(Buggy *b) {
	b->checkpoint();
	map->checkpoint(get_position());
}

void Checkpoint::collide_with_tire(Tire *t) {
	t->checkpoint();
	map->checkpoint(get_position());
}