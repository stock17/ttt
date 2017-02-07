#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
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

// void GRID_Draw (SDL_Renderer* r, int x, int y, int size);  // V.1
void GRID_Draw (SDL_Renderer* r, struct Grid g);              // V.2
void DrawCircle (SDL_Renderer* r, int cx, int cy, int radius);
void DrawCross (SDL_Renderer* r, int cx, int cy, int size);
int  GRID_CheckWin(struct Grid g, int winner);
int  GRID_CheckNoMove(struct Grid g);
int  CompMove (struct Grid * g_ptr);

void ShowMessage(SDL_Renderer * r, char * text);
int ContinueGame (SDL_Renderer * r, struct Grid * g);



int main (void)
{
    SDL_Window *win;
    SDL_Renderer *rend;
    SDL_Event e;    
       
// ================== Init SDL =======================
    SDL_Init (SDL_INIT_VIDEO);
    win = SDL_CreateWindow("Tic-tac-toe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, RES_X, RES_Y, 0);
    rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);    

      // Clear screen    
    SDL_SetRenderDrawColor(rend,0,0,0,255);  
    SDL_RenderClear(rend);
    SDL_RenderPresent(rend);
        
// ================= Init TTF =======================

    TTF_Init();
    if(TTF_Init()==-1) {
       printf("TTF_Init: %s\n", TTF_GetError());
       exit(2);
    }
   
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
    
    

    GRID_Draw (rend, mygrid);
    SDL_RenderPresent(rend);
    int player_move = 1;   
    
    
//===================================================
//================ Main loop  =======================
//===================================================
    while(1)
    {
        
    // ---------- Checking win --------------
        if (GRID_CheckWin (mygrid, 1)) {
            SDL_Delay(1000);
            ShowMessage(rend, "YOU WIN!");            
            if (!ContinueGame(rend, &mygrid))
                break;
        }    
        else if (GRID_CheckWin (mygrid,2)) {
            SDL_Delay(1000);
            ShowMessage(rend, "YOU LOSE!");
            if (!ContinueGame(rend, &mygrid))
                break;            
        }
        else if (GRID_CheckNoMove (mygrid)) {
            SDL_Delay(1000);
            ShowMessage(rend, "DRAW!");
            if (!ContinueGame(rend, &mygrid))
                break;
        }

     // ----------- Computer move --------------
   
        if (!player_move)
        {
            if (CompMove(&mygrid)) {
            
                SDL_Delay(1000);
            
                SDL_SetRenderDrawColor(rend,0,0,0,0);
                SDL_RenderClear(rend);
                GRID_Draw(rend, mygrid);
                SDL_RenderPresent(rend);
            
                player_move = 1;                
                           
            }
                    
        }
        
    // ------------ Player move ---------------
        else {                

            SDL_WaitEvent(&e);

    // ----------- Check quit ---------------- 
            
            if (e.type == SDL_QUIT)
                break;

            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mx, my;
                SDL_GetMouseState(&mx, &my);
           
            // Check if mouse coordinates in any cell 
                       
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {    
                        if // mouse touches some cell, and cell hasn't sign
                         (
                             (mx > mygrid.cellcenter[i][j].x - mygrid.cellsize/2 && my > mygrid.cellcenter[i][j].y - mygrid.cellsize/2) &&
                             (mx < mygrid.cellcenter[i][j].x + mygrid.cellsize/2 && my < mygrid.cellcenter[i][j].y + mygrid.cellsize/2) &&
                             (mygrid.cellsign[i][j] == 0)
                         )
                         // then set sign
                         {                           
                            mygrid.cellsign[i][j] = 1;                            
                         
                        // Reload grid                        
                            SDL_SetRenderDrawColor(rend,0,0,0,0);
                            SDL_RenderClear(rend);
                            GRID_Draw(rend, mygrid);
                            SDL_RenderPresent(rend);
                        
                        // Give move to computer
                            player_move = 0; 

                        }
                     }                            
                 }
              }
         }
        
    }

    TTF_Quit();
            
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    
} //-------------- Main end --------------




/****************************************
*************** FUNCTIONS ***************
****************************************/

