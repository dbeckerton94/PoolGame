/*-----------------------------------------------------------
  Simulation Source File

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

// In Game Variables
#define SMALL_VELOCITY		(0.3f) // Ball Stop Velocity
table gTable;

// Rack Positions for Ball Postions
static const float gRackPositionX[] = {0.0f,0.0f,(BALL_RADIUS*2.0f),(-BALL_RADIUS*2.0f),(BALL_RADIUS*4.0f)}; 
static const float gRackPositionZ[] = {0.5f,0.0f,(-BALL_RADIUS*3.0f),(-BALL_RADIUS*3.0f)}; 

// Ball Properties
float gCoeffRestitution = 0.5f;
float gCoeffFriction = 0.03f;
float gGravityAccn = 9.8f;


/*-----------------------------------------------------------
			CUSHION CLASS
  -----------------------------------------------------------*/

cushion::CushionSection::CushionSection(void)
{
	for (int i = 0; i < 4; i++)
	{
		vertices[i] = vec3<float>(0.0f, 0.0f, 0.0f);
	}
	centre = vec3<float>(0.0f, 0.0f, 0.0f);
	normal = vec3<float>(0.0f, 0.0f, 0.0f);
}
cushion::CushionSection::~CushionSection(void)
{

}
void cushion::CushionSection::MakeNormal(void)
{
	//can do this in 2d
	vec3<float> temp = vertices[2] - vertices[1];
	vec3<float> temp2 = vertices[0] - vertices[1];
	normal = temp.Cross(temp2);
	normal.Normalise();
}

void cushion::CushionSection::MakeCentre(void)
{
	centre = vertices[0];
	centre += vertices[1];
	centre /= 2.0;
}

// Cushion Class Definition.
cushion::cushion(void)
{
	for (int i = 0; i < NUM_CUSHIONSECTIONS; i++)
	{
		cushionFaces[i] = CushionSection();
	}
}
cushion::~cushion(void)
{

}

void cushion::SetupCushions(const vec3<float> &posA, const vec3<float> &posB)
{
	vec3<float> positionA_YAxis = posA + vec3<float>(0.0f, TABLE_Y, 0.0f);
	vec3<float> positionB_YAxis = posB + vec3<float>(0.0f, TABLE_Y, 0.0f);
	// Interior Face (Used for Collisions).
	cushionFaces[0].vertices[0] = posA;
	cushionFaces[0].vertices[1] = positionA_YAxis;
	cushionFaces[0].vertices[2] = positionB_YAxis;
	cushionFaces[0].vertices[3] = posB;
	cushionFaces[0].MakeNormal();

	// Set up the other faces for the 3D.
	float sizeModifier = 10;
	vec3<float> offset = vec3<float>(cushionFaces[0].normal(0) / sizeModifier, cushionFaces[0].normal(1) / sizeModifier, cushionFaces[0].normal(2) / sizeModifier);
	// Exterior Face.
	cushionFaces[1].vertices[0] = posA - offset;
	cushionFaces[1].vertices[1] = posB - offset;
	cushionFaces[1].vertices[2] = positionB_YAxis - offset;
	cushionFaces[1].vertices[3] = positionA_YAxis - offset;
	// Top Face.
	cushionFaces[2].vertices[0] = positionA_YAxis;
	cushionFaces[2].vertices[1] = positionA_YAxis - offset;
	cushionFaces[2].vertices[2] = positionB_YAxis - offset;
	cushionFaces[2].vertices[3] = positionB_YAxis;
	// Bottom Face.
	cushionFaces[3].vertices[0] = posA;
	cushionFaces[3].vertices[1] = posB;
	cushionFaces[3].vertices[2] = posB - offset;
	cushionFaces[3].vertices[3] = posA - offset;
	// Front Face.
	cushionFaces[4].vertices[0] = posB;
	cushionFaces[4].vertices[1] = positionB_YAxis;
	cushionFaces[4].vertices[2] = positionB_YAxis - offset;
	cushionFaces[4].vertices[3] = posB - offset;
	// Back Face.
	cushionFaces[5].vertices[0] = posA;
	cushionFaces[5].vertices[1] = posA - offset;
	cushionFaces[5].vertices[2] = positionA_YAxis - offset;
	cushionFaces[5].vertices[3] = positionA_YAxis;

	// Calculates each CushionFace's centres.
	for (int j = 0; j < NUM_CUSHIONSECTIONS; j++)
	{
		cushionFaces[j].MakeNormal();
		cushionFaces[j].MakeCentre();
	}
}


