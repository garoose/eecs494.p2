#include <zenilib.h>

#include "Endpoint.h"
#include "Buggy.h"

void Endpoint::collide_with_buggy(Buggy *b) {
	map->end_level(b->get_score());
}

void Endpoint::collide_with_tire(Tire *t) {
	map->end_level(t->get_score());
}