/*
 * PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * main.c - Basic Input demo -- reads from control pad and indicates button
 *          presses.
 *
 * Copyright (c) 2005 Marcus R. Brown <mrbrown@ocgnet.org>
 * Copyright (c) 2005 James Forshaw <tyranid@gmail.com>
 * Copyright (c) 2005 John Kelley <ps2dev@kelley.ca>
 * Copyright (c) 2005 Donour Sizemore <donour@uchicago.edu>
 *
 * $Id: main.c 1095 2005-09-27 21:02:16Z jim $
 */
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>

#include <stdlib.h>
#include <string.h>

/* Define the module info section */
PSP_MODULE_INFO("CONTROLTEST", 0, 1, 1);

/* Define the main thread's attribute value (optional) */
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

/* Define printf, just to make typing easier */
#define printf	pspDebugScreenPrintf

void dump_threadstatus(void);

int done = 0;


char levels[3][20][20] =
{
    {{'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'},{'W', 'P', 'D', 'D', 'W', 'D', 'D', 'D', 'D', 'D', 'W', 'D', 'W', 'D', 'W', 'T', 'D', 'W', 'D', 'W'}, {'W', 'D', 'W', 'D', 'W', 'D', 'W', 'D', 'W', 'D', 'W', 'D', 'W', 'D', 'W', 'W', 'D', 'W', 'D', 'W'}, {'W', 'D', 'W', 'D', 'D', 'D', 'W', 'D', 'W', 'D', 'D', 'D', 'W', 'D', 'W', 'D', 'D', 'D', 'D', 'W'}, {'W', 'W', 'W', 'D', 'W', 'W', 'W', 'W', 'W', 'D', 'W', 'D', 'W', 'D', 'W', 'W', 'W', 'D', 'W', 'W'}, {'W', 'D', 'W', 'D', 'W', 'D', 'D', 'D', 'D', 'D', 'W', 'D', 'W', 'D', 'D', 'D', 'D', 'D', 'D', 'W'}, {'W', 'D', 'W', 'D', 'W', 'W', 'W', 'D', 'W', 'W', 'W', 'W', 'W', 'D', 'W', 'W', 'W', 'W', 'W', 'W'}, {'W', 'D', 'D', 'D', 'D', 'D', 'W', 'D', 'D', 'D', 'W', 'T', 'W', 'D', 'W', 'D', 'W', 'T', 'D', 'W'}, {'W', 'W', 'W', 'W', 'W', 'D', 'W', 'W', 'W', 'D', 'W', 'D', 'W', 'D', 'W', 'D', 'W', 'W', 'D', 'W'}, {'W', 'D', 'D', 'D', 'D', 'D', 'W', 'D', 'D', 'D', 'W', 'D', 'D', 'D', 'D', 'D', 'W', 'D', 'D', 'W'}, {'W', 'W', 'W', 'D', 'W', 'D', 'W', 'W', 'W', 'D', 'W', 'W', 'W', 'D', 'W', 'W', 'W', 'D', 'W', 'W'}, {'W', 'D', 'D', 'D', 'W', 'D', 'W', 'T', 'W', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'}, {'W', 'D', 'W', 'W', 'W', 'W', 'W', 'D', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'D', 'W'}, {'W', 'D', 'W', 'D', 'W', 'D', 'D', 'D', 'W', 'D', 'W', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'}, {'W', 'D', 'W', 'D', 'W', 'D', 'W', 'W', 'W', 'D', 'W', 'W', 'W', 'D', 'W', 'W', 'W', 'W', 'W', 'W'}, {'W', 'W', 'W', 'D', 'W', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W', 'D', 'D', 'D', 'D', 'W'}, {'W', 'D', 'D', 'D', 'D', 'D', 'W', 'D', 'W', 'W', 'W', 'W', 'W', 'D', 'D', 'D', 'D', 'D', 'T', 'W'}, {'W', 'D', 'W', 'W', 'W', 'D', 'W', 'D', 'W', 'D', 'D', 'D', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'}, {'W', 'D', 'T', 'W', 'D', 'D', 'W', 'D', 'D', 'D', 'W', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'E', 'W'}, {'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'}},
    {{'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'},{'W', 'P', 'D', 'D', 'W', 'D', 'D', 'D', 'D', 'D', 'W', 'D', 'W', 'D', 'D', 'D', 'D', 'D', 'D', 'W'}, {'W', 'D', 'W', 'D', 'W', 'D', 'W', 'W', 'W', 'D', 'W', 'D', 'W', 'D', 'W', 'W', 'W', 'W', 'D', 'W'}, {'W', 'D', 'W', 'D', 'W', 'D', 'W', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W', 'D', 'D', 'W'}, {'W', 'D', 'W', 'W', 'W', 'D', 'W', 'W', 'W', 'D', 'W', 'W', 'W', 'W', 'W', 'D', 'W', 'D', 'D', 'W'}, {'W', 'D', 'D', 'D', 'D', 'D', 'W', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W', 'D', 'W', 'D', 'W', 'W'}, {'W', 'W', 'W', 'W', 'W', 'W', 'W', 'D', 'W', 'W', 'W', 'D', 'W', 'D', 'W', 'W', 'W', 'D', 'D', 'W'}, {'W', 'D', 'D', 'T', 'W', 'D', 'D', 'D', 'D', 'D', 'W', 'D', 'W', 'D', 'W', 'T', 'W', 'D', 'D', 'W'}, {'W', 'D', 'W', 'W', 'W', 'D', 'W', 'D', 'W', 'D', 'W', 'W', 'W', 'W', 'W', 'D', 'W', 'W', 'D', 'W'}, {'W', 'D', 'D', 'D', 'D', 'D', 'W', 'D', 'W', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W', 'D', 'D', 'W'}, {'W', 'W', 'W', 'D', 'W', 'W', 'W', 'W', 'W', 'D', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'D', 'D', 'W'}, {'W', 'D', 'W', 'D', 'D', 'D', 'W', 'D', 'D', 'D', 'D', 'D', 'D', 'T', 'W', 'T', 'W', 'D', 'W', 'W'}, {'W', 'D', 'W', 'D', 'W', 'W', 'W', 'W', 'W', 'D', 'W', 'W', 'W', 'W', 'W', 'D', 'W', 'D', 'D', 'W'}, {'W', 'D', 'D', 'D', 'D', 'D', 'D', 'T', 'W', 'D', 'W', 'D', 'D', 'D', 'D', 'D', 'W', 'D', 'D', 'W'}, {'W', 'D', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'D', 'W', 'W', 'W', 'W', 'W', 'D', 'W', 'W', 'D', 'W'}, {'W', 'D', 'D', 'D', 'D', 'D', 'W', 'D', 'D', 'D', 'D', 'W', 'D', 'W', 'D', 'D', 'D', 'D', 'D', 'W'}, {'W', 'W', 'W', 'D', 'W', 'D', 'D', 'D', 'D', 'W', 'D', 'D', 'D', 'W', 'D', 'W', 'D', 'W', 'W', 'W'}, {'W', 'T', 'W', 'D', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'D', 'W', 'D', 'W', 'E', 'W'}, {'W', 'D', 'D', 'D', 'D', 'D', 'D', 'W', 'T', 'T', 'D', 'D', 'D', 'D', 'D', 'W', 'D', 'D', 'D', 'W'}, {'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'}},
    {{'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'},{'W', 'P', 'D', 'D', 'W', 'D', 'D', 'D', 'D', 'D', 'W', 'D', 'W', 'D', 'W', 'T', 'D', 'W', 'D', 'W'}, {'W', 'D', 'W', 'D', 'W', 'D', 'W', 'D', 'W', 'D', 'W', 'D', 'W', 'D', 'W', 'W', 'D', 'W', 'D', 'W'}, {'W', 'D', 'W', 'D', 'D', 'D', 'W', 'D', 'W', 'D', 'D', 'D', 'W', 'D', 'W', 'D', 'D', 'D', 'D', 'W'}, {'W', 'W', 'W', 'D', 'W', 'W', 'W', 'W', 'W', 'D', 'W', 'D', 'W', 'D', 'W', 'W', 'W', 'D', 'W', 'W'}, {'W', 'D', 'W', 'D', 'W', 'D', 'D', 'D', 'D', 'D', 'W', 'D', 'W', 'D', 'D', 'D', 'D', 'D', 'D', 'W'}, {'W', 'D', 'W', 'D', 'W', 'W', 'W', 'D', 'W', 'W', 'W', 'W', 'W', 'D', 'W', 'W', 'W', 'W', 'W', 'W'}, {'W', 'D', 'D', 'D', 'D', 'D', 'W', 'D', 'D', 'D', 'W', 'T', 'W', 'D', 'W', 'D', 'W', 'T', 'D', 'W'}, {'W', 'W', 'W', 'W', 'W', 'D', 'W', 'W', 'W', 'D', 'W', 'D', 'W', 'D', 'W', 'D', 'W', 'W', 'D', 'W'}, {'W', 'D', 'D', 'D', 'D', 'D', 'W', 'D', 'D', 'D', 'W', 'D', 'D', 'D', 'D', 'D', 'W', 'D', 'D', 'W'}, {'W', 'W', 'W', 'D', 'W', 'D', 'W', 'W', 'W', 'D', 'W', 'W', 'W', 'D', 'W', 'W', 'W', 'D', 'W', 'W'}, {'W', 'D', 'D', 'D', 'W', 'D', 'W', 'T', 'W', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'}, {'W', 'D', 'W', 'W', 'W', 'W', 'W', 'D', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'D', 'W'}, {'W', 'D', 'W', 'D', 'W', 'D', 'D', 'D', 'W', 'D', 'W', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'}, {'W', 'D', 'W', 'D', 'W', 'D', 'W', 'W', 'W', 'D', 'W', 'W', 'W', 'D', 'W', 'W', 'W', 'W', 'W', 'W'}, {'W', 'W', 'W', 'D', 'W', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W', 'D', 'D', 'D', 'D', 'W'}, {'W', 'D', 'D', 'D', 'D', 'D', 'W', 'D', 'W', 'W', 'W', 'W', 'W', 'D', 'D', 'D', 'D', 'D', 'T', 'W'}, {'W', 'D', 'W', 'W', 'W', 'D', 'W', 'D', 'W', 'D', 'D', 'D', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'}, {'W', 'D', 'T', 'W', 'D', 'D', 'W', 'D', 'D', 'D', 'W', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'E', 'W'}, {'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'}}
};

char board[20][20] = {
    {'W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W'},{'W','T','T','T','R','R','R','D','D','W','D','D','D','D','D','R','T','T','T','W'},{'W','T','T','T','R','R','R','D','D','W','D','D','D','D','D','R','T','T','T','W'},{'W','T','T','T','R','R','R','D','D','W','D','D','D','D','D','R','T','T','T','W'},{'W','T','T','T','R','R','R','D','D','W','D','D','D','D','D','R','T','T','T','W'},{'W','T','T','T','R','R','R','D','D','W','D','D','D','D','D','R','T','T','T','W'},{'W','T','T','T','R','R','R','D','D','W','D','D','D','D','D','R','T','T','T','W'},{'W','T','T','T','R','R','R','D','D','W','D','D','D','D','D','R','T','T','T','W'},{'W','T','T','T','R','R','R','D','D','W','D','D','D','D','D','R','T','T','T','W'},{'W','T','T','T','R','R','R','D','D','P','D','D','D','D','D','R','T','T','T','W'},{'W','T','T','T','R','R','R','D','D','W','D','D','D','D','D','R','T','T','T','W'},{'W','T','T','T','R','R','R','D','D','W','D','D','D','D','D','R','T','T','T','E'},{'W','T','T','T','R','R','R','D','D','W','D','D','D','D','D','R','T','T','T','W'},{'W','T','T','T','R','R','R','D','D','W','D','D','D','D','D','R','T','T','T','W'},{'W','T','T','T','R','R','R','D','D','W','D','D','D','D','D','R','T','T','T','W'},{'W','T','T','T','R','R','R','D','D','W','D','D','D','D','D','R','T','T','T','W'},{'W','T','T','T','R','R','R','D','D','W','D','D','D','D','D','R','T','T','T','W'},{'W','T','T','T','R','R','R','D','D','W','D','D','D','D','D','R','T','T','T','W'},{'W','T','T','T','R','R','R','D','D','W','D','D','D','D','D','R','T','T','T','W'},{'W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W','W'}
};
int playerPos[] = {1,1};
int steps = 0;
int collectedTreasure = 0;
int score = 0;
int level = 0;
//int onMenu = 0;
int gameOver = 0;
int onMenu = 1;
int difficultyLevel = 0;
int isButtonHeld = 0;
int howManyLevels = 3;



/* Exit callback */
int exit_callback(int arg1, int arg2, void *common)
{
	done = 1;
	return 0;
}

/* Callback thread */
int CallbackThread(SceSize args, void *argp)
{
	int cbid;

	cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();

	return 0;
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void)
{
	int thid = 0;

	thid = sceKernelCreateThread("update_thread", CallbackThread,
				     0x11, 0xFA0, 0, 0);
	if(thid >= 0)
	{
		sceKernelStartThread(thid, 0, 0);
	}

	return thid;
}


void loadLevel()
{
    level += 1;
    board[playerPos[1]][playerPos[0]] = 'B';
    playerPos[0] = 1;
    playerPos[1] =  1;
    board[playerPos[1]][playerPos[0]] = 'P';

    if((level - 1) < howManyLevels)
    {
        for (int yP = 0; yP < 20; yP++)
        {
            for (int xP = 0; xP < 20; xP++)
            {
                board[yP][xP] = levels[level - 1][yP][xP];
            }
        }
    }
    else
    {
        //gameOver = true;
		gameOver = 1;
    }
    
    
    return;
}

void startGame()
{
    level = 0;
    steps = 0;
    score = 0;
    collectedTreasure = 0;
    
    loadLevel();
    
    //sceRtcGetCurrentClockLocalTime(&time);
}

void move(int dX, int dY)
{

	if(playerPos[0] + dX > 19 || playerPos[0] + dX < 0 || playerPos[1] + dY < 0 || playerPos[1] + dY > 19)
    {
        return;
    }
	if (board[playerPos[1] + dY][playerPos[0] + dX] == 'T')
    {
        score += 20;
        collectedTreasure += 1;
    }
    if (board[playerPos[1] + dY][playerPos[0] + dX] == 'W')
    {
        steps += 1;
        return;
    }
	if (board[playerPos[1] + dY][playerPos[0] + dX] == 'E')
    {
        
        

        loadLevel();

        
        return;
        
    }

	board[playerPos[1]][playerPos[0]] = 'B';

    playerPos[0] = playerPos[0] + dX;
    playerPos[1] =  playerPos[1] + dY;

    board[playerPos[1]][playerPos[0]] = 'P';
    steps += 1;
}

int main(void)
{
	SceCtrlData pad;

	pspDebugScreenInit();
	SetupCallbacks();

	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
	while(!done){
		pspDebugScreenSetXY(0, 2);

    		sceCtrlReadBufferPositive(&pad, 1); 

		printf("Analog X = %d ", pad.Lx);
		printf("Analog Y = %d \n", pad.Ly);

		if(onMenu == 1)
		{
			printf("\n\n");
			printf("RRRRRRRRRRRRRRRRR   BBBBBBBBBBBBBBBBB   \n");
			printf("R::::::::::::::::R  B::::::::::::::::B  \n");
			printf("R::::::RRRRRR:::::R B::::::BBBBBB:::::B \n");
			printf("RR:::::R     R:::::RBB:::::B     B:::::B\n");
			printf("  R::::R     R:::::R  B::::B     B:::::B\n");
			printf("  R::::R     R:::::R  B::::B     B:::::B\n");
			printf("  R::::RRRRRR:::::R   B::::BBBBBB:::::B \n");
			printf("  R:::::::::::::RR    B:::::::::::::BB  \n");
			printf("  R::::RRRRRR:::::R   B::::BBBBBB:::::B \n");
			printf("  R::::R     R:::::R  B::::B     B:::::B\n");
			printf("  R::::R     R:::::R  B::::B     B:::::B\n");
			printf("  R::::R     R:::::R  B::::B     B:::::B\n");
			printf("RR:::::R     R:::::RBB:::::BBBBBB::::::B\n");
			printf("R::::::R     R:::::RB:::::::::::::::::B \n");
			printf("R::::::R     R:::::RB::::::::::::::::B  \n");
			printf("RRRRRRRR     RRRRRRRBBBBBBBBBBBBBBBBB\n");
			printf("press square\n");
			if (pad.Buttons != 0){
				

				if(pad.Buttons & PSP_CTRL_TRIANGLE)
				{
					difficultyLevel = 0;
					gameOver = 0;
					onMenu = 0;
					startGame();
				}
				if(pad.Buttons & PSP_CTRL_SQUARE)
				{
					difficultyLevel = 1;
					gameOver = 0;
					onMenu = 0;
					startGame();
				}
				if(pad.Buttons & PSP_CTRL_CROSS)
				{
					difficultyLevel = 2;
					gameOver = 0;
					onMenu = 0;
					startGame();
				}

				/*ADD LEVEL LOADING*/
				/*
					if(pad.Buttons & PSP_CTRL_CIRCLE)
					{
						loadFileLevel();
					}
				*/
			}
			continue;
		}
		else if(gameOver == 0)
		{

			for (int y = 0; y < 20; y++)
			{
				for (int x = 0; x < 20; x++)
				{
					//printf("%c", board[y][x]);


					//int drawingX = 6 + (13 * x);
					//int drawingY = 6 + (13 * y);

					if (difficultyLevel == 1)
					{
						if(x > (playerPos[0] + 2) || x < (playerPos[0] - 2))
						{
							if(x != 0 && x != 19 && y != 0 && y != 19)
							{
								printf(" ");
								continue;
							}
						}

						if(y > (playerPos[1] + 2) || y < (playerPos[1] - 2))
						{
							if(y != 0 && y != 19 && x != 0 && x != 19)
							{
								printf(" ");
								continue;
							}
						}
					}
					else if(difficultyLevel == 2)
					{
						if(x > (playerPos[0] + 1) || x < (playerPos[0] - 1))
						{
							if(y != 0 && y != 19 && x != 0 && x != 19)
							{
								printf(" ");
								continue;
							}
						}

						if(y > (playerPos[1] + 1) || y < (playerPos[1] - 1))
						{
							if(y != 0 && y != 19 && x != 0 && x != 19)
							{
								printf(" ");
								continue;
							}
						}

					}


					if(board[y][x] == 'B')
					{
						printf(" ");
					}
					else
					{
						printf("%c", board[y][x]);
					}
				}
				printf("\n");
			}
			printf("\nScore: %i", score);
			printf("\nTreasure: %i", collectedTreasure);
			printf("\nSteps: %i \n", steps);
			printf("\nLevel: %i \n", level);

			if (pad.Buttons != 0){
				if (pad.Buttons & PSP_CTRL_SQUARE){
					isButtonHeld = 0;
					printf("Square pressed \n");
				}
				
				if (pad.Buttons & PSP_CTRL_SELECT){
					onMenu = 1;
					printf("Square pressed \n");
				}
				

				if (pad.Buttons & PSP_CTRL_UP){
					if(isButtonHeld == 0)
					{
						printf("Up pressed \n");
						move(0, -1);
					}
					isButtonHeld = 1;
				} 
				else if (pad.Buttons & PSP_CTRL_DOWN){
					if(isButtonHeld == 0)
					{
						printf("Down pressed \n");
						move(0, 1);
					}
					isButtonHeld = 1;
				} 
				else if (pad.Buttons & PSP_CTRL_LEFT){
					if(isButtonHeld == 0)
					{
						printf("Left pressed \n");
						move(-1, 0);
					}
					isButtonHeld = 1;
				} 
				else if (pad.Buttons & PSP_CTRL_RIGHT){
					if(isButtonHeld == 0)
					{
						printf("Right pressed \n");
						move(1, 0);
					}
					isButtonHeld = 1;
				}
				

				     
			}
			else
			{
				isButtonHeld = 0;
			}      
			
		}
		else
		{
			for (int i = 0; i < 20; i++)
			{
				
				printf("game over\n");
			}
			printf("Press square for menu");
			if (pad.Buttons != 0){
				if (pad.Buttons & PSP_CTRL_SQUARE){
					gameOver = 0;
					onMenu = 1;
				}
			}
		}
	}

	sceKernelExitGame();
	return 0;
}
