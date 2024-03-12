// Game Produced for Keele University Games Computing Module 2023
// Student ID - 20014189
// Module Code - CSC-30019-2023-SEM1-A
// Musical Credits listed below - all music used within this game is copyright free
// Bop sound - used when the player gets a sphere - https://www.youtube.com/watch?v=oDVIWWexyKo
// General soundtrack - https://www.youtube.com/watch?v=mRN_T6JkH-c


#include "ofApp.h"
#include "ofSoundPlayer.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Initialize variables for menu and game
    isInMenu = true;
    isInHelpScreen = false;
    isLevelStarting = false;
    isGameOver = false;
    isLevel1Complete = false;
    isLevel2Complete = false;
    isLevel3Complete = false;
    isLevel3Starting = false;
    cubePosition.set(0, 0, 0);
    cubeX = 0;
    cubeSpeed = 5.0f;
    groundY = -50;
    ofSetBackgroundColor(0);
    spheresCollected = 0;
    menuMusic.load("music/Shade.mp3"); // the background music for the game, credit details found at top - copyright free
    menuMusic.play();
    lastFlashTime = 0;
    isFlashing = false;
    flashDuration = 0.5;
    flashColor = ofColor(255, 255, 255, 128);
    // Set the camera position
    cam.setPosition(2.80847, 185.694, 421.051);
    cam.lookAt(cubePosition);
    cam.setFov(90);
    cam.disableMouseInput();
    gracePeriodEndTime = ofGetElapsedTimef() + 2.0; // 2 second grace period will allow the player to settle into a level without instantly failing
    spawnSphere();
    spawnTriangles();
    bopSound.load("music/bop.mp3"); //this plays when a player collects a sphere
        bopSound.setMultiPlay(true);
}

//--------------------------------------------------------------
void ofApp::update() {
    if (isInMenu) {
    } else if (isInHelpScreen) {
    } else if (isLevelStarting) {
        if (ofGetElapsedTimef() - levelStartTimer > levelStartTime) {
            isLevelStarting = false;
            gameTimer = ofGetElapsedTimef();
        }
    } else if (!isGameOver) {
        if (!isLevel1Complete) {
            updateLevel1();
        } else if (!isLevel2Complete) {
            updateLevel2();
        } else if (!isLevel3Complete) {
            updateLevel3();
        }
    }
}

//--------------------------------------------------------------

void ofApp::updateLevel1(){
    if (spheresCollected >= 1) {
        isLevel1Complete = true;
        isLevelStarting = true;
        levelStartTimer = ofGetElapsedTimef();
        spheresCollected = 0;
    } else {
        if (ofGetElapsedTimef() - gameTimer > gameDurationLevel1) {
                    isGameOver = true;
                    isLevel1Complete = false;
                } else {
            updateGameLogic();
        }
    }
    if (ofGetElapsedTimef() - lastSphereSpawnTime > sphereLifetimeL1) {
        spawnSphere();
    }
}

//--------------------------------------------------------------

void ofApp::updateLevel2() {
    if (spheresCollected >= 1) {
        isLevel2Complete = true;
        isLevelStarting = true;
        levelStartTimer = ofGetElapsedTimef();
        spheresCollected = 0;
        startLevel3();
    } else {
        if (isGameOver && isLevel2Complete) {
            startLevel3();
        }
        else {
            updateGameLogic();
            for (auto& triangle : triangles) {
                triangle.position += triangle.direction * triangle.speed;
                if (triangle.position.x < -250 || triangle.position.x > 250 ||
                    triangle.position.z < -250 || triangle.position.z > 250) {
                    triangle.direction = -triangle.direction;
                }
            }
            
            if (ofGetElapsedTimef() > gracePeriodEndTime) {
                for (const auto& triangle : triangles) {
                    if (cubePosition.distance(triangle.position) < 20) {
                        isGameOver = true;
                        isLevel3Complete = false;
                        return;
                    }
                }
                
            }
        }}

    if (ofGetElapsedTimef() - lastSphereSpawnTime > sphereLifetimeL2) {
        spawnSphere();
    }
}

//--------------------------------------------------------------

