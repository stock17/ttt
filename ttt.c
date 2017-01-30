#include <stdio.h>
#include <SDL2/SDL.h>

int main (void)
{
    SDL_Init (SDL_INIT_VIDEO);
    SDL_Window *win = SDL_CreateWindow("Tic-tac-toe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    
    
    while(1)
    {
        SDL_Event e;
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                break;
        }
        
        SDL_RenderClear(rend);
        SDL_RenderPresent(rend);
    }
    
    
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    
}
