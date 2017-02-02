#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

#define GRIDSIZE 100
#define RES_X 640
#define RES_Y 480


struct Grid {
    SDL_Point center;
    int cellsize;
    int cellsign[3][3];
    SDL_Point cellcenter[3][3];
};

// void DrawGrid (SDL_Renderer* r, int x, int y, int size);  // V.1
void DrawGrid (SDL_Renderer* r, struct Grid g);              // V.2
void DrawCircle (SDL_Renderer* r, int cx, int cy, int radius);
void DrawCross (SDL_Renderer* r, int cx, int cy, int size);



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

    mygrid.center.x = RES_X / 2;
    mygrid.center.y = RES_Y / 2;
    mygrid.cellsize = GRIDSIZE;
    int init_x = mygrid.center.x - mygrid.cellsize;
    int init_y = mygrid.center.y - mygrid.cellsize;
    
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
        init_x = mygrid.center.x - mygrid.cellsize;
        init_y += mygrid.cellsize;
    }
    //===================================================
    

    // Draw grid 
    //DrawGrid (rend, (RES_X - 3 * GRIDSIZE) / 2, (RES_Y - 3 * GRIDSIZE) / 2, GRIDSIZE);
    //V.2

        // Testing...
    mygrid.cellsign[0][0] = 1;    
        mygrid.cellsign[1][1] = 2;
            mygrid.cellsign[2][2] = 1;
    
    
    DrawGrid (rend, mygrid);
    SDL_RenderPresent(rend);
    
    
    
    /* Draw = testing 
    DrawCircle(rend, 320, 240, 30); 
    DrawCross(rend, 220, 240, 30);
    SDL_RenderPresent(rend);
    
    DrawCross(rend, 420, 240, 30);
    SDL_RenderPresent(rend);
    ********************************/
            
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
            SDL_GetMouseState(&mx, &my);
           
         // Check if mouse coordinates in any cell 
                       
             for (int i = 0; i < 3; i++)
             {
                 for (int j = 0; j < 3; j++)
                 {    
                     if 
                     (
                         (mx > mygrid.cellcenter[i][j].x - mygrid.cellsize/2 && my > mygrid.cellcenter[i][j].y- mygrid.cellsize/2) &&
                         (mx < mygrid.cellcenter[i][j].x + mygrid.cellsize/2 && my < mygrid.cellcenter[i][j].y + mygrid.cellsize/2)
                     )
                     {
                         if (e.button.button == SDL_BUTTON_LEFT)
                             mygrid.cellsign[i][j] = 1;
                         else
                             mygrid.cellsign[i][j] = 2;
                     }  
              
                        
                          
                 }
              }

            SDL_RenderClear(rend);
            DrawGrid(rend, mygrid);
            SDL_RenderPresent(rend);
            
         }
        
    }
    
    
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    
}

/* Function to draw the game grid
Args: render, coorinates of left up corner, size of cell side
void DrawGrid (SDL_Renderer* r, int x, int y, int size)
{
    //for (int i = 0; i < 4 * size; i += size) // with border
    for (int i = size; i < 3 * size; i += size) // without border
    {
        SDL_RenderDrawLine(r, x, y+i, x+3*size, y+i);
        SDL_RenderDrawLine(r, x+i, y, x+i, y+3*size);           
    }
}*/


// Function to draw the game grid V.2
// Args: render, grid
void DrawGrid (SDL_Renderer* r, struct Grid g)
{
    int x = g.center.x - g.cellsize * 1.5;
    int y = g.center.y - g.cellsize * 1.5;
    int step = g.cellsize;
   
    for (int i = 0; i < 4 * step; i += step) // with border
    //for (int i = step; i < 3 * step; i += step) // without border
    {
        SDL_RenderDrawLine(r, x, y+i, x+3*step, y+i);
        SDL_RenderDrawLine(r, x+i, y, x+i, y+3*step);           
    }
    
    
    //Draw signs
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {    
            if (g.cellsign[i][j] == 1)
                DrawCross(r, g.cellcenter[i][j].x, g.cellcenter[i][j].y, g.cellsize / 3);
            else if (g.cellsign[i][j] == 2)
                DrawCircle(r, g.cellcenter[i][j].x, g.cellcenter[i][j].y, g.cellsize / 3);
           
           
            SDL_RenderDrawPoint(r, g.cellcenter[i][j].x, g.cellcenter[i][j].y);
        }

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
