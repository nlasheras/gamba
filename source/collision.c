#include "collision.h"
#include "entities.h"

void collision_init(Collider* c, int x, int y, int w, int h)
{
	c->enabled = true;
	c->x = x;
	c->y = y;
	c->w = w;
	c->h = h;
}

bool collision_test(const Entity* e1, const Entity* e2)
{
	if (e1->collider.enabled && e2->collider.enabled)
	{
		const int bl = e1->x + e1->collider.x;
		const int bt = e1->y + e1->collider.y;
		const int br = bl + e1->collider.w;
		const int bb = bt + e1->collider.h;

		const int el = e2->x + e2->collider.x;
		const int et = e2->y + e2->collider.y;
		const int er = el + e2->collider.w;
		const int eb = et + e2->collider.h;

		return (bl >= el && bl < er && bt >= et && bt < eb) ||
			(br >= el && br < er && bb >= et && bb < eb) ||
			(br >= el && br < er && bt >= et && bt < eb) ||
			(bl >= el && bl < er && bb >= et && bb < eb);
	}
	return false;
}
