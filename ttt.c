#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

#define GRIDSIZE 100
#define RES_X 640
#define RES_Y 480

void DrawGrid (SDL_Renderer* r, int x, int y, int size);
void DrawCircle (SDL_Renderer* r, int cx, int cy, int radius);
void DrawCross (SDL_Renderer* r, int cx, int cy, int size);

struct Grid {
    SDL_Point center;
    int cellsize;
    int cellsign[3][3];
    SDL_Point cellcenter[3][3];
};

int main (void)
{
    SDL_Window *win;
    SDL_Renderer *rend;
    SDL_Event e;
    
    SDL_Init (SDL_INIT_VIDEO);
    win = SDL_CreateWindow("Tic-tac-toe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, RES_X, RES_Y, 0);
    rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    
    SDL_RenderClear(rend);
    SDL_RenderPresent(rend);
    
    //=================== Init grid =====================
    struct Grid mygrid;
    //mygrid.center = {(RES_X / 2), (RES_Y / 2)};
    mygrid.center.x = RES_X / 2;
    mygrid.center.y = RES_Y / 2;
    mygrid.cellsize = GRIDSIZE;
    int init_x = mygrid.center.x - mygrid.cellsize / 2;
    int init_y = mygrid.center.y - mygrid.cellsize / 2;
    
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {    
            // Move sings of cells to 0; cross - 1, zero - 2
            mygrid.cellsign[i][j] = 0; 

            // Set center coordinates of cells
            mygrid.cellcenter[i][j].x = init_x;
            mygrid.cellcenter[i][j].y = init_y;
            init_x += mygrid.cellsize;            
        }
        init_x = mygrid.center.x - mygrid.cellsize / 2;
        init_y += mygrid.cellsize;
    }
    //===================================================
    

    // Draw grid 
    DrawGrid (rend, (RES_X - 3 * GRIDSIZE) / 2, (RES_Y - 3 * GRIDSIZE) / 2, GRIDSIZE);
    
    // Draw = testing //*************
    DrawCircle(rend, 320, 240, 30); 
    DrawCross(rend, 220, 240, 30);
    SDL_RenderPresent(rend);
    
    DrawCross(rend, 420, 240, 30);
    SDL_RenderPresent(rend);
    //*******************************
            
    while(1)
    {
       
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                break;
        }
        
        //Catch the mouse
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            int mx, my;
            int mousestate = SDL_GetMouseState(&mx, &my);
            
            //if (SDL_BUTTON(SDL_BUTTON_RIGHT))
            if (e.button.button == SDL_BUTTON_LEFT)
                DrawCross(rend, mx, my, 30);
            else
                DrawCircle(rend, mx, my, 30);

            SDL_RenderPresent(rend);
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

// Function to draw circle
// Args: renderer, center coords, radius
void DrawCircle (SDL_Renderer* r, int cx, int cy, int radius)
{
    for (float rad = 0; rad < 6.2; rad += 0.02)
    {      
        SDL_RenderDrawPoint(r, cx+cos(rad)*radius, cy+sin(rad)*radius);    
    }   
}

// Function to draw cross
// Args: renderer, center coords, size of square side / 2
void DrawCross (SDL_Renderer* r, int cx, int cy, int size)
{
      SDL_RenderDrawLine(r, cx-size, cy-size, cx+size, cy+size);
      SDL_RenderDrawLine(r, cx-size, cy+size, cx+size, cy-size);
}