// Function to draw the game grid V.2
// Args: render, grid
void GRID_Draw (SDL_Renderer* r, struct Grid g)
{
    int x = g.center.x - g.cellsize * 1.5; // Get up left corner
    int y = g.center.y - g.cellsize * 1.5; //
    int step = g.cellsize;
   
    // Draw grid...
    SDL_SetRenderDrawColor(r,255,255,255,255);
    
    for (int i = 0; i < 4 * step; i += step)      // with border
    //for (int i = step; i < 3 * step; i += step) // without border
    {
        SDL_RenderDrawLine(r, x, y+i, x+3*step, y+i);
        SDL_RenderDrawLine(r, x+i, y, x+i, y+3*step);           
    }
    
    //Double border
    SDL_Rect db = {x - g.cellsize / 5, y - g.cellsize / 5, g.cellsize * 3 + 2 * g.cellsize / 5, g.cellsize * 3 + 2 * g.cellsize / 5};
    SDL_RenderDrawRect(r, &db);
    
    
    
    //Draw signs
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {    
            SDL_SetRenderDrawColor(r,255,255,255,255);        
            SDL_RenderDrawPoint(r, g.cellcenter[i][j].x, g.cellcenter[i][j].y);

            if (g.cellsign[i][j] == 1)
            {
                SDL_SetRenderDrawColor(r,255,0,0,255);
                DrawCross(r, g.cellcenter[i][j].x, g.cellcenter[i][j].y, g.cellsize / 3);
            }
            else if (g.cellsign[i][j] == 2)
            {
                SDL_SetRenderDrawColor(r,0,255,0,255);
                DrawCircle(r, g.cellcenter[i][j].x, g.cellcenter[i][j].y, g.cellsize / 3);
            }
           

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
int GRID_CheckWin(struct Grid g, int winner)
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
//=============== end GRID_CheckWin ==================


// Function checks moves
int GRID_CheckNoMove(struct Grid g)
{
    //Check the grid to free cells
    
    for (int i = 0; i < 3; i++) 
    {
        for (int j = 0; j < 3; j++) 
        {
            if (g.cellsign[i][j] == 0)  
                return 0;
        }
        
    }

    return 1;
}


//=================================================
//========== Artifical Intelligence ===============
//=================================================

int CompMove (struct Grid * g_ptr) {

// 1. Check lines, columns and diagonal for 2 same signs

    int n_foe = 0;
    int n_ally = 0;
    int foe = 1;
    int ally = 2;

// 1.1. Check lines

    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (g_ptr->cellsign[i][j] == ally)  n_ally++;           
        }
        
        if (n_ally == 2) {
            for (int k = 0; k < 3; k++) {
                if (g_ptr->cellsign[i][k] == 0) {
                    g_ptr->cellsign[i][k] = ally;
                    return 1;
                }
            }
        }

        n_foe = n_ally = 0;
    }
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (g_ptr->cellsign[i][j] == foe)  n_foe++;
        }
        
        if (n_foe == 2) {
            for (int k = 0; k < 3; k++) {
                if (g_ptr->cellsign[i][k] == 0) {
                    g_ptr->cellsign[i][k] = ally;
                    return 1;
                }
            }
        }
        n_foe = n_ally = 0;
    }

// 1.2. Check columns
       
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 3; i++) {            
            if (g_ptr->cellsign[i][j] == ally)  n_ally++;
        }
        
        if (n_ally == 2) {
            for (int k = 0; k < 3; k++) {
                if (g_ptr->cellsign[k][j] == 0) {
                    g_ptr->cellsign[k][j] = ally;
                    return 1;
                }
                
            }
        }

        n_ally = n_foe = 0;
    }
    
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 3; i++) {
            if (g_ptr->cellsign[i][j] == foe)  n_foe++;           
        }            

        if (n_foe == 2) {
            for (int k = 0; k < 3; k++) {
                if (g_ptr->cellsign[k][j] == 0) {
                    g_ptr->cellsign[k][j] = ally;
                    return 1;
                }
                
            }
        }
        n_ally = n_foe = 0;
    }
    
// 1.3. Check 1st diagonal 
       
    if (g_ptr->cellsign[0][0] == ally) n_ally++;
    if (g_ptr->cellsign[1][1] == ally) n_ally++;
    if (g_ptr->cellsign[2][2] == ally) n_ally++;


    if (n_ally == 2)  {
        for (int k = 0; k < 3; k++) {
            if (g_ptr->cellsign[k][k] == 0) {
                g_ptr->cellsign[k][k] = ally;
                return 1;
            }            
        }
    }

    if (g_ptr->cellsign[0][0] == foe) n_foe++;
    if (g_ptr->cellsign[1][1] == foe) n_foe++;
    if (g_ptr->cellsign[2][2] == foe) n_foe++;
    
    

    if (n_foe == 2)  {
        for (int k = 0; k < 3; k++) {
            if (g_ptr->cellsign[k][k] == 0) {
                g_ptr->cellsign[k][k] = ally;
                return 1;
            }            
        }
    }
    n_foe = n_ally = 0;

    
