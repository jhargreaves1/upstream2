#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <cstring>
#include <stdio.h>
#include <cmath>
#include <stdlib.h>
#include "kevinJ.h"
#include "jonathanD.h"

void gameOver(Game *game);

void collision(Game *game)
{
	if (game->c.isJumping) {
		game->c.center[1] += game->c.velocity[1];
		game->c.velocity[0] = 0;
		game->c.velocity[1] -= 0.8;
		if (game->c.center[1] <= 0) {
			game->c.isJumping = false;
			game->c.isStanding = true;
			game->c.center[1] = 15;

		}
	}
	if (game->c.center[0] == game->c.newPosX &&
			game->c.center[1] == game->c.newPosY)
		return;
	if (game->c.center[0] < game->c.newPosX) {
		if (game->c.isJumping) {
			game->c.center[0] += 20.0;
		} else {
			game->c.center[0] += 10.0;
			if(game->c.velocity[0]<=0 )
				game->c.velocity[0]+=0.2;
		}
		if (game->c.center[0] > game->c.newPosX)
			game->c.center[0] = game->c.newPosX;
	} else if (game->c.center[0] > game->c.newPosX) {
		if (game->c.isJumping) {
			game->c.center[0] -= 15.0;
		} else {
			game->c.center[0] -= 10.0;
			if(game->c.velocity[0]>=0 )
				game->c.velocity[0] -=0.2;
		}
		if (game->c.center[0] < game->c.newPosX)
			game->c.center[0] = game->c.newPosX;
	}
	//check for frog(ball)'s collision with lilies
	/*
	Lilypad *node = game->ihead;
	if (game->c.isStanding == false) {
		while (node) {
			float d1 = node->pos[1] - game->c.center[1];
			float d2 = node->pos[0] - game->c.center[0];
			float dist = sqrt(d1*d1 + d2*d2);
			if (dist <= game->c.radius+15.0) {
				//rejump the ball
				game->c.isJumping = true;
				game->c.velocity[1] = 15.0;
				playSounds("./wav/boing.wav",1.0f, false,game->muted);
				deleteLily(node,game);
				//basic scoring for now -Kevin
				game->score+=10;
			}
			node = node->next;
		}
	}
	*/
	// ========================================
	// collision frog with log
	for(int i=0;i<4;i++)
	{
	if (game->c.center[0] <= game->log[i]->getXpos()+15 &&
			game->c.center[0] >= game->log[i]->getXpos()-15 &&
			game->c.center[1] <= game->log[i]->getYpos()+50 &&
			game->c.center[1] >= game->log[i]->getYpos()-50) {
		game->c.velocity[1]=game->log[i]->getYvel()+15;
		playSounds("./wav/tick.wav",1.0f, false, game->muted);
		game->score+=2;

        }
	}

	// collision frog with lp
	for(int i=0;i<game->num_lp;i++)
	{
	if (game->c.center[0] <= game->lp[i]->getXpos()+20 &&
			game->c.center[0] >= game->lp[i]->getXpos()-20 &&
			game->c.center[1] <= game->lp[i]->getYpos()+20 &&
			game->c.center[1] >= game->lp[i]->getYpos()-20) {
		game->c.velocity[1]=game->lp[i]->getYvel()+16;
		playSounds("./wav/boing.wav",1.0f, false, game->muted);
		game->score+=10;
        game->lp[i]->move(-200,-200,0,-3);
        }
	}
	// collision frog with gator head
	if (game->c.center[0] <= game->gator->getXpos()-10 &&
			game->c.center[0] >= game->gator->getXpos()-40 &&
			game->c.center[1] <= game->gator->getYpos()+15 &&
			game->c.center[1] >= game->gator->getYpos()-15) {
		gameOver(game);
	}
	// collision frog with gator back
	if (game->c.center[0] <= game->gator->getXpos()+50 &&
			game->c.center[0] >= game->gator->getXpos()-6 &&
			game->c.center[1] <= game->gator->getYpos()+30 &&
			game->c.center[1] >= game->gator->getYpos()-30) {
                game->c.isJumping = true;
				game->c.velocity[1] = 15.0;
				playSounds("./wav/boing.wav",1.0f, false,game->muted);
				game->score+=30;
	}
	// collision gator with log
	for(int i=0;i<4;i++)
	{
	if (game->gator->getXpos() <= game->log[i]->getXpos()+80 &&
			game->gator->getXpos() >= game->log[i]->getXpos()-20 &&
			game->gator->getYpos() >= game->log[i]->getYpos()-50 &&
			game->gator->getYpos() <= game->log[i]->getYpos()+50) {
		game->gator->move(game->gator->getXpos()+45,game->gator->getYpos()+15,
				game->gator->getXvel(),game->gator->getYvel());
        }
	}

	//fell down
	if (game->c.center[1]<=20 &&  game->bridge->getYpos()<=120) {
		gameOver(game);

	}
	//frog at bottom
	if (game->c.center[1] <= 15.0)
	{
		game->highscore[++game->scoreCount] = game->score;
		game->score = 0;
	}

}