void ofApp::updateLevel3() {
    if (spheresCollected >= 16) {
        isGameOver = true;
        isLevel3Complete = true;
        triangles.clear();
        spawnTriangles();
    } else {
        if (ofGetElapsedTimef() - gameTimer > gameDurationLevel3) {
            isGameOver = true;
            isLevel3Complete = false;
        } else {
            updateGameLogic();
            if (ofGetElapsedTimef() - lastFlashTime > ofRandom(2, 7)) { // Random time between flashes
                isFlashing = true;
                lastFlashTime = ofGetElapsedTimef();
            }
            if (isFlashing && ofGetElapsedTimef() - lastFlashTime > flashDuration) {
                isFlashing = false;
            }

            for (auto& triangle : triangles) {
                triangle.position += triangle.direction * triangle.speed;
                if (triangle.position.x < -250 || triangle.position.x > 250 ||
                    triangle.position.z < -250 || triangle.position.z > 250) {
                    triangle.direction = -triangle.direction;
                }
            }
            if (ofGetElapsedTimef() > gracePeriodEndTime) {
                for (const auto& triangle : triangles) {
                    if (cubePosition.distance(triangle.position) < 20) {
                        isGameOver = true;
                        isLevel3Complete = false;
                        return;
                    }
                }
            }
        }
    }
    if (ofGetElapsedTimef() - lastSphereSpawnTime > sphereLifetimeL3) {
        spawnSphere();
    }
}

//--------------------------------------------------------------
void ofApp::startGame() {
    isInMenu = false;
    isLevelStarting = true;
    levelStartTimer = ofGetElapsedTimef();
    score = 0;
    spheresCollected = 0;
    gameTimer = ofGetElapsedTimef();
    cubePosition.set(0, 0, 0);
    cubeX = 0;
    spawnTriangles();
}

//--------------------------------------------------------------

void ofApp::startLevel2() {
    isLevel2Starting = true;
    levelStartTimer = ofGetElapsedTimef();
    score = 0;
    spheresCollected = 0;
    gameTimer = ofGetElapsedTimef();
    
    spawnTriangles();
    
}

//--------------------------------------------------------------

void ofApp::startLevel3() {
    isLevel3Starting = true;
    levelStartTimer = ofGetElapsedTimef();
    score = 0;
    spheresCollected = 0;
    gameTimer = ofGetElapsedTimef();
    spawnTriangles();
}


//--------------------------------------------------------------

void ofApp::updateGameLogic(){
    if (ofGetKeyPressed('d')) {
        cubeX += cubeSpeed;
    }
    if (ofGetKeyPressed('a')) {
        cubeX -= cubeSpeed;
    }
    cubePosition.z += (ofGetKeyPressed('s') - ofGetKeyPressed('w')) * cubeSpeed;
    cubeX = ofClamp(cubeX, -250, 250);
    cubePosition.z = ofClamp(cubePosition.z, -250, 250);
        if (cubePosition.distance(spherePosition) < 20) {
            score++;
            spheresCollected++;
            bopSound.play();
            spawnSphere();
        }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(isInMenu) {
        drawMenu();
    } else if(isInHelpScreen) {
        drawHelpScreen();
    } else if(isLevelStarting) {
        drawLevelStart();
    } else if (!isGameOver) {
        gameDraw();
    } else {
        drawGameOver();
    }
}

