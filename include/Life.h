#pragma once
#include <sdliv.h>

//forward declare cross reference
namespace sdliv { class App; };

class Life
{
public:
	static SDL_Surface *surfaceBuffer;
	static SDL_Surface *currentSurface;
	static Uint32 eventType;
	static sdliv::App *app;

public:
	static void Init(sdliv::App *);
	static void Update(void);
	static void Cleanup(void);
	
};

