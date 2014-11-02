#include "Actor.h"

namespace particleNS{
	const float LIFETIME = 1;
}
class Particle:public Actor
{
private:
	float age;
public:
	Particle();
	virtual void update(float frametime);
	void create(D3DXVECTOR2 loc,COLOR_ARGB c);
	virtual void draw(VECTOR2 screenLoc);
};