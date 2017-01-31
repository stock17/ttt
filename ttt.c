#include <stdio.h>
#include <SDL2/SDL.h>

#define GRIDSIZE 200
#define RES_X 1024
#define RES_Y 768

void DrawGrid (SDL_Renderer* r, int x, int y, int size);


int main (void)
{
    SDL_Window *win;
    SDL_Renderer *rend;
    
    SDL_Init (SDL_INIT_VIDEO);
    win = SDL_CreateWindow("Tic-tac-toe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, RES_X, RES_Y, 0);
    rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    
    SDL_RenderClear(rend);
    SDL_RenderPresent(rend);
        
    // Draw grid 
    DrawGrid (rend, (RES_X - 3 * GRIDSIZE) / 2, (RES_Y - 3 * GRIDSIZE) / 2, GRIDSIZE);
    SDL_RenderPresent(rend);
        
    while(1)
    {
        SDL_Event e;
        
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                break;
        }
        
    }
    
    
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    
}

// Function to draw the game grid
// Args: render, coorinates of left up corner, size of cell side

void DrawGrid (SDL_Renderer* r, int x, int y, int size)
{
    //for (int i = 0; i < 4 * size; i += size) // with border
    for (int i = size; i < 3 * size; i += size) // without border
    {
        SDL_RenderDrawLine(r, x, y+i, x+3*size, y+i);
        SDL_RenderDrawLine(r, x+i, y, x+i, y+3*size);           
    }
}
