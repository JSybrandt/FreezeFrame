#include "Actor.h"
class Particle:public Actor
{
private:
	float currentOpacity;
	float dissipationRate;
public:
	Particle();
	virtual void update(float frametime);
	void set(D3DXVECTOR2 loc);
	virtual void draw(VECTOR2 screenLoc);
};