//--------------------------------------------------------------
void ofApp::drawLevelStart(){
    ofSetColor(255);
    if (!isLevel1Complete) {
        ofDrawBitmapString("Level 1", ofGetWidth() / 2, ofGetHeight() / 2);
        ofDrawBitmapString("_______", ofGetWidth() / 2, ofGetHeight() / 2);
        ofDrawBitmapString("Catch 12 Spheres in 20 seconds", ofGetWidth() / 2, ofGetHeight() / 2 + 25);
        resetPos();
    } else if (!isLevel2Complete) {
        ofDrawBitmapString("Level 2", ofGetWidth() / 2, ofGetHeight() / 2);
        ofDrawBitmapString("_______", ofGetWidth() / 2, ofGetHeight() / 2);
        ofDrawBitmapString("Catch 14 Spheres in 30 seconds", ofGetWidth() / 2, ofGetHeight() / 2 + 25);
        ofDrawBitmapString("Avoid the blue cones!", ofGetWidth() / 2, ofGetHeight() / 2 + +45);
        resetPos();
    } else {
        ofDrawBitmapString("Level 3", ofGetWidth() / 2, ofGetHeight() / 2);
        ofDrawBitmapString("_______", ofGetWidth() / 2, ofGetHeight() / 2);
        ofDrawBitmapString("Catch 16 Spheres in 30 seconds", ofGetWidth() / 2, ofGetHeight() / 2 + 25);
        ofDrawBitmapString("Avoid the blue cones", ofGetWidth() / 2, ofGetHeight() / 2 + +45);
        ofDrawBitmapString("dont get distracted by the light!", ofGetWidth() / 2, ofGetHeight() / 2 + +65);
    }
}
//--------------------------------------------------------------
void ofApp::drawGameOver() {
    ofSetColor(255);
    if (isLevel3Complete) {
        ofDrawBitmapString("GAME COMPLETED!", ofGetWidth() / 2, ofGetHeight() / 2);
        ofDrawBitmapString("Press ENTER to return to main menu", ofGetWidth() / 2, ofGetHeight() / 2 + 20);
    } else if (isLevel2Complete && !isLevel3Complete) {
        ofDrawBitmapString("Level 3 Failed", ofGetWidth() / 2, ofGetHeight() / 2);
        ofDrawBitmapString("Press ENTER to return to main menu", ofGetWidth() / 2, ofGetHeight() / 2 + 20);
    } else if (isLevel1Complete && !isLevel2Complete) {
        ofDrawBitmapString("Level 2 Failed", ofGetWidth() / 2, ofGetHeight() / 2);
        ofDrawBitmapString("Press ENTER to return to main menu", ofGetWidth() / 2, ofGetHeight() / 2 + 20);
    } else {
        ofDrawBitmapString("Level 1 Failed", ofGetWidth() / 2, ofGetHeight() / 2);
        ofDrawBitmapString("Press ENTER to return to main menu", ofGetWidth() / 2, ofGetHeight() / 2 + 20);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(isInMenu) {
        if(key == ' ') {
            startGame();
        } else if(key == 'h') {
            isInHelpScreen = true;
            isInMenu = false;
        }
    } else if(isInHelpScreen) {
        if(key == OF_KEY_BACKSPACE) {
            isInHelpScreen = false;
            isInMenu = true;
        }
    } else if (isGameOver) {
        if(key == OF_KEY_RETURN) {
            resetGameState();
            isInMenu = true;
            isGameOver = false;
        }
    }
    
    if (key == '2') {
        if (!isTopDownView) {
            cam.setPosition(0, 300, 0);
            cam.lookAt(ofVec3f(0, 0, 0));
            isTopDownView = true;
        } else {
            cam.setPosition(2.80847, 185.694, 421.051);
            cam.lookAt(cubePosition);
            isTopDownView = false;
        }
    }
}

//--------------------------------------------------------------

void ofApp::resetGameState() { //this gets called at the end of gameplay once level 3 is completed
    isLevel1Complete = false;
    isLevel2Complete = false;
    isLevel3Complete = false;
    isLevelStarting = false;
    isGameOver = false;
    isLevel3Starting = false;

    cubePosition.set(0, 0, 0);
    cubeX = 0;
    score = 0;
    spheresCollected = 0;
    isFlashing = false;


}

//--------------------------------------------------------------

void ofApp::resetPos() { //this gets used in-between the levels being completed (during gameplay)
    if (isLevel1Complete && !isLevel2Complete) {
        for (auto& triangle : triangles) {
            triangle.position.y = cubePosition.y;
        }
    } else {
        for (auto& triangle : triangles) {
            triangle.position.y = 1000;
        }
    }
    cubePosition.set(0, 0, 0);
    cubeX = 0;
    score = 0;
    spheresCollected = 0;
    isFlashing = false;
    
}

//--------------------------------------------------------------
void ofApp::gameDraw() {
    cam.begin();
    ofSetColor(255);
    ofDrawGrid(500, 10, false, false, true, false);

    cubePosition.x = cubeX;
    ofSetColor(255, 0, 0);
    ofDrawBox(cubePosition, 20);

    ofSetColor(128, 0, 128);
        ofDrawSphere(spherePosition, 10);

    ofSetColor(0, 0, 255);
        for (const auto& triangle : triangles) {
            float coneRadius = triangle.baseWidth / 2.0f;
            float coneHeight = triangle.height;
            ofDrawCone(triangle.position.x, triangle.position.y, triangle.position.z, coneRadius, coneHeight);
        }
    
    // Temporarily switch to 2D mode for drawing the flash effect - this will stop it looking funny if it was to flash in 3D
        ofPushMatrix();
        ofSetMatrixMode(OF_MATRIX_PROJECTION);
        ofLoadIdentityMatrix();
        glOrtho(0, ofGetWidth(), ofGetHeight(), 0, -1, 1);
        ofSetMatrixMode(OF_MATRIX_MODELVIEW);
        ofLoadIdentityMatrix();

        if (isFlashing) {
            ofSetColor(flashColor);
            ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        }
        ofPopMatrix();

    cam.end();
    
    float timeLeft;
    if (!isLevel1Complete) {
        timeLeft = std::max(0.0f, gameDurationLevel1 - (ofGetElapsedTimef() - gameTimer));
    } else {
        timeLeft = std::max(0.0f, gameDurationLevel2 - (ofGetElapsedTimef() - gameTimer));
    }

    ofSetColor(255, 255, 255);
    ofDrawBitmapString("Time Left: " + ofToString(timeLeft, 2), 10, 20);
    ofDrawBitmapString("Spheres Collected: " + ofToString(spheresCollected), 10, 60);
}

//--------------------------------------------------------------

void ofApp::drawMenu(){
    ofSetColor(255);
    ofDrawBitmapString("Sphere Dash - A Prototype Game", ofGetWidth() / 2, ofGetHeight() / 2);
    ofDrawBitmapString("______________________________", ofGetWidth() / 2, ofGetHeight() / 2);
    ofDrawBitmapString("Options:", ofGetWidth() / 2, ofGetHeight() / 2 + 20);
    ofDrawBitmapString("Press space to start", ofGetWidth() / 2, ofGetHeight() / 2 + 40);
    ofDrawBitmapString("Press 'H' for help", ofGetWidth() / 2, ofGetHeight() / 2 + 60);
}

//--------------------------------------------------------------

void ofApp::drawHelpScreen(){
    ofSetColor(255);
    ofDrawBitmapString("Help Screen", ofGetWidth() / 2, ofGetHeight() / 2);
    ofDrawBitmapString("___________", ofGetWidth() / 2, ofGetHeight() / 2);
    ofDrawBitmapString("Instructions:\n- Use 'W,A,S,D' to move\n- Press '2' to toggle the camera angle\n- Avoid obstacles and collect spheres\n- Win by collecting spheres!", ofGetWidth() / 2, ofGetHeight() / 2 + 20);
    ofDrawBitmapString("Press BACKSPACE to return", ofGetWidth() / 2, ofGetHeight() / 2 + 100);
}

//--------------------------------------------------------------

void ofApp::spawnSphere(){
    float x = ofRandom(-250, 250);
    float z = ofRandom(-250, 250);
    float y = cubePosition.y;
    spherePosition.set(x, y, z);
    lastSphereSpawnTime = ofGetElapsedTimef();
}

//--------------------------------------------------------------

void ofApp::spawnTriangles() {
    triangles.clear();

    int numberOfTriangles = 3;
   
    for (int i = 0; i < numberOfTriangles; ++i) {
        TriangleObstacle triangle;
        triangle.position.set(ofRandom(-250, 250), cubePosition.y, ofRandom(-250, 250));
        triangle.speed = 3.0;
        triangle.direction.set(ofRandom(-1, 1), 0, ofRandom(-1, 1));
        triangle.direction.normalize();
        triangle.baseWidth = 30.0;
        triangle.height = 50.0;
        triangles.push_back(triangle);
    }
}