/*-----------------------------------------------------------
					BALL CLASS 
-----------------------------------------------------------*/

// Ball Initial Postion/Velocity/Radius/Mass
ball::ball(void) : position(0.0), velocity(0.0), radius(BALL_RADIUS), mass(BALL_MASS) 
{
	CountBall = ballIndexCnt++; // number of balls
	Reset(); // initial instance reset
}

ball::~ball(void)
{
	// Deconstructor
}

//Initilise Ball
int ball::ballIndexCnt = 0;

// Impulse Of Ball
void ball::ApplyImpulse(const vec3<float> &imp)
{
	if (CountBall == 0)	// If White Ball	
	{
	 gTable.players[gTable.CurrentPlayerCount]->PlayerShotsLeft--;

		if (!gTable.HasplayerUsedTurn) // Check If Player has used the Cue
		{
			gTable.HasplayerUsedTurn = true;
		}
	}
	velocity = imp;
}

// Friction Of Balls
void ball::ApplyFrictionForce(float ms)
{
	if (velocity.Magnitude() <= 0.0) return;
	{
		return;
	}
	vec3<float> accelaration = -velocity.Normalised();					    //accelaration is opposite to direction of motion
	accelaration *= (gCoeffFriction * gGravityAccn);					    //friction force = constant * mg Ma, so accelaration = force/mass = constant*g
	vec3<float> velocityChange = ((accelaration * ms) / 1000.0f);		    //integrate velocity : find change in velocity
	if (velocityChange.Magnitude() > velocity.Magnitude()) velocity = 0.0;  //cap magnitude of change in velocity to remove integration errors
	else velocity += velocityChange;
}

// Update Ball Velocity/Force
void ball::Update(float ms)
{
	ApplyFrictionForce(ms);									 //apply friction
	position += ((velocity * ms) / 1000.0f); 				 //integrate position
	if (velocity.Magnitude()<SMALL_VELOCITY) velocity = 0.0; //set small velocities to zero
}

// Reset Ball Positions
void ball::Reset(void)
{
	// Resets Balls Variables
	HasbeenPocketed = false;
	velocity = 0.0;

	if (CountBall == 0)		// White Ball Reset
	{
		position = vec3<float>(0.0f, BALL_RADIUS, TABLE_Z / 2);
		return;
	}

	// Resets Rack Positions
	static const float seperation = (BALL_RADIUS * 3.0f);
	static const float rowSeperation = (BALL_RADIUS * 2.5f);
	int row = 1;
	int rowIndex = CountBall;
	while (rowIndex > row)
	{
		rowIndex -= row;
		row++;
	}
	position = vec3<float>((((row - 1) * seperation) / 2.0f) - (seperation * (row - rowIndex)), BALL_RADIUS, -(rowSeperation * (row	)));
}

// Check The Ball for Collision or if it has been Pocketed
void ball::DoBallCollision(ball &b)
{
	if(HasHitBall(b) && !b.HasbeenPocketed)
	{ 
		HitBall(b);
	}
				
}

// Check for Collision with Cushion
void ball::CheckCollision_Cushion(const cushion::CushionSection &cushion)
{
	if (CollisionCheck_Cushion(cushion))			
	{
		Collision_Response_Cusion(cushion);			// Reponse for Hitting Function
	}
}

// Check when Player Hits Ball
bool ball::HasHitBall(const ball &b) const
{
	vec3<float> relPosn = position - b.position; 
	float dist = (float)relPosn.Magnitude(); //relative position of ball from other ball
	vec3<float> relPosnNorm = relPosn.Normalised();  // Distance Between Balls
	vec3<float> relVelocity = velocity - b.velocity; // relative velocity

	//if moving apart, cannot have hit
	if (relVelocity.Dot(relPosnNorm) >= 0.0) return false;
	
	//if distnce is more than sum of radii, have not hit
	if (dist > (radius + b.radius)) return false;
	return true;
}

// Check if Ball has Hit Cushion
bool ball::CollisionCheck_Cushion(const cushion::CushionSection &cushion) const
{
	if (velocity.Dot(cushion.normal) >= 0.0)		// Velocity Moving Away = NO HIT
	{
		return false;
	}

	vec3<float> relPos = position - cushion.centre;
	float sep = relPos.Dot(cushion.normal);
	if (sep > radius)									// Hit Has Occured
	{
		return false;
	}
	return true;
}




