#pragma once

#include <Windows.h>

// note, this class should probably take responsibility for the colliding code too, hint hint. 

class Collidable
{
public:
	virtual void hasCollided() { OutputDebugStringA("collidable hasCollided - derive your own version of this method\n"); }

};

