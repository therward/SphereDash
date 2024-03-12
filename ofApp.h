// Game Produced for Keele University Games Computing Module 2023
// Student ID - 20014189
// Module Code - CSC-30019-2023-SEM1-A
// Musical Credits listed below - all music used within this game is copyright free
// Bop sound - used when the player gets a sphere - https://www.youtube.com/watch?v=oDVIWWexyKo
// General soundtrack - https://www.youtube.com/watch?v=mRN_T6JkH-c


#pragma once

#include "ofMain.h"
#include "ofSoundPlayer.h" // this will be used for the general soundtrack of the game as found at

// Structure for the moving cones (in levels 2 and 3)
struct TriangleObstacle {
    ofVec3f position;
    float speed;
    ofVec3f direction;
    float baseWidth;
    float height;
};

class ofApp : public ofBaseApp {
public:
    
    // Core methods
    void setup();
    void update();
    void draw();
    void keyPressed(int key);

    // Menu
    void drawMenu();
    void drawHelpScreen();
    bool isInMenu;
    bool isInHelpScreen;

    // General Game
    void startGame();
    void gameUpdate();
    void gameDraw();
    void drawLevelStart();
    void drawGameOver();
    void resetGameState();

    // Levels
    void updateLevel1();
    void updateLevel2();
    void updateLevel3();
    void startLevel2();
    void startLevel3();
    bool isLevelStarting;
    bool isGameOver;
    float levelStartTimer;
    float gameTimer;
    const float levelStartTime = 3.0f;
    bool isLevel1Complete;
    bool isLevel2Starting;
    bool isLevel2Complete;
    bool isLevel3Starting;
    bool isLevel3Complete;
    const float gameDurationLevel1 = 20.0f; // Duration for Level 1
    const float gameDurationLevel2 = 30.0f; // Duration for Level 2
    const float gameDurationLevel3 = 30.0f; // Duration for Level 3

    // Player and camera management
    ofEasyCam cam;
    ofVec3f cubePosition;
    float cubeX;
    float cubeSpeed;
    bool isJumping;
    float jumpStartTime;
    float gravity;
    float groundY;
    int score;
    int spheresCollected;
    void updateGameLogic();
    void resetPos();
    bool isTopDownView;

    // Sphere management
    ofVec3f spherePosition; 
    void spawnSphere();
    float lastSphereSpawnTime;
    const float sphereLifetimeL1 = 4.0;
    const float sphereLifetimeL2 = 3.0;
    const float sphereLifetimeL3 = 2.0;

    // Triangle
    std::vector<TriangleObstacle> triangles;
    void spawnTriangles();

    // Flash effect variables - used in level 3
    float lastFlashTime;
    bool isFlashing;
    float flashDuration;
    ofColor flashColor;
    float gracePeriodEndTime; // Grace period for collision detection (prevents players instantly failing a level on spawn)

    // Sounds (credits listed above)
    ofSoundPlayer menuMusic;
    ofSoundPlayer bopSound;
};