// Reponce of Cusion being Hit by Ball
void ball::Collision_Response_Cusion(const cushion::CushionSection &cushion)
{
	// Reverses the velocity component perpendicular to cushion face.  
	float comp = velocity.Dot(cushion.normal) * (1.0f + gCoeffRestitution);
	vec3<float> delta = -(cushion.normal * comp);
	velocity += delta;

	// Position of Particles
	int n = (rand() % 4) + 3;
	vec3<float> pos(position(0), radius, position(2));
	vec3<float> particleOffSet(cushion.normal(0), cushion.normal(1), cushion.normal(2));
	pos += (particleOffSet * radius);
	
	for (int i = 0; i < n; i++)
	{
		gTable.parts.AddParticle(pos); // Create particles.
	}
}


// Players First Ball Hit
void ball::HitBall(ball &b)
{
	// White Ball (Cue Ball) No Colour has been Set Yet
	if (CountBall == 0 && !gTable.players[gTable.CurrentPlayerCount]->HasPlayerHitABall)
	{
		gTable.players[gTable.CurrentPlayerCount]->HasPlayerHitABall = true;  // Player has Hit White Ball

		switch (gTable.players[gTable.CurrentPlayerCount]->BallColourValue)
		{
		case 0:	// White Ball (Cue Ball)
		{
			if (b.colour == ball::Black) // If Black Ball has been Hit First	
			{
				gTable.ExtraTurn = 1;			// Other User Gets Extra Shot (2)
				gTable.ChangePlayer = true;		// Changes Player with Extra Shot
			}
			break;
		}
		
		case 2:	// If the Players Colour is Blue
		{
			if (b.colour == ball::Black) // If Black Ball has been Hit First
			{
				gTable.ExtraTurn = 1;		 // Other User Gets Extra Shot(2)
				gTable.ChangePlayer = true;	 // Changes Player with Extra Shot	
			}
			// If player first Hit wrong Colour (Yellow)
			if (b.colour == ball::Yellow)
			{
				gTable.ExtraTurn = 1;			// Other User Gets Extra Shot (2)
				gTable.ChangePlayer = true;		// Changes Player with Extra Shot
			}
			break;
		}
		case 3:	// If the Players Colour is Yellow
		{
			if (b.colour == ball::Black)	 // If Black Ball has been Hit First
			{
				gTable.ExtraTurn = 1;			// Other User Gets Extra Shot (2)
				gTable.ChangePlayer = true;		// Changes Player with Extra Shot
			}
			// If player first Hit wrong Colour (Blue)
			if (b.colour == ball::Blue)	
			{
				gTable.ExtraTurn = 1;			// Other User Gets Extra Shot (2)
				gTable.ChangePlayer = true;		// Changes Player with Extra Shot
			}
			break;
		}
		}
	}

	//find direction from other ball to this ball
	vec3<float> relDir = (position - b.position).Normalised();

	//split velocities into 2 parts:  one component perpendicular, and one parallel to 
	//the collision plane, for both balls
	//(NB the collision plane is defined by the point of contact and the contact normal)
	float perpV = (float)velocity.Dot(relDir);
	float perpV2 = (float)b.velocity.Dot(relDir);
	
	vec3<float> parallelV = velocity-(relDir*perpV);
	vec3<float> parallelV2 = b.velocity-(relDir*perpV2);
	
	//Calculate new perpendicluar components:
	//v1 = (2*m2 / m1+m2)*u2 + ((m1 - m2)/(m1+m2))*u1;
	//v2 = (2*m1 / m1+m2)*u1 + ((m2 - m1)/(m1+m2))*u2;
	float sumMass = mass + b.mass;
	float perpVNew = (float)((perpV*(mass-b.mass))/sumMass) + (float)((perpV2*(2.0*b.mass))/sumMass);
	float perpVNew2 = (float)((perpV2*(b.mass-mass))/sumMass) + (float)((perpV*(2.0*mass))/sumMass);
	
	//find new velocities by adding unchanged parallel component to new perpendicluar component
	velocity = parallelV + (relDir*perpVNew);
	b.velocity = parallelV2 + (relDir*perpVNew2);

	//make some particles
	int n = (rand()%5)+5;
	vec3<float> pos(position(0),radius, position(1));
	vec3<float> oset(relDir(0),0.0,relDir(1));
	pos+=(oset*radius);
	for(int i=0;i<n;i++)
	{
		gTable.parts.AddParticle(pos);
	}
}


/*-----------------------------------------------------------
					PARTICLE CLASS
  -----------------------------------------------------------*/

