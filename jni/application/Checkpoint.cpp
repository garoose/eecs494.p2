#include <zenilib.h>

#include "Checkpoint.h"
#include "Buggy.h"

void Checkpoint::collide_with_buggy(Buggy *b) {
	col = get_Colors()["green"];
	b->checkpoint(get_position());
	map->checkpoint(get_position());
}

void Checkpoint::collide_with_tire(Tire *t) {
	col = get_Colors()["green"];
	t->checkpoint(get_position());
	map->checkpoint(get_position());
}