// 1.4. Check 2st diagonal
    
    if (g_ptr->cellsign[0][2] == ally) n_ally++;
    if (g_ptr->cellsign[1][1] == ally) n_ally++;
    if (g_ptr->cellsign[2][0] == ally) n_ally++;

    if (n_ally == 2)  {
        for (int k = 0; k < 3; k++) {
            if (g_ptr->cellsign[k][2-k] == 0) {
                g_ptr->cellsign[k][2-k] = ally;
                return 1;
            }            
        }
    }
    
    if (g_ptr->cellsign[0][2] == foe) n_foe++;
    if (g_ptr->cellsign[1][1] == foe) n_foe++;
    if (g_ptr->cellsign[2][0] == foe) n_foe++;
    
    if (n_foe == 2) {
        for (int k = 0; k < 3; k++) {
            if (g_ptr->cellsign[k][2-k] == 0) {
                g_ptr->cellsign[k][2-k] = ally;
                return 1;
            }
        }
    }

// 2. No 2 same signs in rows, lines and diagonals

    // 2.1. Check center cell
    if (g_ptr->cellsign[1][1] == 0) { 
        g_ptr->cellsign[1][1] = ally;
        return 1;
    }
    
    // 2.2. Count bare cells
    int barecell [9][2];
    int n_barecell = 0;
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (g_ptr->cellsign[i][j] == 0)  {
                barecell[n_barecell][0] = i;
                barecell[n_barecell][1] = j;
                n_barecell++;;
            }
        }
    }
    // 2.3. Set sign to random bare cell
    srand(time(NULL));
    int n = rand()%n_barecell;
    g_ptr->cellsign[barecell [n] [0] ]  [ barecell [n] [1] ] = ally;
    return 1;    
    

    return 0;
}    

//================ Show Message ======================

void ShowMessage(SDL_Renderer * r, char * text)
{
    TTF_Font * f = TTF_OpenFont("FreeSans.ttf", 40);

    SDL_Color text_color = {255, 255, 255};
    SDL_Surface * text_surface = TTF_RenderText_Solid(f, text, text_color);
    SDL_Texture * text_texture = SDL_CreateTextureFromSurface(r, text_surface);

    SDL_SetRenderDrawColor(r,0,0,0,255);
    SDL_RenderClear(r);
    SDL_Rect dst = {RES_X/8*3,RES_Y/8*3,RES_X/4,RES_Y/4};
    SDL_RenderCopy(r, text_texture, NULL, &dst);
    SDL_RenderPresent(r);
    SDL_Delay(2000);
    
    SDL_FreeSurface(text_surface);    
    SDL_DestroyTexture(text_texture);
    TTF_CloseFont(f);
}
// ================= Continue ========================

int ContinueGame (SDL_Renderer * r, struct Grid * g)

{
    SDL_Event e1;     
    
    char *text = "Do you want to play once again? Y/N (LMB/RMB)";
    TTF_Font * f = TTF_OpenFont("FreeSans.ttf", RES_X/32);
    SDL_Color fg = {255, 255, 255};
    SDL_Color bg = {0, 0, 0};
    SDL_Surface * text_surface = TTF_RenderText_Shaded(f, text, fg, bg);
    SDL_Texture * text_texture = SDL_CreateTextureFromSurface(r, text_surface);

    SDL_SetRenderDrawColor(r,0,0,0,255);
    SDL_RenderClear(r);
    SDL_Rect dst = {RES_X/4,RES_Y/8*3,RES_X/2,RES_Y/16};
    SDL_RenderCopy(r, text_texture, NULL, &dst);
    SDL_RenderPresent(r);
    
    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);  
    TTF_CloseFont(f);  
 
    
    while (1)
    {
        SDL_WaitEvent(&e1);

        if (e1.type == SDL_QUIT)
        {
            return 0;
        }
        

        if ( (e1.type == SDL_KEYDOWN && e1.key.keysym.sym == SDL_GetKeyFromName("Y")) ||
             (e1.type == SDL_MOUSEBUTTONDOWN && e1.button.button == SDL_BUTTON_LEFT) )
        
        {
            SDL_SetRenderDrawColor(r,0,0,0,0);                
            SDL_RenderClear(r);
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    g->cellsign[i][j] = 0;                 

            GRID_Draw(r, *g);                
            SDL_RenderPresent(r);
            return 1; 
        }

        else if ( (e1.type == SDL_KEYDOWN && e1.key.keysym.sym == SDL_GetKeyFromName("N")) ||
             (e1.type == SDL_MOUSEBUTTONDOWN && e1.button.button == SDL_BUTTON_RIGHT) )
                return 0;
        
    }        
}









