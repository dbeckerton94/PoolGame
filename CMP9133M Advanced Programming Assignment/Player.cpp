/*-----------------------------------------------------------
Player Class

Computer Science Msc ( Advanced Programming )
Student: Dean Beckerton (BEC12375372)
Module: CMP9133M

-----------------------------------------------------------*/

// SOURCES
#include <iostream>
#include<stdio.h>
#include "stdafx.h"
#include<math.h>
#include"simulation.h"


/*-----------------------------------------------------------
PLAYER CLASS
-----------------------------------------------------------*/

Player::Player(void) // Initilise Player Instance
{
	PlayerShotsLeft = 0;
	BallColourValue = 0;
	HasBlackBeenPotted = false;
	HasPlayerHitABall = false;
}
Player::~Player(void)
{
	// Deconstructor
}
void Player::Reset(void) // Resets the instance of the Player.
{
	PlayerShotsLeft = 0;
	BallColourValue = 0;
	HasBlackBeenPotted = false;
	HasPlayerHitABall = false;
}


// Change player
void table::SwitchPlayer(void)
{
	// Counts Players
	CurrentPlayerCount++;
	NextPlayerCount++;
	if (CurrentPlayerCount == NUM_PLAYERS)
	{
		CurrentPlayerCount = 0;
	}
	if (NextPlayerCount == NUM_PLAYERS)
	{
		NextPlayerCount = 0;
	}

	// If Player Gets Ball of Own Colour, they Get Another Turn
	if (players[CurrentPlayerCount]->BallColourValue == 2 && numBlueBalls != 0)
	{
		players[CurrentPlayerCount]->PlayerShotsLeft = 1 + ExtraTurn;
	}

	// If Player Gets Ball of Own Colour, they Get Another Turn
	else if (players[CurrentPlayerCount]->BallColourValue == 3 && numYellowBalls != 0)
	{
		players[CurrentPlayerCount]->PlayerShotsLeft = 1 + ExtraTurn;
	}

	else
	{
		// Error Handle
		players[CurrentPlayerCount]->PlayerShotsLeft = 1;
	}
	players[CurrentPlayerCount]->PlayerShotsLeft = 1 + ExtraTurn;
	players[NextPlayerCount]->PlayerShotsLeft = 0;

	ExtraTurn = 0;
	HasplayerUsedTurn = false;
	players[CurrentPlayerCount]->HasPlayerHitABall = false;
	players[NextPlayerCount]->HasPlayerHitABall = false;
	ChangePlayer = false;

}



// inital player setup
void table::SetupPlayers(void)
{
	for (int i = 0; i < NUM_PLAYERS; i++)
	{
		players.push_back(new Player());
	}
	if (CurrentPlayerCount == 0) // Player Number One
	{
		NextPlayerCount = 1;
		players[CurrentPlayerCount]->Reset();
		players[NextPlayerCount]->Reset();
		players[CurrentPlayerCount]->PlayerShotsLeft = 1;
	}
	else if (CurrentPlayerCount == 1) // Player Number Two
	{
		NextPlayerCount = 0;
		players[CurrentPlayerCount]->Reset();
		players[NextPlayerCount]->Reset();
		players[CurrentPlayerCount]->PlayerShotsLeft = 1;
	}
	ChangePlayer = false;
}