void screenUpdate(Game *game)
{
    //std::cout<<game->splash->getFrame()<<std::endl;
	if(game->frog->getYpos() > game->windowHeight/2) {
	int move_down = 10;
	if(game->frog->getYpos() > (game->windowHeight/3)*2)
        move_down = 30;
		game->c.center[1]-=move_down;
		game->frog->move(game->frog->getXpos(), game->frog->getYpos()-move_down,
				game->frog->getXvel(), game->frog->getYvel()-5 );
		game->water[0]->move(game->water[0]->getXpos(), game->water[0]->getYpos()-move_down,
				game->water[0]->getXvel(), game->water[0]->getYvel() );
		game->water[1]->move(game->water[1]->getXpos(), game->water[1]->getYpos()-move_down,
				game->water[1]->getXvel(), game->water[1]->getYvel() );
		game->water[2]->move(game->water[2]->getXpos(), game->water[2]->getYpos()-move_down,
				game->water[2]->getXvel(), game->water[2]->getYvel() );
		game->gator->move(game->gator->getXpos(), game->gator->getYpos()-move_down,
				game->gator->getXvel(), game->gator->getYvel() );

				for(int i=0;i<4;i++)
        {
            game->log[i]->move(game->log[i]->getXpos(), game->log[i]->getYpos()-move_down,
				game->log[i]->getXvel(), game->log[i]->getYvel() );
		}

				for(int i=0;i<game->num_lp;i++)
        {
            game->lp[i]->move(game->lp[i]->getXpos(), game->lp[i]->getYpos()-move_down,
				game->lp[i]->getXvel(), game->lp[i]->getYvel() );
		}
		game->bridge->move(game->bridge->getXpos(), game->bridge->getYpos()-move_down,
				game->bridge->getXvel(), game->bridge->getYvel() );
		//shift lilies
		Lilypad *node = game->ihead;
		game->maxtimer = 12; //adjust spawn timer to spawn faster as we go up
		while (node) {
			node->pos[1] -= move_down;
			node = node->next;
		}
	} else {
		game->maxtimer = 45; //reset spawn timer to original if below half screen
	}
}

void gameOver(Game *game)
{
    // move splash on screen if offscreen
    if(game->splash->getXpos()<0)
		game->splash->move(game->c.center[0],game->c.center[1]+40,0,0);
    //move frog,logs and gator offscreen
		game->frog->move(-200,-200,0,0);
		for(int i=0;i<4;i++)
        {
            game->log[i]->move(-200*i,-300,0,0);
		}
		game->gator->move(-200,-200,0,0);
		// reset lilies
		clearLilies(game);

		//wait for splash to complete
	if(game->splash->getFrame()>=195)
	{
	//move bridge back
	game->bridge->move(300,150,0,0);
		for(int i=0;i<4;i++)
        {
            game->log[i]->move(50*i,-100*i,-.15*i,-1);
		}
		game->gator->move(-300,400,-2,-.5);
		game->frog->move(-100,-800,0,0);
		game->c.center[1]=-10;
		game->c.center[0]=-800;
		game->splash->move(-200,-200,0,0);

	}



}
