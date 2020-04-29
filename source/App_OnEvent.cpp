#include <sdliv.h>



void sdliv::App::OnEvent(SDL_Event * e)
{
	SDL_assert(e != nullptr);

	switch (e->type)
	{
		case SDL_WINDOWEVENT:
			window->centerElement(active_element);
			OnRender();
			break;
		case SDL_QUIT:
			Running = false;
#ifdef LIFE
			Life::Cleanup();
#endif
			break;
#ifdef LIFE
		case SDL_USEREVENT:
			if (e->user.type == Life::eventType) {
				Life::Update();
			}
#endif
		default:
			break;
	}
}
