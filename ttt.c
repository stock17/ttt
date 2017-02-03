#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
int  CheckWin(struct Grid g, int winner);



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
            // Set signs of cells to 0; cross - 1, zero - 2
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
    
    DrawGrid (rend, mygrid);
    SDL_RenderPresent(rend);
    int player_move = 1;
    
//===================================================
//================ Main loop  =======================
//===================================================
    while(1)
    {

    // ---------- Checking win --------------
        if (CheckWin (mygrid, 1)) {
            printf("Cross win\n");
            break;
        }
        else if (CheckWin (mygrid,2)) {
            printf("Zero win\n");
            break;
        }

    // ----------- Check quit ----------------        

        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                break;
        }
        
    // ----------- Computer move --------------
   
        if (!player_move)
        {
            int n, i, j;
        //Random for testing...
            do {
                srand(time(NULL));
                n = rand()%9;
                j = n % 3;
                i = n / 3;
                
            } while (mygrid.cellsign[i][j] != 0);
            
            mygrid.cellsign [i][j] = 2;
            
            SDL_RenderClear(rend);
            DrawGrid(rend, mygrid);
            SDL_RenderPresent(rend);
            
            player_move = 1;
                    
        }
        
    // ------------ Player move ---------------
        else 
        {
            SDL_WaitEvent(&e);

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
                             (mx > mygrid.cellcenter[i][j].x - mygrid.cellsize/2 && my > mygrid.cellcenter[i][j].y - mygrid.cellsize/2) &&
                             (mx < mygrid.cellcenter[i][j].x + mygrid.cellsize/2 && my < mygrid.cellcenter[i][j].y + mygrid.cellsize/2)
                         )
                         // then set sign
                         {
                             if (e.button.button == SDL_BUTTON_LEFT)
                                mygrid.cellsign[i][j] = 1;
                            //else
                          //    mygrid.cellsign[i][j] = 2;
                         
                        // Reload grid
                            SDL_RenderClear(rend);
                            DrawGrid(rend, mygrid);
                            SDL_RenderPresent(rend);
                        // Give move to computer
                            player_move = 0; 

                        }
                     }                            
                 }
              }
         }
        
    }
        
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    
}

/****************************************
*************** FUNCTIONS ***************
****************************************/

// Function to draw the game grid V.2
// Args: render, grid
void DrawGrid (SDL_Renderer* r, struct Grid g)
{
    int x = g.center.x - g.cellsize * 1.5; // Get up left corner
    int y = g.center.y - g.cellsize * 1.5; //
    int step = g.cellsize;
   
    // Draw grid...
    for (int i = 0; i < 4 * step; i += step)      // with border
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


// Function checks win
int CheckWin(struct Grid g, int winner)
{
    int n = 0;
    
    //Check lines
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (g.cellsign[i][j] == winner)  n++;
        }
        
        if (n == 3) return 1;
        else n = 0;
    }
    
    //Check columns
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (g.cellsign[j][i] == winner) n++;
        }

        if (n == 3) return 1;
        else n = 0;
    }
    
    //Check 1st diagonal
    
    if (g.cellsign[0][0] == winner) n++;
    if (g.cellsign[1][1] == winner) n++;
    if (g.cellsign[2][2] == winner) n++;
    
    if (n == 3) return 1;
    else n = 0;
    
    //Check 2st diagonal
    
    if (g.cellsign[0][2] == winner) n++;
    if (g.cellsign[1][1] == winner) n++;
    if (g.cellsign[2][0] == winner) n++;
    
    if (n == 3)
        return 1;
    
    return 0; //no winner
}
//=============== end CheckWin ==================





