// Create Particles at Set Position
void particleSet::AddParticle(const vec3<float> &pos)
{
	if (num >= MAX_PARTICLES) return;
	particles[num] = new particle;
	particles[num]->position = pos;

	particles[num]->velocity(0) = ((rand() % 200) - 100) / 200.0f;
	particles[num]->velocity(2) = ((rand() % 200) - 100) / 200.0f;
	particles[num]->velocity(1) = 2.0f*((rand() % 100) / 100.0f);

	num++;
}

// Calcultate Particles
void particleSet::Update(float ms)
{
	int i = 0;
	while (i<num)
	{
		particles[i]->Update(ms);
		if ((particles[i]->position(1) < 0.0) && (particles[i]->velocity(1)<0.0))
		{
			delete particles[i];
			particles[i] = particles[num - 1];
			num--;
		}
		else i++;
	}
}

// Particle Calculation Dependent upon Velocity and Postion
void particle::Update(float ms)
{
	position += (velocity * ms) / 1000.0f;
	velocity(1) -= (4.0f * ms) / 1000.0f;
}



/*-----------------------------------------------------------
	TABLE CLASS
-----------------------------------------------------------*/

table::table(void)
{
	// Initial Table
	numBlueBalls = 7;			// Number of Blue Balls 
	numYellowBalls = 7;			// Number of Yellow Balls
	CheckWhiteBall = false;		// White Ball no Potted
	CheckBlackBall = false;		// Black Ball no Potted
	CurrentPlayerCount = 0;		// 0 players
	NextPlayerCount = 0;		// 0 turns
	ExtraTurn = 0;				// 0 Extra Turns
	HasplayerUsedTurn = false;	// No player has used Cue
	ChangePlayer = false;		// Player doesnt need to Change
	Winner = -1;			// No Winner

	// Setup Functions to Pool Game File
	SetupBallColours();  // Set Colours for Each Ball
	SetupCushions();	 // Set Cushion Position on Table
	SetupPockets();	     // Set Pocket Positions on Table
	SetupPlayers();		 // Set Starting Player Function
}

table::~table(void)
{
	for (int i = 0; i < players.size(); i++)
	{
		delete players[i]; // Deconstructor
	}
}

void table::Update(float ms)
{
	// Check for collisions for each ball
	for (int i = 0; i < NUM_BALLS; i++)
	{
		// Check collisions between the current ball and another ball.
		for (int j = (i + 1); j < NUM_BALLS; j++)
		{
			balls[i].DoBallCollision(balls[j]);
		}
		// Check collisions between the current ball and the cushionFaces.
		for (int j = 0; j < NUM_CUSHIONS; j++)
		{
			balls[i].CheckCollision_Cushion(cushions[j].cushionFaces[0]);
		}
		// Check collisions between the current ball and the pockets.
		for (int j = 0; j < NUMBER_OF_POCKETS; j++)
		{
			balls[i].CheckCollisions_Pocket(pockets[j]);
		}
	}

	// Update all balls.
	for (int i = 0; i < NUM_BALLS; i++)
	{
		balls[i].Update(ms);
	}
	// Update particles.
	//particleSystem.Update(milliseconds);
}



bool table::AnyBallsMoving(void)
{
	// Loop through each Ball.
	for (int i = 0; i < NUM_BALLS; i++)
	{
		if (balls[i].velocity(0) != 0.0)	// If the x velocity of the ball is not 0.
		{
			return true;
		}
		if (balls[i].velocity(1) != 0.0)	// If the y velocity of the ball is not 0.
		{
			return true;
		}
		if (balls[i].velocity(2) != 0.0)	// If the z velocity of the ball is not 0.
		{
			return true;
		}
	}
	
	// Player has Failed to Hit any Balls
	if (!players[CurrentPlayerCount]->HasPlayerHitABall &&  HasplayerUsedTurn)		
	{
		ExtraTurn = 1;			// Other Player extra Turn
		ChangePlayer = true;	// Changes Player
	}
	
	// Player Has Hit a Ball
	if (players[CurrentPlayerCount]->HasPlayerHitABall &&  HasplayerUsedTurn)		
	{
		players[CurrentPlayerCount]->HasPlayerHitABall = false;
		players[NextPlayerCount]->HasPlayerHitABall = false;
	}


	// Check White Hasnt Been Potted
	if (CheckWhiteBall)		
	{
		balls[0].Reset(); // Reset White Ball to Start Position
	}

	CheckWhiteBall = false;
	HasplayerUsedTurn = false;

	// Check Players Shots Left , If O Change
	if (players[CurrentPlayerCount]->PlayerShotsLeft == 0)	
	{
		ChangePlayer = true;
	}
	if (ChangePlayer) // Changes Player 
	{
		SwitchPlayer();
	}
	return false;
}

