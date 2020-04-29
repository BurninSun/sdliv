#include <sdliv.h>
#include <life.h>
#include <iostream>

SDL_Surface *Life::surfaceBuffer = nullptr;
SDL_Surface *Life::currentSurface = nullptr;
Uint32 Life::eventType;
sdliv::App *Life::app = nullptr;

void Life::Init(sdliv::App *app)
{
	Life::app = app;
	currentSurface = app->GetActiveElement()->GetSurface();
	surfaceBuffer = SDL_CreateRGBSurface
	(
		0,
		currentSurface->w,
		currentSurface->h,
		currentSurface->format->BitsPerPixel,
		currentSurface->format->Rmask,
		currentSurface->format->Gmask,
		currentSurface->format->Bmask,
		currentSurface->format->Amask
	);
	surfaceBuffer = SDL_ConvertSurface(surfaceBuffer, currentSurface->format, 0);

	SDL_BlitSurface(currentSurface, nullptr, surfaceBuffer, nullptr);

	eventType = SDL_RegisterEvents(1);
	SDL_AddTimer(500, [](Uint32 t, void *param)->Uint32 
	{
		SDL_Event e;
		SDL_UserEvent ue;
		ue.type = eventType;
		e.type = SDL_USEREVENT;
		e.user = ue;
		SDL_PushEvent(&e); 
		return t;
	},
		nullptr);

	int bytesPerPixel = surfaceBuffer->format->BytesPerPixel;
	SDL_LockSurface(surfaceBuffer);
	for (int y = 0; y < surfaceBuffer->h; y++)
	{
		for (int x = 0; x < surfaceBuffer->w; x++)
		{
			if (rand() > RAND_MAX / 4) {
				for (int p = 0; p < bytesPerPixel; p++)
				{
					((char*)(surfaceBuffer->pixels))[y*surfaceBuffer->pitch + x * bytesPerPixel + p] = 0x0;
				}
			}
		}
	}
	SDL_UnlockSurface(surfaceBuffer);
}

void Life::Update(void)
{
	//we're always behind one frame, rendering the previously calculated frame
	SDL_BlitSurface(surfaceBuffer, nullptr, currentSurface, nullptr);
	app->GetActiveElement()->createFromSurface(currentSurface);

	SDL_FillRect(surfaceBuffer, nullptr, 0x00);


	int bytesPerPixel = currentSurface->format->BytesPerPixel;
	int bitsPerPixel = currentSurface->format->BitsPerPixel;
	int rmask = currentSurface->format->Rmask;
	int gmask = currentSurface->format->Gmask;
	int bmask = currentSurface->format->Bmask;
	int neighbors;
	Uint32 color;

	SDL_LockSurface(surfaceBuffer);
	SDL_LockSurface(currentSurface);
	for (int y = 0; y < currentSurface->h; y++)
	{
		for (int x = 0; x < currentSurface->w; x++)
		{
			neighbors = 0;
			for (int dx = -1; dx <= 1; dx++)
			{
				for (int dy = -1; dy <= 1; dy++)
				{
					if (!(dx == dy && dx == 0))
					{
						color = 0;
						for (int p = 0; p < bytesPerPixel; p++)
						{
							color += ((char *)(currentSurface->pixels))[((y + dy) % currentSurface->h)*currentSurface->pitch + ((x + dx) % currentSurface->w) * bytesPerPixel + p] << (8 * p);
						}
						neighbors += (color != 0);
					}
					if (neighbors > 3)
					{
						break;
					}
				}
				if (neighbors > 3)
				{
					break;
				}
			}
			if (neighbors > 3 || neighbors < 2)
			{
				for (int p = 0; p < bytesPerPixel; p++)
				{
					((char *)(surfaceBuffer->pixels))[y*surfaceBuffer->pitch + x * bytesPerPixel + p] = 0x00;
				}
			}
			else if (neighbors == 2)
			{
				color = 0;
				for (int p = 0; p < bytesPerPixel; p++)
				{
					color += ((char *)(currentSurface->pixels))[y*currentSurface->pitch + x * bytesPerPixel + p] << (8 * p);
				}
				if (color != 0)
				{
					for (int p = 0; p < bytesPerPixel; p++)
					{
						((char *)(surfaceBuffer->pixels))[y*surfaceBuffer->pitch + x * bytesPerPixel + p] = ((char *)(currentSurface->pixels))[y*currentSurface->pitch + x * bytesPerPixel + p];
					}
				}
			}
			else if (neighbors == 3)
			{
				for (int p = 0; p < bytesPerPixel; p++)
				{
					((char *)(surfaceBuffer->pixels))[y*surfaceBuffer->pitch + x * bytesPerPixel + p] = 0xFF;
				}
			}
			else
			{
				sdliv::log("wtf?");
			}
		}
	}

	SDL_UnlockSurface(currentSurface);
	SDL_UnlockSurface(surfaceBuffer);


	app->OnRender();
}

void Life::Cleanup(void)
{ 
	SDL_FreeSurface(surfaceBuffer);
}