#include "Asteroid.h"
#include "Buggy.h"

void Asteroid::collide_with_buggy(Buggy *b) {
	explode();
	b->explode();
}