// Set Each Ball Colour for Rack Position
void table::SetupBallColours(void)
{
	// Sets each balls colour.
	gTable.balls[0].colour = ball::White;
	gTable.balls[1].colour = ball::Blue;
	gTable.balls[2].colour = ball::Yellow;
	gTable.balls[3].colour = ball::Blue;
	gTable.balls[4].colour = ball::Blue;
	gTable.balls[5].colour = ball::Black;
	gTable.balls[6].colour = ball::Yellow;
	gTable.balls[7].colour = ball::Yellow;
	gTable.balls[8].colour = ball::Blue;
	gTable.balls[9].colour = ball::Yellow;
	gTable.balls[10].colour = ball::Blue;
	gTable.balls[11].colour = ball::Blue;
	gTable.balls[12].colour = ball::Yellow;
	gTable.balls[13].colour = ball::Blue;
	gTable.balls[14].colour = ball::Yellow;
	gTable.balls[15].colour = ball::Yellow;
}

// Checks All Balls on Table
void table::CountAllBalls(void)
{
	float BlueBallsCounter = 0.0f;
	float YellowBallsCounter = 0.0f;
	
	//LOOP
	for (int i = 0; i < NUM_BALLS; i++)
	{   
		if (balls[i].colour == ball::Black && balls[i].HasbeenPocketed)		
		{
			CheckBlackBall = true; // Black Ball
		}
		if (balls[i].colour == ball::Blue && !balls[i].HasbeenPocketed)		
		{
			BlueBallsCounter++; // Count Blue Balls
		}
		if (balls[i].colour == ball::Yellow && !balls[i].HasbeenPocketed)		
		{
			YellowBallsCounter++;	// Count Yellow Balls
		}
	}
	numBlueBalls = BlueBallsCounter;  // Value
	numYellowBalls = YellowBallsCounter; // Value
	
}

//reset table to initial instance
void table::Reset(void)
{
	CheckWhiteBall = false;
	CheckBlackBall = false;
	CurrentPlayerCount = 0;
	NextPlayerCount = 1;
	ExtraTurn = 0;
	HasplayerUsedTurn = false;
	ChangePlayer = false;
	Winner = -1;

	// reset all players
	for (int i = 0; i < NUM_PLAYERS; i++)
	{
		players[i]->Reset();
	}
	// reset all balls
	for (int i = 0; i < NUM_BALLS; i++)
	{
		balls[i].Reset();
	}
	players[CurrentPlayerCount]->PlayerShotsLeft = 1;
}



// Set Cushion Positions on Table
void table::SetupCushions(void)
{
	// Left
	cushions[0].SetupCushions(vec3<float>(-TABLE_X, 0.0f, -TABLE_Z + gTable.pockets[0].radius), vec3<float>(-TABLE_X, 0.0f, 0 - gTable.pockets[0].radius));
	cushions[1].SetupCushions(vec3<float>(-TABLE_X, 0.0f, 0 + gTable.pockets[1].radius), vec3<float>(-TABLE_X, 0.0f, TABLE_Z - gTable.pockets[1].radius));
	// Bottom
	cushions[2].SetupCushions(vec3<float>(-TABLE_X + gTable.pockets[2].radius, 0.0f, TABLE_Z), vec3<float>(TABLE_X - gTable.pockets[2].radius, 0.0f, TABLE_Z));
	// Right
	cushions[3].SetupCushions(vec3<float>(TABLE_X, 0.0f, TABLE_Z - gTable.pockets[3].radius), vec3<float>(TABLE_X, 0.0f, 0 + gTable.pockets[3].radius));
	cushions[4].SetupCushions(vec3<float>(TABLE_X, 0.0f, 0 - gTable.pockets[4].radius), vec3<float>(TABLE_X, 0.0f, -TABLE_Z + gTable.pockets[4].radius));
	// Top table Wall.
	cushions[5].SetupCushions(vec3<float>(TABLE_X - gTable.pockets[5].radius, 0.0f, -TABLE_Z), vec3<float>(-TABLE_X + gTable.pockets[5].radius, 0.0f, -TABLE_Z));
}


//END 