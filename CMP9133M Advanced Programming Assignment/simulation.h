
/*-----------------------------------------------------------
				Simulation Header File

Computer Science Msc ( Advanced Programming )
Student: Dean Beckerton (BEC12375372)
Module: CMP9133M
  -----------------------------------------------------------*/
#include"vecmath.h"
#include <vector>

/*-----------------------------------------------------------
				DEFINE IN-GAME VARIABLES
  -----------------------------------------------------------*/
#define TABLE_X			(0.6f) 
#define TABLE_Z			(1.2f)
#define TABLE_Y			(0.1f)
#define BALL_RADIUS		(0.05f)
#define BALL_MASS		(0.1f)
#define TWO_PI			(6.2832f)
#define	SIM_UPDATE_MS	(10)
#define NUM_BALLS		(16)		
#define NUM_CUSHIONS	(6)	
#define NUM_CUSHIONSECTIONS (6)	
#define MAX_PARTICLES	(200)
#define NUMBER_OF_POCKETS   (6)
#define NUM_PLAYERS (2)
#define POCKET_SIZE (0.09f)

/*-----------------------------------------------------------
					POCKET HEADER 
-----------------------------------------------------------*/
class Pockets
{
public:
	vec3 <float> position; // Pocket Postion on Table
	float radius;		   // Pocket Size (Radius)
	Pockets(void);		   // Constructor
	~Pockets(void);        // Deconstructor
};

/*-----------------------------------------------------------
				 CUSHION HEADER
-----------------------------------------------------------*/
class cushion
{
public:
	class CushionSection // Nested
	{
	public:
		void MakeNormal(void);	 //	Initilise Variable Cushion Surface
		void MakeCentre(void);   // Initilise Variable Cushion Centre

		vec3<float>	vertices[4]; // Cushion 3D Rectangle
		vec3<float>	centre;		 // Centre of Cushion Section 
		vec3<float>	normal;		 // Cushion Surface

		CushionSection(void);    // Constructor
		~CushionSection(void);	 // Deconstructor
	};

	// Creates Set Amount of Cushions Defined
	CushionSection cushionFaces[NUM_CUSHIONSECTIONS];

	// Instantiation functions.
	cushion(void);
	~cushion(void);

	// Set Cushion Postions for Table
	void SetupCushions(const vec3<float> &posA, const vec3<float> &posB);
};

/*-----------------------------------------------------------
				PLAYER HEADER
-----------------------------------------------------------*/

class Player
{
public:
	float PlayerShotsLeft;			// Number of Shots left for Player Variable 
	int  BallColourValue;		    // Assign Value to Each Ball Variable
	bool HasBlackBeenPotted;		// Has Player Potted Black Ball (Yes/No)
	bool HasPlayerHitABall;			// Has PLayer Hit any Ball (Yes/No)

	Player(void);					// Constructor
	~Player(void);					// Deconstructor
	void Reset(void);				// Reset Player Function
};

/*-----------------------------------------------------------
				BALL HEADER
  -----------------------------------------------------------*/

class ball
{
	static int ballIndexCnt;	// Balls Counter
public:
	vec3<float>	position;	    // Balls Postion (Table)		
	vec3<float>	velocity;		// Balls Velocity (Table)
	float radius;               // Ball Radius
	float mass;					// Balls Mass
	int	CountBall;				// Ball Counter Check
	bool HasbeenPocketed;		// Has Ball Been Pocketeded (Yes/No)
	enum Colour { White, Black, Blue, Yellow };	// Set of Ball Colours
	Colour colour;				// Colour Set Variable

	ball(void);					// Constructor
	~ball(void);				// Deconstructor

	// Main Ball Functions
	void ApplyImpulse(const vec3<float> &imp);	// Calculate Balls Impulse
	void ApplyFrictionForce(float ms);			// Calculate Balls Friction	
	void Update(float ms);						// Update Instance of Ball
	void Reset(void);							// Reset Instance of Ball
	
	// Cushion Check Collision, If so Provide Responce
	void CheckCollision_Cushion(const cushion::CushionSection &cushion);		// Check for Collision
	bool CollisionCheck_Cushion(const cushion::CushionSection &cushion) const;  // Has there Been Collision (Yes/No)
	void Collision_Response_Cusion(const cushion::CushionSection &cushion);		// Respond to Collision

	// Ball Check Collision, if so Provide Collision Responce
	void DoBallCollision(ball &b);								// Check for Collision
	bool HasHitBall(const ball &b) const;						// Has there Been Collision (Yes/No)
	void HitBall(ball &b);										// Respond to Collision
	
	// Pocket Check Collision, If so Provide Responce			
	void CheckCollisions_Pocket(Pockets &pocket);				// Check for Collision
	bool HasHitPocket(const Pockets &pocket) const;				// Has there Been Collision (Yes/No)
	void Hit_Pocket();											// Respond to Collision
};

/*-----------------------------------------------------------
					PARTICLE HEADER
-----------------------------------------------------------*/

class particle
{
public:
	vec3<float> position;						// Particle Postion (Table)
	vec3<float> velocity;						// Particle Velocity (Table)
	particle() { position = 0; velocity = 0; }	// Intilise Particle (Position, Velocity) 
	void Update(float ms);						// Update Paricles
};

class particleSet	//Nested
{
public:
	particle *particles[MAX_PARTICLES];	// Max Particles
	int num;							// Calculate Particles

	// Constructor
	particleSet()
	{
		for (int i = 0; i<MAX_PARTICLES; i++) particles[i] = 0;
		num = 0;
	}
	
	// Deconstructor 
	~particleSet()
	{
		for (int i = 0; i<MAX_PARTICLES; i++)
		{
			if (particles[i]) delete particles[i];
		}
	}

	// Public Particle Variables
	void AddParticle(const vec3<float> &pos);
	void Update(float ms);
};


/*-----------------------------------------------------------
					TABLE HEADER
-----------------------------------------------------------*/
class table
{
public:
	ball balls[NUM_BALLS];					// Initilise Number of Balls
	cushion cushions[NUM_CUSHIONS];			// Initilise Cushion Numbers					
	Pockets pockets[NUMBER_OF_POCKETS];		// Initilise Number of Pockets
	particleSet parts;						// Intilise Paricles
	
	float numBlueBalls;				// Number of Blue Balls (Table)
	float numYellowBalls;			// Number of Yellow Balls (Table)
	bool CheckWhiteBall;			// Check If White Ball On Table				
	bool CheckBlackBall;			// Check If Black Ball On Table		
	int CurrentPlayerCount;			// Current Player Variable
	int NextPlayerCount;			// Change Player Variable
	int ExtraTurn;					// Allow Extra Turn Variable
	bool HasplayerUsedTurn;			// Check if Player has used the Cue
	bool ChangePlayer;				// Check if need to Change Player
	int Winner;						// Winner Player Variable
	std::vector<Player*> players;	// Template Class

	table(void);					// Constructor
	~table(void);					// Deconstructor

	// Functions
	void Update(float ms);			// Check State of Table
	bool AnyBallsMoving(void);		// Check if Any Balls Are Moving
	void SetupBallColours(void);	// Set Colours of Balls
	void CountAllBalls(void);		// Counts Both Blue and Yellow Balls
	void SetupCushions(void);		// Set Cushions to Table
	void SetupPockets(void);		// Set Pockets to Table
	void SetupPlayers(void);		// Set Players to Table
	void SwitchPlayer(void);		// Swap Players on Table
	void Reset(void);
};

/*-----------------------------------------------------------
  global table
  -----------------------------------------------------------*/
extern table gTable;
