/*-----------------------------------------------------------
Pocket Class

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
POCKET CLASS
-----------------------------------------------------------*/

Pockets::Pockets(void)
{
	position = vec3<float>(0.0f, 0.0f, 555.0f); //Pocket Initial Position
	radius = POCKET_SIZE; // Pockets in-game Size
}

Pockets::~Pockets(void)
{
	// Deconstructor
}


// Check for Collision with Pocket
void ball::CheckCollisions_Pocket(Pockets &pocket)
{
	if (HasHitPocket(pocket))
	{
		Hit_Pocket();							// Responce for Hitting Pocket
	}
}


// Check Collision of Ball with Pocket
bool ball::HasHitPocket(const Pockets &poc) const
{
	// Check Boundary of Pocket
	if (position(0) >= poc.position(0) - poc.radius &&
		position(0) <= poc.position(0) + poc.radius &&
		position(2) >= poc.position(2) - poc.radius &&
		position(2) <= poc.position(2) + poc.radius)
	{
		return true;	// Ball has Gone into Pocket Area
	}
	return false; // Ball has not gone into Pocket Area
}


// Sets up Pockets
void table::SetupPockets(void)
{
	// Setup Positions of All Pockets
	pockets[0].position = cushions[0].cushionFaces[0].vertices[0] + vec3<float>(0.0f, 0.01f, -gTable.pockets[0].radius);
	pockets[1].position = cushions[1].cushionFaces[0].vertices[0] + vec3<float>(0.0f, 0.01f, -gTable.pockets[1].radius);
	pockets[2].position = cushions[2].cushionFaces[0].vertices[0] + vec3<float>(-gTable.pockets[2].radius, 0.01f, 0.0f);
	pockets[3].position = cushions[3].cushionFaces[0].vertices[0] + vec3<float>(0.0f, 0.01f, gTable.pockets[3].radius);
	pockets[4].position = cushions[4].cushionFaces[0].vertices[0] + vec3<float>(0.0f, 0.01f, gTable.pockets[4].radius);
	pockets[5].position = cushions[5].cushionFaces[0].vertices[0] + vec3<float>(gTable.pockets[5].radius, 0.01f, 0.0f);
}


// Ball Has Hit a Pocket on Table
void ball::Hit_Pocket()
{

	// White Ball Has Been Pocketed
	if (colour == 0)
	{
		std::cout << "White Ball Has Been Pocketed" << std::endl;
		gTable.CheckWhiteBall = true;
		velocity = vec3<float>(0.0f, 0.0f, 0.0f);							// Velocity of Ball = 0
		position = vec3<float>(500.0f, 0.0f, 0.0f);							// Sets Position off Table
		gTable.ExtraTurn = 1;
		gTable.ChangePlayer = true;
	}
	// Black Ball Has Been Pocketed
	if (colour == 1)
	{
		std::cout << "Black Ball Has Been Pocketed" << std::endl;
		HasbeenPocketed = true;
		velocity = vec3<float>(0.0f, 0.0f, 0.0f);							// Velocity of Ball = 0
		position = vec3<float>(500.0f, 0.0f, 0.0f);							// Sets Position off Table
		gTable.CheckBlackBall = true;
		gTable.players[gTable.CurrentPlayerCount]->HasBlackBeenPotted = true;


		// If the current players got none of their colour balls left.
		if ((gTable.players[gTable.CurrentPlayerCount]->BallColourValue == 2 && gTable.numBlueBalls == 0) ||
			(gTable.players[gTable.CurrentPlayerCount]->BallColourValue == 3 && gTable.numYellowBalls == 0))
		{
			std::cout << "You WON!!!!!" << std::endl;
			gTable.Winner = gTable.CurrentPlayerCount;		// Win Game
		}
		else
		{
			// Still has Balls of their Colour on Table
			std::cout << "You Pocketed Black Ball when you still had Balls of your Colour, You Lose!" << std::endl;
			gTable.Winner = gTable.NextPlayerCount;			// Breaks Rule, Other Player Wins
			gTable.ExtraTurn = 1;
			gTable.ChangePlayer = true;
		}
	}

	// If Pocket either Yellow, Blue
	if (colour == ball::Blue || colour == ball::Yellow)
	{
		HasbeenPocketed = true;
		velocity = vec3<float>(0.0f, 0.0f, 0.0f);							// Velocity 0
		position = vec3<float>(500.0f, 0.0f, 0.0f);							// OFF SCREEN

																			// First Colour Ball Blue Been Pocketed, Assign Player a Colour
		if (gTable.players[gTable.CurrentPlayerCount]->BallColourValue == 0)
		{
			if (colour == ball::Blue)	// Set Blue
			{
				gTable.players[gTable.CurrentPlayerCount]->BallColourValue = 2;	 // Sets Blue Only
				for (int i = 0; i < NUM_PLAYERS; i++)	// Alternate Player is Other Colour
				{
					if (i != gTable.CurrentPlayerCount)
					{
						gTable.players[i]->BallColourValue = 3; //Set other Colour
					}
				}
			}

			// First Colour Ball Yellow Been Pocketed, Assign Player a Colour
			if (colour == ball::Yellow)
			{
				gTable.players[gTable.CurrentPlayerCount]->BallColourValue = 3;  // Sets Yellow Only
				for (int i = 0; i < NUM_PLAYERS; i++)	// Alternate Player is Other Colour
				{
					if (i != gTable.CurrentPlayerCount)
					{
						gTable.players[i]->BallColourValue = 2;
					}
				}
			}
		}

		// If Player Pockets Correct Colour
		if (gTable.players[gTable.CurrentPlayerCount]->BallColourValue == colour)
		{
			// The ball was one of the current players.
			gTable.players[gTable.CurrentPlayerCount]->PlayerShotsLeft = 1;		// Allow Player Another Shot
		}

		// If Player Pockets Wrong Colour
		if ((gTable.players[gTable.CurrentPlayerCount]->BallColourValue == 2 || gTable.players[gTable.CurrentPlayerCount]->BallColourValue == 3) &&
			gTable.players[gTable.CurrentPlayerCount]->BallColourValue != colour)
		{
			if (!gTable.CheckWhiteBall)			// Change Player
			{
				gTable.ExtraTurn = 1;
				gTable.ChangePlayer = true;
			}
		}
	}
}