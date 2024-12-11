// This is the main file for the game logic and function
#include "game.h"
#include "Golem.h"
#include "Slime.h"
#include "Guard.h"
#include "Hunter.h"
#include "Entity.h"
#include "Framework\console.h"
#include "Inventory.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <typeinfo>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#pragma comment(lib, "winmm.lib")

double  g_dDeltaTime, g_dGameTime, timerTrap, enemyMeleeAttackTimer, golemRadiusAttackTimer, moveTimer, dialogueTimer, dialogueDelay, enemyHurtPlayerTimer, trapHurtPlayerTimer, golemRadiusAttackrenderTimer;

SKeyEvent g_skKeyEvent[K_COUNT];
SMouseEvent g_mouseEvent;

// Game specific variables here
SGameChar   g_sChar;
Slime slimes;
Inventory inventory;
EGAMESTATES g_eGameState = S_SPLASHSCREEN; // initial state

int damagetaken = 0, kills = 0;
unsigned level_no = 1, itemsCount = 0, charCount = 0, cutscene_no = 1;
bool E_KeyPressed = false, onDialogue = false, updatedDialogueTimer = false, golemDefeated = false, golemIsAttacking = false, keyObtained = false;
bool lv3_StartingDialogueShown = false, lv3_BootDialogueShown = false, lv4_StartingDialogueShown = false, lv4_GolemDefeatDialogueShown = false, lv5_EntryDialogueShown = false, lv5_NoKeyDialogueShown = false;
bool usingSword = false, usingChestplate = false, usingBoot = false, itemClicked = false;
bool SWORD_AND_CHESTPLATE_GIVEN = false, bootGiven = false, chestOpened = false;
bool playerTookDamageFromEnemy = false, playerTookDamageFromTrap = false;
bool cutscene_music = false, main_music = false, end_music = false;

std::string dialogue = "", renderingDialogue = "";

// Console object
Console g_Console(120, 50, "Temple Escape");
unsigned GUI_height = 10;

// Arrays
std::vector<std::vector<std::string>> mapVector;       // Map array for all maps
std::vector<std::vector<std::string>> inventoryVector; // Inventory array
Entity* enemies[10] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };


// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init(void) {
    srand((unsigned int)time(NULL));
    // Set precision for floating point output
    g_dGameTime = 0.0, timerTrap = 0.0, enemyMeleeAttackTimer = 0.0, golemRadiusAttackTimer = 0.0, moveTimer = 0.0, dialogueTimer = 0.0, dialogueDelay = 0.0, enemyHurtPlayerTimer = 0.0, golemRadiusAttackrenderTimer = 0.0;
    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;
    //change values to change where player spawns
    g_sChar.m_cLocation.X = 10; g_sChar.m_cLocation.Y = 37;
    g_sChar.hp = 10; g_sChar.dmg = 1; g_sChar.def = 0; g_sChar.weight = 2;
    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 16, L"Consolas");
    // remember to set your keyboard handler, so that your functions can be notified of input events
    g_Console.setKeyboardHandler(keyboardHandler);
    g_Console.setMouseHandler(mouseHandler);
}

//--------------------------------------------------------------
// Purpose  : Reset before exiting the program
//            Do your clean up of memory here
//            This is called once just before the game exits
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void shutdown(void)
{
    // Reset to white text on black background
    colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
    g_Console.clearBuffer();
}

//--------------------------------------------------------------
// Purpose  : Get all the console input events
//            This function sets up the keyboard and mouse input from the console.
//            We will need to reset all the keyboard status, because no events will be sent if no keys are pressed.
//
//            Remember to set the handlers for keyboard and mouse events.
//            The function prototype of the handler is a function that takes in a const reference to the event struct
//            and returns nothing. 
//            void pfKeyboardHandler(const KEY_EVENT_RECORD&);
//            void pfMouseHandlerconst MOUSE_EVENT_RECORD&);
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void getInput(void)
{
    // resets all the keyboard events
    memset(g_skKeyEvent, 0, K_COUNT * sizeof(*g_skKeyEvent));
    // then call the console to detect input from user
    g_Console.readConsoleInput();
}

//--------------------------------------------------------------
// Purpose  : This is the handler for the keyboard input. Whenever there is a keyboard event, this function will be called.
//            Ideally, you should pass the key event to a specific function to handle the event.
//            This is because in some states, some keys would be disabled. Hence, to reduce your code complexity, 
//            it would be wise to split your keyboard input handlers separately.
//            
//            The KEY_EVENT_RECORD struct has more attributes that you can use, if you are adventurous enough.
//
//            In this case, we are not handling any keyboard event in the Splashscreen state
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent - reference to a key event struct
// Output   : void
//--------------------------------------------------------------
void keyboardHandler(const KEY_EVENT_RECORD& keyboardEvent)
{
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: gameplayKBHandler(keyboardEvent); break; // don't handle anything for the splash screen
    case S_GAME: gameplayKBHandler(keyboardEvent); break; // handle gameplay keyboard event
    case S_END: gameplayKBHandler(keyboardEvent); break;
    }
}

//--------------------------------------------------------------
// Purpose  : This is the handler for the mouse input. Whenever there is a mouse event, this function will be called.
//            Ideally, you should pass the key event to a specific function to handle the event.
//            This is because in some states, some keys would be disabled. Hence, to reduce your code complexity, 
//            it would be wise to split your keyboard input handlers separately.
//            
//            For the mouse event, if the mouse is not moved, no event will be sent, hence you should not reset the mouse status.
//            However, if the mouse goes out of the window, you would not be able to know either. 
//
//            The MOUSE_EVENT_RECORD struct has more attributes that you can use, if you are adventurous enough.
//
//            In this case, we are not handling any mouse event in the Splashscreen state
//            
// Input    : const MOUSE_EVENT_RECORD& mouseEvent - reference to a mouse event struct
// Output   : void
//--------------------------------------------------------------
void mouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
{
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: break; // don't handle anything for the splash screen
    case S_GAME: gameplayMouseHandler(mouseEvent); break; // handle gameplay mouse event
    case S_END: break;
    }
}

//--------------------------------------------------------------
// Purpose  : This is the keyboard handler in the game state. Whenever there is a keyboard event in the game state, this function will be called.
//            
//            Add more keys to the enum in game.h if you need to detect more keys
//            To get other VK key defines, right click on the VK define (e.g. VK_UP) and choose "Go To Definition" 
//            For Alphanumeric keys, the values are their ascii values (uppercase).
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
void gameplayKBHandler(const KEY_EVENT_RECORD& keyboardEvent)
{
    // here, we map the key to our enums
    EKEYS key = K_COUNT;
    switch (keyboardEvent.wVirtualKeyCode)
    {
    case 0x57: key = K_UP; break;
    case 0x53: key = K_DOWN; break;
    case 0x41: key = K_LEFT; break;
    case 0x44: key = K_RIGHT; break;
    case 0x45: key = K_INVENTORY; break;
    case 0x46: key = K_INTERACTIVE; break;
    case VK_RETURN: key = K_ENTER; break;
    case VK_ESCAPE: key = K_ESCAPE; break;
    case VK_UP: key = K_ATK_UP; break;
    case VK_DOWN: key = K_ATK_DOWN; break;
    case VK_LEFT: key = K_ATK_LEFT; break;
    case VK_RIGHT: key = K_ATK_RIGHT; break;
    }
    // a key pressed event would be one with bKeyDown == true
    // a key released event would be one with bKeyDown == false
    // if no key is pressed, no event would be fired.
    // so we are tracking if a key is either pressed, or released
    if (key != K_COUNT)
    {
        g_skKeyEvent[key].keyDown = keyboardEvent.bKeyDown;
        g_skKeyEvent[key].keyReleased = !keyboardEvent.bKeyDown;
    }
}

//--------------------------------------------------------------
// Purpose  : This is the mouse handler in the game state. Whenever there is a mouse event in the game state, this function will be called.
//            
//            If mouse clicks are detected, the corresponding bit for that mouse button will be set.
//            mouse wheel, 
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
void gameplayMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
{
    if (mouseEvent.dwEventFlags & MOUSE_MOVED) // update the mouse position if there are no events
    {
        g_mouseEvent.mousePosition = mouseEvent.dwMousePosition;
    }
    g_mouseEvent.buttonState = mouseEvent.dwButtonState;
    g_mouseEvent.eventFlags = mouseEvent.dwEventFlags;
}

//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function
//            double dt - This is the amount of time in seconds since the previous call was made
//
//            Game logic should be done here.
//            Such as collision checks, determining the position of your game characters, status updates, etc
//            If there are any calls to write to the console here, then you are doing it wrong.
//
//            If your game has multiple states, you should determine the current state, and call the relevant function here.
//
// Input    : dt = deltatime
// Output   : void
//--------------------------------------------------------------
void update(double dt)
{
    // get the delta time
    g_dDeltaTime = dt;
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN:
        if (cutscene_music == false) {
            PlaySound(L"Cutscenes.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
            cutscene_music = true;
        }
        if (cutscene_no <= 3) { initCSVector(cutscene_no); if (g_skKeyEvent[K_ENTER].keyReleased) { cutscene_no++; } }
        else {
            splashScreenWait();
        } break; // game logic for the splash screen
    case S_GAME:
        if (main_music == false) {
            PlaySound(L"Main.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
            main_music = true;
        }
        updateGame(dt); break; // gameplay logic when we are in the game
    case S_END:
        if (end_music == false) {
            if (g_sChar.hp <= 0) { //lose
                PlaySound(L"LoseScene.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
            }
            else { //win
                PlaySound(L"VictoryScene.wav", NULL, SND_FILENAME | SND_ASYNC);
            }
            end_music = true;
        }
        endScreenWait(); break;
    }
}

void splashScreenWait()    // waits for time to pass in splash screen
{
    if (g_skKeyEvent[K_ENTER].keyReleased) { // wait for user to press enter to switch to game mode, else do nothing
        g_eGameState = S_GAME;
        initMapVector();
        createEnemies();
        initInventoryVector();
        changeDialogue(1);
    }
    processUserInput();
}

void updateGame(double dt)                          // gameplay logic
{
    processUserInput();                             // checks if you should change states or do something else with the game, e.g. pause, exit
    if (E_KeyPressed == false)
    {
        g_dGameTime += dt;

        if (onDialogue == false)
        {
            g_sChar.dir = 'N';
            playerAttack();                         //will add atk speed later
            moveTimer += dt;
            if (moveTimer >= 0.5) { enemyMovement(); moveTimer = 0.0; }
            if (playerTookDamageFromEnemy == true) { enemyHurtPlayerTimer += dt; }
            if (playerTookDamageFromTrap == true) { trapHurtPlayerTimer += dt; }
            enemyMeleeAttackTimer += dt;
            if (enemyMeleeAttackTimer >= 0.5) { enemyMeleeAttack();  enemyMeleeAttackTimer = 0.0; }
            golemRadiusAttackTimer += dt;
            if (golemRadiusAttackTimer >= 3) { golemRadiusAttack(); golemIsAttacking = true; golemRadiusAttackTimer = 0.0; }
            if (golemIsAttacking == true) { golemRadiusAttackrenderTimer += dt; }
            timerTrap += dt;
            if (timerTrap >= 0.3) { TouchSpikeTrap(); timerTrap = 0.0; }
        }
    }
    checkPosition();                                // checks whether player's next position is an exit, a chest, 
    if (g_sChar.hp <= 0) { g_eGameState = S_END; }                       // check if player dead
    if (onDialogue == false)
    {
        for (int i = 0; i < 10; i++) {
            if (enemies[i] != nullptr) {
                if (enemies[i]->get_hp() <= 0) {    // check if enemy dead
                    delete enemies[i];
                    enemies[i] = nullptr;
                    kills++;
                }
            }
        }
    }
    if ((onDialogue == true) && (updatedDialogueTimer == false)) { updatedDialogueTimer = true; dialogueTimer = g_dGameTime; }
    if ((level_no == 4) && (enemies[0] == nullptr)) { golemDefeated = true; }
    // moves the character, collision detection, physics, etc + update player's stats + update player's inventory + check for mouse click
    if (onDialogue == false) { keyPressed(); updateStats(); updateInventory(); mouseClicked(); }
}

void endScreenWait()
{
    processUserInput();
}

void keyPressed()
{
    // Updating the location of the character based on the key released
    if (E_KeyPressed == false)
    {
        if ((g_skKeyEvent[K_UP].keyReleased) && (g_sChar.m_cLocation.Y > 0) && ((mapVector[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X / 2] == "0") || (mapVector[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X / 2] == "7") || (mapVector[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X / 2] == "8") || (mapVector[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X / 2] == "9")))
        {
            g_sChar.m_cLocation.Y--;
        }
        if ((g_skKeyEvent[K_LEFT].keyReleased) && (g_sChar.m_cLocation.X > 1) && ((mapVector[g_sChar.m_cLocation.Y][(g_sChar.m_cLocation.X / 2) - 1] == "0") || (mapVector[g_sChar.m_cLocation.Y][(g_sChar.m_cLocation.X / 2) - 1] == "7") || (mapVector[g_sChar.m_cLocation.Y][(g_sChar.m_cLocation.X / 2) - 1] == "8") || (mapVector[g_sChar.m_cLocation.Y][(g_sChar.m_cLocation.X / 2) - 1] == "9")))
        {
            g_sChar.m_cLocation.X -= 2;
        }
        if ((g_skKeyEvent[K_DOWN].keyReleased) && (g_sChar.m_cLocation.Y < (g_Console.getConsoleSize().Y - 1 - GUI_height)) && ((mapVector[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X / 2] == "0") || (mapVector[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X / 2] == "7") || (mapVector[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X / 2] == "8") || (mapVector[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X / 2] == "9")))
        {
            g_sChar.m_cLocation.Y++;
        }
        if ((g_skKeyEvent[K_RIGHT].keyReleased) && (g_sChar.m_cLocation.X < (g_Console.getConsoleSize().X - 2)) && ((mapVector[g_sChar.m_cLocation.Y][(g_sChar.m_cLocation.X / 2) + 1] == "0") || (mapVector[g_sChar.m_cLocation.Y][(g_sChar.m_cLocation.X / 2) + 1] == "7") || (mapVector[g_sChar.m_cLocation.Y][(g_sChar.m_cLocation.X / 2) + 1] == "8") || (mapVector[g_sChar.m_cLocation.Y][(g_sChar.m_cLocation.X / 2) + 1] == "9")))
        {
            g_sChar.m_cLocation.X += 2;
        }
        if (g_skKeyEvent[K_INTERACTIVE].keyReleased) // we don't want player to spam
        {
            if ((mapVector[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X / 2] == "3") || (mapVector[g_sChar.m_cLocation.Y][(g_sChar.m_cLocation.X / 2) - 1] == "3") || (mapVector[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X / 2] == "3") || (mapVector[g_sChar.m_cLocation.Y][(g_sChar.m_cLocation.X / 2) + 1] == "3")) { if (bootGiven == false) { bootGiven = true; changeDialogue(4); inventory.AddInGameItem("Boot"); } }
            if ((mapVector[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X / 2] == "4") || (mapVector[g_sChar.m_cLocation.Y][(g_sChar.m_cLocation.X / 2) - 1] == "4") || (mapVector[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X / 2] == "4") || (mapVector[g_sChar.m_cLocation.Y][(g_sChar.m_cLocation.X / 2) + 1] == "4")) { if (chestOpened == false) { chestOpened = true; changeDialogue(13); inventory.AddInGameItem("Potion"); } }
            if ((mapVector[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X / 2] == "5") || (mapVector[g_sChar.m_cLocation.Y][(g_sChar.m_cLocation.X / 2) - 1] == "5") || (mapVector[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X / 2] == "5") || (mapVector[g_sChar.m_cLocation.Y][(g_sChar.m_cLocation.X / 2) + 1] == "5")) { changeDialogue(2); }
            if ((mapVector[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X / 2] == "6") || (mapVector[g_sChar.m_cLocation.Y][(g_sChar.m_cLocation.X / 2) - 1] == "6") || (mapVector[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X / 2] == "6") || (mapVector[g_sChar.m_cLocation.Y][(g_sChar.m_cLocation.X / 2) + 1] == "6")) { if (SWORD_AND_CHESTPLATE_GIVEN == false) { SWORD_AND_CHESTPLATE_GIVEN = true; changeDialogue(3); inventory.AddInGameItem("Sword"); inventory.AddInGameItem("Chestplate"); } }
            if ((mapVector[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X / 2] == "10") || (mapVector[g_sChar.m_cLocation.Y][(g_sChar.m_cLocation.X / 2) - 1] == "10") || (mapVector[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X / 2] == "10") || (mapVector[g_sChar.m_cLocation.Y][(g_sChar.m_cLocation.X / 2) + 1] == "10")) { if (keyObtained == false) { keyObtained = true; changeDialogue(14); inventory.AddInGameItem("Key"); } }
        }
    }
    if (g_skKeyEvent[K_INVENTORY].keyReleased) // we don't want player to spam
    {
        if (E_KeyPressed == false) { E_KeyPressed = true; }
        else { E_KeyPressed = false; }
    }
}

void playerAttack()
{
if (g_skKeyEvent[K_ATK_UP].keyReleased)
{
    for (int i = 0; i < 10; i++) {
        if (enemies[i] != nullptr) {
            if (enemies[i]->get_posX() == g_sChar.m_cLocation.X && enemies[i]->get_posY() == g_sChar.m_cLocation.Y - 1) {
                enemies[i]->set_hp(enemies[i]->get_hp() - (g_sChar.dmg - enemies[i]->get_def()));
            }
        }
    }
    g_sChar.dir = 'U';
}
if (g_skKeyEvent[K_ATK_DOWN].keyReleased)
{
    for (int i = 0; i < 10; i++) {
        if (enemies[i] != nullptr) {
            if (enemies[i]->get_posX() == g_sChar.m_cLocation.X && enemies[i]->get_posY() == g_sChar.m_cLocation.Y + 1) {
                enemies[i]->set_hp(enemies[i]->get_hp() - (g_sChar.dmg - enemies[i]->get_def()));
            }
        }
    }
    g_sChar.dir = 'D';
}
if (g_skKeyEvent[K_ATK_LEFT].keyReleased)
{
    for (int i = 0; i < 10; i++) {
        if (enemies[i] != nullptr) {
            if (enemies[i]->get_posX() == g_sChar.m_cLocation.X - 2 && enemies[i]->get_posY() == g_sChar.m_cLocation.Y) {
                enemies[i]->set_hp(enemies[i]->get_hp() - (g_sChar.dmg - enemies[i]->get_def()));
            }
        }
    }
    g_sChar.dir = 'L';
}
if (g_skKeyEvent[K_ATK_RIGHT].keyReleased)
{
    for (int i = 0; i < 10; i++) {
        if (enemies[i] != nullptr) {
            if (enemies[i]->get_posX() == g_sChar.m_cLocation.X + 2 && enemies[i]->get_posY() == g_sChar.m_cLocation.Y) {
                enemies[i]->set_hp(enemies[i]->get_hp() - (g_sChar.dmg - enemies[i]->get_def()));
            }
        }
    }
    g_sChar.dir = 'R';
}
}

void processUserInput()
{
    if (g_skKeyEvent[K_ESCAPE].keyReleased) { g_bQuitGame = true; } // quits the game if player hits the escape key
}

void checkPosition()
{
    if ((level_no == 3) && (lv3_StartingDialogueShown == false) && (g_sChar.m_cLocation.Y > 20)) { lv3_StartingDialogueShown = true; changeDialogue(6); }
    if ((level_no == 3) && (lv3_BootDialogueShown == false) && (g_sChar.m_cLocation.Y > 20) && (onDialogue == false) && (usingBoot == true)) { lv3_BootDialogueShown = true; changeDialogue(7); }
    if ((level_no == 4) && (lv4_GolemDefeatDialogueShown == false) && (golemDefeated == true)) { lv4_GolemDefeatDialogueShown = true; changeDialogue(9); }
    if ((level_no == 5) && (lv5_EntryDialogueShown == false) && (mapVector[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X / 2] == "7")) { lv5_EntryDialogueShown = true; changeDialogue(11); }
    if ((level_no == 5) && (lv5_EntryDialogueShown == true) && (lv5_NoKeyDialogueShown == false) && (keyObtained == false) && (mapVector[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X / 2] == "8")) { lv5_NoKeyDialogueShown = true; changeDialogue(12); }
    if ((level_no == 5) && (lv5_EntryDialogueShown == true) && (lv5_NoKeyDialogueShown == false) && (keyObtained == false) && (mapVector[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X / 2] == "8")) { lv5_NoKeyDialogueShown = true; changeDialogue(12); }

    if (mapVector[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X / 2] == "7") {

        if ((level_no == 4 && golemDefeated == true) || (level_no < 5 && level_no != 4)) { deleteEnemies(); mapVector.clear(); level_no++; initMapVector(); createEnemies(); }

        switch (level_no)
        {
        case 2: g_sChar.m_cLocation.X = 8; g_sChar.m_cLocation.Y = 36; chestOpened = false; break;
        case 3: g_sChar.m_cLocation.X = 2; g_sChar.m_cLocation.Y = 2; chestOpened = false; changeDialogue(5); break;
        case 4: g_sChar.m_cLocation.X = 2; g_sChar.m_cLocation.Y = 20; chestOpened = false; if (lv4_StartingDialogueShown == false) { lv4_StartingDialogueShown = true; changeDialogue(8); } break;
        case 5: g_sChar.m_cLocation.X = 2; g_sChar.m_cLocation.Y = 21; chestOpened = false; if (lv5_EntryDialogueShown == false) { changeDialogue(10); } break;
        }
    }

    if ((lv5_EntryDialogueShown == true) && (keyObtained == true) && (mapVector[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X / 2] == "8")) { g_eGameState = S_END; } // Ending scene
}

void updateStats()
{
    if (usingSword == true) { g_sChar.dmg = 3; }
    else if (usingSword == false) { g_sChar.dmg = 1; }

    if (usingChestplate == true) { g_sChar.def = 1; }
    else if (usingChestplate == false) { g_sChar.def = 0; }

    if (usingBoot == true) { g_sChar.weight = 1; }
    else if (usingBoot == false) { g_sChar.weight = 2; }
}

void mouseClicked()
{
    if ((E_KeyPressed == true) && (g_mouseEvent.eventFlags == 0) && (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED))
    {
        if (((g_mouseEvent.mousePosition.Y - 8) > 1) && (((g_mouseEvent.mousePosition.X / 2) - 9) > 1) && ((g_mouseEvent.mousePosition.Y - 8) < 23) && (((g_mouseEvent.mousePosition.X / 2) - 9) < 41))
        {
            if ((inventoryVector[g_mouseEvent.mousePosition.Y - 8][(g_mouseEvent.mousePosition.X / 2) - 9] == "11") || (inventoryVector[g_mouseEvent.mousePosition.Y - 8][(g_mouseEvent.mousePosition.X / 2) - 9] == "12")) { if (usingSword == false) { usingSword = true; } else { usingSword = false; } }
            else if (inventoryVector[g_mouseEvent.mousePosition.Y - 8][(g_mouseEvent.mousePosition.X / 2) - 9] == "13") { if (usingChestplate == false) { usingChestplate = true; } else { usingChestplate = false; } }
            else if (inventoryVector[g_mouseEvent.mousePosition.Y - 8][(g_mouseEvent.mousePosition.X / 2) - 9] == "14") { if (usingBoot == false) { usingBoot = true; } else { usingBoot = false; } }
            else if (inventoryVector[g_mouseEvent.mousePosition.Y - 8][(g_mouseEvent.mousePosition.X / 2) - 9] == "15")
            {
                inventory.RemoveItem("Potion");
                itemsCount = 0;
                inventoryVector.clear(); initInventoryVector();
                if ((g_sChar.hp + 2) <= 10) { g_sChar.hp += 2; }
                else if ((g_sChar.hp + 2) > 10) { g_sChar.hp = 10; }
            }
        }
    }
}

//--------------------------------------------------------------
// Purpose  : Render function is to update the console screen
//            At this point, you should know exactly what to draw onto the screen.
//            Just draw it!
//            To get an idea of the values for colours, look at console.h and the URL listed there
// Input    : void
// Output   : void
//--------------------------------------------------------------
void render() {
    clearScreen();      // clears the current screen and draw from scratch 
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN:
        if (cutscene_no <= 3) { renderCS(); }
        else { renderSplashScreen(); }; break;
    case S_GAME: renderFramerate(); renderGame(); break;
    case S_END: renderEndScreen(); break;
    }
    //renderInputEvents();    // renders status of input events
    renderToScreen();       // dump the contents of the buffer to the screen, one frame worth of game
}

void clearScreen() {
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(0xF0);
}

void initCSVector(int CSno)
{
    std::ifstream CSvector; // We dont need to move it to the top as we are not using it from other functions, this frees up memory after this function is ran
    mapVector.clear();
    switch (CSno)
    {
    case 1: CSvector = std::ifstream("Cutscene1.csv"); break;
    case 2: CSvector = std::ifstream("Cutscene2.csv"); break;
    case 3: CSvector = std::ifstream("Cutscene3.csv"); break;
    }
    std::string row;
    while (std::getline(CSvector, row))
    {
        std::stringstream rowStream(row);
        std::string(cell);
        std::vector<std::string> rowVector;
        while (std::getline(rowStream, cell, ',')) { rowVector.push_back(cell); }
        mapVector.push_back(rowVector);
    }
    CSvector.close();
}

void renderCS() {
    for (unsigned y = 0; y < 40; y++)
    {
        for (unsigned x = 0; x < 60; x++)
        {
            WORD map_colour = 0x00;
            switch (stoi(mapVector[y][x]))
            {
            case 0: map_colour = 0x00; break;
            case 1: map_colour = 0x10; break;
            case 2: map_colour = 0x20; break;
            case 4: map_colour = 0x40; break;
            case 5: map_colour = 0x50; break;
            case 6: map_colour = 0x60; break;
            case 8: map_colour = 0x80; break;
            case 10: map_colour = 0xA0; break;
            case 14: map_colour = 0xE0; break;
            case 15: map_colour = 0xF0; break;
            }
            g_Console.writeToBuffer(x * 2, y, "  ", map_colour);
            switch (cutscene_no) {
            case 1:
                g_Console.writeToBuffer(0, 40, "Villager 1: It's so bright! What's going on?", 0xF1);
                g_Console.writeToBuffer(0, 41, "Villager 2: It's coming from the temple!", 0xFD);
                g_Console.writeToBuffer(0, 42, "John: I'm going over to check it out!", 0xF4);
                g_Console.writeToBuffer(0, 43, "Villager 1: I hope nothing bad will happen...", 0xF1);
                g_Console.writeToBuffer(0, 44, "Villager 2: Be careful...", 0xFD);
                break;
            case 2:
                g_Console.writeToBuffer(0, 40, "tzzt... GROWL!!! zzzt... < crickets and wild beasts >", 0xF0);
                break;
            case 3:
                g_Console.writeToBuffer(0, 40, "John: Wow, the temple's doors are open. It's never happened before. I wonder what I can find in there!", 0xF4);
                break;
            }
        }
    }
}

void menuScreen(COORD& c)
{
    std::ostringstream title;
    title << char(244) << "  " << char(205) << char(205) << char(203) << char(205) << char(205) << " " << char(201) << char(205) << char(205) << char(205) << char(205) << " " << char(201) << char(205) << char(205) << char(203) << char(205) << char(205) << char(187) << " " << char(201) << char(205) << char(205) << char(205) << char(187) << " " << char(210) << "     " << char(201) << char(205) << char(205) << char(205) << char(205) << "  " << char(244);
    g_Console.writeToBuffer(c, title.str(), 0xF0); title.str(""); c.Y += 1;
    title << char(245) << "    " << char(186) << "   " << char(186) << "     " << char(186) << "  " << char(208) << "  " << char(186) << " " << char(186) << "   " << char(186) << " " << char(186) << "     " << char(186) << "      " << char(245);
    g_Console.writeToBuffer(c, title.str(), 0xF0); title.str(""); c.Y += 1;
    title << char(244) << "    " << char(186) << "   " << char(204) << char(205) << char(205) << char(205) << char(205) << " " << char(186) << "     " << char(186) << " " << char(204) << char(205) << char(205) << char(205) << char(188) << " " << char(186) << "     " << char(204) << char(205) << char(205) << char(205) << char(205) << "  " << char(244);
    g_Console.writeToBuffer(c, title.str(), 0xF0); title.str(""); c.Y += 1;
    title << char(245) << "    " << char(186) << "   " << char(186) << "     " << char(186) << "     " << char(186) << " " << char(186) << "     " << char(186) << "     " << char(186) << "      " << char(245);
    g_Console.writeToBuffer(c, title.str(), 0xF0); title.str(""); c.Y += 1;
    title << char(244) << "    " << char(208) << "   " << char(200) << char(205) << char(205) << char(205) << char(205) << " " << char(208) << "     " << char(208) << " " << char(208) << "     " << char(200) << char(205) << char(205) << char(205) << char(181) << " " << char(200) << char(205) << char(205) << char(205) << char(205) << "  " << char(244);
    g_Console.writeToBuffer(c, title.str(), 0xF0); title.str(""); c.Y += 1;
    title << char(245) << "   " << char(201) << char(205) << char(205) << char(205) << char(205) << " " << char(201) << char(205) << char(205) << char(205) << char(181) << " " << char(201) << char(205) << char(205) << char(205) << char(181) << " " << char(201) << char(205) << char(205) << char(205) << char(187) << " " << char(201) << char(205) << char(205) << char(205) << char(187) << " " << char(201) << char(205) << char(205) << char(205) << char(205) << "   " << char(245);
    g_Console.writeToBuffer(c, title.str(), 0xF0); title.str(""); c.Y += 1;
    title << char(244) << "   " << char(186) << "     " << char(186) << "     " << char(186) << "     " << char(186) << "   " << char(186) << " " << char(186) << "   " << char(186) << " " << char(186) << "       " << char(244);
    g_Console.writeToBuffer(c, title.str(), 0xF0); title.str(""); c.Y += 1;
    title << char(245) << "   " << char(204) << char(205) << char(205) << char(205) << char(205) << " " << char(200) << char(205) << char(205) << char(205) << char(187) << " " << char(186) << "     " << char(204) << char(205) << char(205) << char(205) << char(185) << " " << char(204) << char(205) << char(205) << char(205) << char(188) << " " << char(204) << char(205) << char(205) << char(205) << char(205) << "   " << char(245);
    g_Console.writeToBuffer(c, title.str(), 0xF0); title.str(""); c.Y += 1;
    title << char(244) << "   " << char(186) << "         " << char(186) << " " << char(186) << "     " << char(186) << "   " << char(186) << " " << char(186) << "     " << char(186) << "       " << char(244);
    g_Console.writeToBuffer(c, title.str(), 0xF0); title.str(""); c.Y += 1;
    title << char(245) << "   " << char(200) << char(205) << char(205) << char(205) << char(205) << " " << char(198) << char(205) << char(205) << char(205) << char(188) << " " << char(200) << char(205) << char(205) << char(205) << char(181) << " " << char(208) << "   " << char(208) << " " << char(208) << "     " << char(200) << char(205) << char(205) << char(205) << char(205) << "   " << char(245);
    g_Console.writeToBuffer(c, title.str(), 0xF0); title.str("");
}

void renderSplashScreen() {             // renders the splash screen aka menu screen
    COORD c = g_Console.getConsoleSize();
    c.Y = 11; c.X = c.X / 2 - 21;
    menuScreen(c);
    c = g_Console.getConsoleSize();
    c.Y = 27;
    c.X = c.X / 2 - 11;
    g_Console.writeToBuffer(c, "Press 'Enter' to start", 0xF0);
    c.Y += 2;
    c.X = g_Console.getConsoleSize().X / 2 - 16;
    g_Console.writeToBuffer(c, "Press 'W, A, S, D' to move around", 0xF0);
    c.Y++;
    c.X = g_Console.getConsoleSize().X / 2 - 14;
    g_Console.writeToBuffer(c, "Press 'E' to open inventory", 0xF0);
    c.Y++;
    c.X = g_Console.getConsoleSize().X / 2 - 16;
    g_Console.writeToBuffer(c, "Press 'F' to interact with items", 0xF0);
    c.Y++;
    c.X = g_Console.getConsoleSize().X / 2 - 23;
    g_Console.writeToBuffer(c, "Press 'Arrow Keys' to attack in that direction", 0xF0);
    c.Y += 3;
    c.X = g_Console.getConsoleSize().X / 2 - 10;
    g_Console.writeToBuffer(c, "Press 'Esc' to quit", 0xF0);
}

void renderGame() {
    renderMap(); // then renders the map to the buffer first
    renderEnemies();
    if (playerTookDamageFromEnemy == true) { renderHurtCharacter(); if (enemyHurtPlayerTimer >= 0.5) { playerTookDamageFromEnemy = false;  enemyHurtPlayerTimer = 0.0; } } // renders the character into the buffer (over slimes and golems)
    else if (playerTookDamageFromTrap == true) { renderHurtCharacter(); if (trapHurtPlayerTimer >= 0.15) { playerTookDamageFromTrap = false; trapHurtPlayerTimer = 0.0; } }
    else { renderCharacter(); }
    renderPlayerAttack();
    if (golemIsAttacking == true) { rendergolemRadiusAttack(); if (golemRadiusAttackrenderTimer > 2) { golemIsAttacking = false; golemRadiusAttackrenderTimer = 0.0; } }
    if (E_KeyPressed == true) { renderInventory(); } // renders inventory
    if (onDialogue == true) { renderDialogues(); } // renders dialogues
    if (E_KeyPressed == true) { renderItemInfos(); }
}

void initMapVector()
{
    std::ifstream mapCsv; // We dont need to move it to the top as we are not using it from other functions, this frees up memory after this function is ran
    mapVector.clear();
    switch (level_no)
    {
    case 1: mapCsv = std::ifstream("Map1.csv"); break; //opens map
    case 2: mapCsv = std::ifstream("Map2.csv"); break; //opens map
    case 3: mapCsv = std::ifstream("Map3.csv"); break; //opens map
    case 4: mapCsv = std::ifstream("Map4.csv"); break; //opens map
    case 5: mapCsv = std::ifstream("Map5.csv"); break; //opens map
    }
    std::string row;
    while (std::getline(mapCsv, row))
    {
        std::stringstream rowStream(row);
        std::string(cell);
        std::vector<std::string> rowVector;
        while (std::getline(rowStream, cell, ',')) { rowVector.push_back(cell); }
        mapVector.push_back(rowVector);
    }
    mapCsv.close();
}

void renderMap() {
    for (unsigned y = 0; y < 40; y++)
    {
        for (unsigned x = 0; x < 60; x++)
        {
            WORD map_colour = 0x00;
            switch (stoi(mapVector[y][x]))
            {
            case 0: map_colour = 0x60; break; //path
            case 1: map_colour = 0x00; break; //wall            
            case 2: map_colour = 0x50; break; //dead body     
            case 3: map_colour = 0xD0; break; //special dead body  
            case 4: map_colour = 0xE0; break; //chest
            case 5: map_colour = 0x80; break; //fake chest    
            case 6: map_colour = 0x30; break; //prisoner
            case 7: map_colour = 0x10; break; //exit  
            case 8: map_colour = 0x00; break; //real exit for lv 5
            case 9: map_colour = 0xCC; break; //spike trap
            case 10: map_colour = 0xE0; break; //key chest for lv 5
            }
            g_Console.writeToBuffer(x * 2, y, "  ", map_colour);
        }
    }
}

void createEnemies() {  // The creation of slime object MUST be inside a FUNCTION
    switch (level_no)
    {
    case 1:
        for (unsigned i = 0; i < 5; i++) {      //i determines the number of enemies spawning
            enemies[i] = new Slime;
            while (mapVector[enemies[i]->get_posY()][enemies[i]->get_posX() / 2] != "0") { enemies[i]->setPos((2 * (rand() % 60)), rand() % 40); }
        } break;
    case 2:
        for (unsigned i = 0; i < 10; i++) {
            if (i < 5) { enemies[i] = new Slime; }
            else { enemies[i] = new Guard; }

            while (mapVector[enemies[i]->get_posY()][enemies[i]->get_posX() / 2] != "0") { enemies[i]->setPos((2 * (rand() % 60)), rand() % 40); }
        } break;
    case 3:
        for (unsigned i = 0; i < 10; i++) {
            if (i < 2) { enemies[i] = new Slime; }
            else if (i < 6) { enemies[i] = new Guard; }
            else { enemies[i] = new Hunter; }

            while (mapVector[enemies[i]->get_posY()][enemies[i]->get_posX() / 2] != "0") { enemies[i]->setPos((2 * (rand() % 60)), rand() % 40); }
        } break;
    case 4:
        enemies[0] = new Golem;
        while (mapVector[enemies[0]->get_posY()][enemies[0]->get_posX() / 2] != "0") { enemies[0]->setPos(60, 19); } break;
    }
}

void deleteEnemies() {
    for (unsigned i = 0; i < 10; i++) {
        if (enemies[i] != nullptr) {
            delete enemies[i];
            enemies[i] = nullptr;
        }
    }
}

void renderEnemies() {
    for (unsigned i = 0; i < 10; i++) {
        if (enemies[i] != nullptr)
        {
            g_Console.writeToBuffer(enemies[i]->get_posX(), enemies[i]->get_posY(), enemies[i]->get_face(), enemies[i]->get_colour());
        }
    }
}

void renderCharacter() {
    // Draw the location of the character
    std::string playerChar = "^^";
    g_Console.writeToBuffer(g_sChar.m_cLocation, playerChar, 0xF0);
}
void renderHurtCharacter() {
    std::string playerChar = "^^";
    g_Console.writeToBuffer(g_sChar.m_cLocation, playerChar, 0x40);
}

void renderPlayerAttack() {
    switch (g_sChar.dir) {
    case 'U':
        g_Console.writeToBuffer(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y - 1, "/\\", 0xF0);
        break;
    case 'D':
        g_Console.writeToBuffer(g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y + 1, "\\/", 0xF0);
        break;
    case 'L':
        g_Console.writeToBuffer(g_sChar.m_cLocation.X - 2, g_sChar.m_cLocation.Y, "<<", 0xF0);
        break;
    case 'R':
        g_Console.writeToBuffer(g_sChar.m_cLocation.X + 2, g_sChar.m_cLocation.Y, ">>", 0xF0);
        break;
    }
}
void rendergolemRadiusAttack() {
    if (level_no == 4) {
        if (enemies[0] != nullptr) {
            g_Console.writeToBuffer(enemies[0]->get_posX() + 2, enemies[0]->get_posY(), "!!", 0x64);
            g_Console.writeToBuffer(enemies[0]->get_posX() - 2, enemies[0]->get_posY(), "!!", 0x64);
            g_Console.writeToBuffer(enemies[0]->get_posX(), enemies[0]->get_posY() + 1, "!!", 0x64);
            g_Console.writeToBuffer(enemies[0]->get_posX(), enemies[0]->get_posY() - 1, "!!", 0x64);
            g_Console.writeToBuffer(enemies[0]->get_posX() + 2, enemies[0]->get_posY() + 1, "!!", 0x64);
            g_Console.writeToBuffer(enemies[0]->get_posX() - 2, enemies[0]->get_posY() - 1, "!!", 0x64);
            g_Console.writeToBuffer(enemies[0]->get_posX() + 2, enemies[0]->get_posY() - 1, "!!", 0x64);
            g_Console.writeToBuffer(enemies[0]->get_posX() - 2, enemies[0]->get_posY() + 1, "!!", 0x64);
        }
    }
}
void changeDialogue(unsigned num)
{
    dialogue = "";

    switch (num)
    {
        // First room (sees slimes)
    case 1: dialogue = "Slimes! My punches are quite weak... Maybe there's a weapon lying around here?"; break;
        // First room (fake chest)
    case 2: dialogue = "Hmm... There's a looted chest here. Looks like I should keep an eye out for more chests around this area."; break;
        // First room (prisoner)
    case 3: dialogue = "Hey kid, I don't have much time left, the others have all died too. Here, take this sword and chestplate, they may be   old, but they're better than nothing. Promise me you'll survive."; break;
        // Second room (special dead body)
    case 4: dialogue = "These boots look useful... I feel lighter. Thank you Ol'man!"; break;
        // Third room (at the start of the room)
    case 5: dialogue = "Hmm... Wounds suffered from sharp objects... Could there be spikes ahead? Anyway, I should be careful."; break;
        // Third room (1/4 in room)
    case 6: dialogue = "So many spike traps... and slimes!"; break;
        // Third room (1/4 in room if boots equipped)
    case 7: dialogue = "Hopefully the boots I put on just now can be of some benefit to me."; break;
        // Fourth room (at the start of the room)
    case 8: dialogue = "This creature looks tougher compared to those slimes.. A boss?"; break;
        // Fourth room (boss defeated)
    case 9: dialogue = "IT'S FINALLY DEAD!!!"; break;
        // Fifth room (at the start of the room)
    case 10: dialogue = "Finally... the treasure!!! With this, my village is saved! There won't be any more traps right?"; break;
        // Fifth room (touches fake exit)
    case 11: dialogue = "Wait, how do I leave????"; break;
        // Fifth room (touches real exit without key)
    case 12: dialogue = "You need a key."; break;
        // Opening chest (for potion)
    case 13: dialogue = "A potion? That seems handy."; break;
        // Opening chest (for lv 5 key)
    case 14: dialogue = "Golden key obtained."; break;
    }

    onDialogue = true;
}

void renderDialogues()
{
    g_Console.writeToBuffer(0, 48, renderingDialogue, 0x1F);

    if (g_dGameTime >= (dialogueTimer + dialogueDelay))
    {
        if (dialogue != "")
        {
            if (charCount != dialogue.length())
            {
                renderingDialogue += dialogue[charCount];
                charCount++;
                dialogueDelay += 0.05;
            }

            if (charCount == dialogue.length()) { charCount = 0; dialogue = ""; }
        }
    }

    // wait for 3 seconds before removing the rendering dialogue
    if ((charCount == 0) && (g_dGameTime >= (dialogueTimer + dialogueDelay + 3)))
    {
        renderingDialogue = ""; dialogueTimer = 0.0; dialogueDelay = 0.0; onDialogue = false; updatedDialogueTimer = false;
    }
}

void enemyMovement() {
    for (int z = 0; z < 10; z++) {
        if (enemies[z] != nullptr) {
            int direction = rand() % 4;
            if (enemies[z]->get_face() == "++") { // Guard movement
                if (enemies[z]->get_patrol_dir() == 0) { // U D
                    switch (direction) {
                    case 0:
                    case 1: if (mapVector[enemies[z]->get_posY() - 1][enemies[z]->get_posX() / 2] == "0") { enemies[z]->move(direction); } break; // up
                    case 2:
                    case 3: if (mapVector[enemies[z]->get_posY() + 1][enemies[z]->get_posX() / 2] == "0") { enemies[z]->move(direction); } break; // down
                    }
                }
                else if (enemies[z]->get_patrol_dir() == 1) { // L R
                    switch (direction) {
                    case 0:
                    case 1: if (mapVector[enemies[z]->get_posY()][enemies[z]->get_posX() / 2 - 1] == "0") { enemies[z]->move(direction); } break; // left
                    case 2:
                    case 3: if (mapVector[enemies[z]->get_posY()][enemies[z]->get_posX() / 2 + 1] == "0") { enemies[z]->move(direction); } break; // right
                    }
                }
            }
            else {
                if (enemies[z]->get_face() == "..") { // Hunter direction
                    if ((enemies[z]->get_posY() - g_sChar.m_cLocation.Y) > 0 && (enemies[z]->get_posY() - g_sChar.m_cLocation.Y) < 5) { direction = 0; }
                    else if ((enemies[z]->get_posY() - g_sChar.m_cLocation.Y) < 0 && (enemies[z]->get_posY() - g_sChar.m_cLocation.Y) > -5) { direction = 1; }
                    else if ((enemies[z]->get_posX() - g_sChar.m_cLocation.X) > 0 && (enemies[z]->get_posX() - g_sChar.m_cLocation.X) < 10) { direction = 2; }
                    else if ((enemies[z]->get_posX() - g_sChar.m_cLocation.X) < 0 && (enemies[z]->get_posX() - g_sChar.m_cLocation.X) > -10) { direction = 3; }
                }
                switch (direction) {
                case 0: if (mapVector[enemies[z]->get_posY() - 1][enemies[z]->get_posX() / 2] == "0") { enemies[z]->move(direction); } break;
                case 1: if (mapVector[enemies[z]->get_posY() + 1][enemies[z]->get_posX() / 2] == "0") { enemies[z]->move(direction); } break;
                case 2: if (mapVector[enemies[z]->get_posY()][enemies[z]->get_posX() / 2 - 1] == "0") { enemies[z]->move(direction); } break;
                case 3: if (mapVector[enemies[z]->get_posY()][enemies[z]->get_posX() / 2 + 1] == "0") { enemies[z]->move(direction); } break;
                }
            }
        }
    }
}

void enemyMeleeAttack() {
    for (int j = 0; j < 10; j++) {
        if (enemies[j] != nullptr) {
            if ((g_sChar.m_cLocation.X == enemies[j]->get_posX()) && g_sChar.m_cLocation.Y == enemies[j]->get_posY()) {
                g_sChar.hp -= (enemies[j]->get_dmg() - g_sChar.def);
                playerTookDamageFromEnemy = true;
            }
        }
    }
}
void golemRadiusAttack() {
    if (enemies[0] != nullptr && level_no == 4) {
        if ((g_sChar.m_cLocation.X == enemies[0]->get_posX() - 2 && g_sChar.m_cLocation.Y == enemies[0]->get_posY())        // left
            || (g_sChar.m_cLocation.X == enemies[0]->get_posX() + 2 && g_sChar.m_cLocation.Y == enemies[0]->get_posY())     // right
            || (g_sChar.m_cLocation.X == enemies[0]->get_posX() && g_sChar.m_cLocation.Y == enemies[0]->get_posY() - 1)     // up
            || (g_sChar.m_cLocation.X == enemies[0]->get_posX() && g_sChar.m_cLocation.Y == enemies[0]->get_posY() + 1)     //down
            || (g_sChar.m_cLocation.X == enemies[0]->get_posX() - 2 && g_sChar.m_cLocation.Y == enemies[0]->get_posY() - 1) // top left corner
            || (g_sChar.m_cLocation.X == enemies[0]->get_posX() + 2 && g_sChar.m_cLocation.Y == enemies[0]->get_posY() + 1) // bottom right corner
            || (g_sChar.m_cLocation.X == enemies[0]->get_posX() - 2 && g_sChar.m_cLocation.Y == enemies[0]->get_posY() + 1) // bottom left corner
            || (g_sChar.m_cLocation.X == enemies[0]->get_posX() + 2 && g_sChar.m_cLocation.Y == enemies[0]->get_posY() - 1))// top right corner
        {
            g_sChar.hp -= 3; 
        }
    }
}

void TouchSpikeTrap() {
    if ((mapVector[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X / 2] == "9") && g_sChar.weight == 2) {
        g_sChar.hp -= 1;
        playerTookDamageFromTrap = true;
    }
}

void initInventoryVector()
{
    std::ifstream inventoryCsv("Inventory.csv");
    std::string row;

    while (std::getline(inventoryCsv, row))
    {
        std::stringstream rowStream(row);
        std::string(cell);
        std::vector<std::string> rowVector;
        while (std::getline(rowStream, cell, ',')) { rowVector.push_back(cell); }
        inventoryVector.push_back(rowVector);
    }
    inventoryCsv.close();
}

void updateInventoryHealth()
{
    for (unsigned r = 3; r < 8; r++)
    {
        for (unsigned c = 25; c < 31; c++)
        {
            switch (g_sChar.hp)
            {
            case 0: ((c < 29) ? (((c == 26 || c == 27) && (r >= 4 && r <= 6)) ? (inventoryVector[r][c] = "1") : (inventoryVector[r][c] = "0")) : (inventoryVector[r][c] = "1")); break;
            case 1: ((c != 25) ? (inventoryVector[r][c] = "1") : (inventoryVector[r][c] = "0")); break;
            case 2: ((c < 28) ? (((r == 4 && c != 27) || (r == 6 && c != 25)) ? (inventoryVector[r][c] = "1") : (inventoryVector[r][c] = "0")) : (inventoryVector[r][c] = "1")); break;
            case 3: ((c < 28) ? ((((r == 4 || r == 6) && c != 27)) ? (inventoryVector[r][c] = "1") : (inventoryVector[r][c] = "0")) : (inventoryVector[r][c] = "1")); break;
            case 4: ((c < 29) ? ((((c == 25 && r <= 5) || (c == 27) || ((c == 26 || c == 28) && r == 5))) ? (inventoryVector[r][c] = "0") : (inventoryVector[r][c] = "1")) : (inventoryVector[r][c] = "1")); break;
            case 5: ((c < 28) ? (((r == 4 && c != 25) || (r == 6 && c != 27)) ? (inventoryVector[r][c] = "1") : (inventoryVector[r][c] = "0")) : (inventoryVector[r][c] = "1")); break;
            case 6: ((c < 28) ? (((r == 4 && c != 25) || (r == 6 && c == 26)) ? (inventoryVector[r][c] = "1") : (inventoryVector[r][c] = "0")) : (inventoryVector[r][c] = "1")); break;
            case 7: ((c < 28) ? ((r != 3 && c != 27) ? (inventoryVector[r][c] = "1") : (inventoryVector[r][c] = "0")) : (inventoryVector[r][c] = "1")); break;
            case 8: ((c < 28) ? (((r == 4 || r == 6) && c == 26) ? (inventoryVector[r][c] = "1") : (inventoryVector[r][c] = "0")) : (inventoryVector[r][c] = "1")); break;
            case 9: ((c < 28) ? (((r == 4 && c == 26) || (r == 6 && c != 27)) ? (inventoryVector[r][c] = "1") : (inventoryVector[r][c] = "0")) : (inventoryVector[r][c] = "1")); break;
            case 10: ((c != 25) ? (((c == 26) || ((c == 28 || c == 29) && (r >= 4 && r <= 6))) ? (inventoryVector[r][c] = "1") : (inventoryVector[r][c] = "0")) : (inventoryVector[r][c] = "0")); break;
            }
        }
    }
}

void updateInventoryItems()
{
    for (unsigned y = 0; y < 24; y++)
    {
        for (unsigned x = 0; x < 42; x++)
        {
            if ((inventoryVector[y][x] == "9") && (itemsCount < inventory.GetInGameItems().size()))
            {
                for (unsigned j = 0; j < 3; j++) // dimension of each inventory slot
                {
                    for (unsigned i = 0; i < 3; i++)
                    {
                        if (inventory.GetInGameItems()[itemsCount] == "Sword")
                        {
                            ((j == i) ? ((j == 2 && i == 2) ? (inventoryVector[y + j][x + i] = "11") : (inventoryVector[y + j][x + i] = "12")) : (inventoryVector[y + j][x + i] = "1"));
                        }
                        else if (inventory.GetInGameItems()[itemsCount] == "Chestplate")
                        {
                            ((j == 0 && i == 1) ? (inventoryVector[y + j][x + i] = "1") : (inventoryVector[y + j][x + i] = "13"));
                        }
                        else if (inventory.GetInGameItems()[itemsCount] == "Boot")
                        {
                            ((j != 0 && (i == 0 || i == 2)) ? (inventoryVector[y + j][x + i] = "14") : (inventoryVector[y + j][x + i] = "1"));
                        }
                        else if (inventory.GetInGameItems()[itemsCount] == "Potion")
                        {
                            (((j == 0 && i == 1) || (j == 1 || j == 2)) ? (inventoryVector[y + j][x + i] = "15") : (inventoryVector[y + j][x + i] = "1"));
                        }
                        else if (inventory.GetInGameItems()[itemsCount] == "Key")
                        {
                            (j == i) ? (inventoryVector[y + j][x + i] = "16") : (inventoryVector[y + j][x + i] = "1");
                        }
                    }
                }
                itemsCount++;
            }
        }
    }
}

void updateInventory()
{
    updateInventoryHealth();
    updateInventoryItems();
}

void renderInventory()
{
    for (unsigned y = 0; y < 24; y++)
    {
        for (unsigned x = 0; x < 42; x++)
        {
            WORD inventory_colour = 0x00;
            switch (stoi(inventoryVector[y][x]))
            {
            case 0: inventory_colour = 0x00; break;  // boarder
            case 1: inventory_colour = 0xF0; break;  // gui background
            case 2: inventory_colour = 0x40; break;  // heart icon
            case 3: inventory_colour = 0x60; break;  // char hair  
            case 4: inventory_colour = 0x80; break;  // char face     
            case 5: inventory_colour = 0xB0; break;  // char upper body
            case 6: inventory_colour = 0x30; break;  // char lower body
            case 7: inventory_colour = 0x70; break;  // char hand
            case 8: inventory_colour = 0xA0; break;  // items background
            case 9: inventory_colour = 0x80; break;  // default icon background
            case 10: inventory_colour = 0x70; break; // default center icon
            case 11: inventory_colour = 0x60; break; // sword handle
            case 12: inventory_colour = 0x70; break; // sword part
            case 13: inventory_colour = 0x20; break; // chestplate
            case 14: inventory_colour = 0x30; break; // boot
            case 15: inventory_colour = 0xD0; break; // potion
            case 16: inventory_colour = 0x60; break; // key for lv 5
            }
            g_Console.writeToBuffer((x + 9) * 2, y + 8, "  ", inventory_colour);
        }
    }
}

void renderItemInfos()
{
    if (((g_mouseEvent.mousePosition.Y - 8) > 1) && (((g_mouseEvent.mousePosition.X / 2) - 9) > 1) && ((g_mouseEvent.mousePosition.Y - 8) < 23) && (((g_mouseEvent.mousePosition.X / 2) - 9) < 41))
    {
        if ((inventoryVector[g_mouseEvent.mousePosition.Y - 8][(g_mouseEvent.mousePosition.X / 2) - 9] == "11") || (inventoryVector[g_mouseEvent.mousePosition.Y - 8][(g_mouseEvent.mousePosition.X / 2) - 9] == "12"))
        {
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y, "> Sword <", 0x17);
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 1, "Damage: +2", 0x17);
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 2, (usingSword == true) ? "Equipped" : "Equip", 0x17);
        }
        else if (inventoryVector[g_mouseEvent.mousePosition.Y - 8][(g_mouseEvent.mousePosition.X / 2) - 9] == "13")
        {
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y, "> Chestplate <", 0x17);
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 1, "Defense: +1", 0x17);
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 2, (usingChestplate == true) ? "Equipped" : "Equip", 0x17);
        }
        else if (inventoryVector[g_mouseEvent.mousePosition.Y - 8][(g_mouseEvent.mousePosition.X / 2) - 9] == "14")
        {
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y, "> Boot <", 0x17);
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 1, "Weight: -1", 0x17);
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 2, (usingBoot == true) ? "Equipped" : "Equip", 0x17);
        }
        else if (inventoryVector[g_mouseEvent.mousePosition.Y - 8][(g_mouseEvent.mousePosition.X / 2) - 9] == "15")
        {
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y, "> Potion <", 0x17);
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 1, "Health: +2", 0x17);
        }
        else if (inventoryVector[g_mouseEvent.mousePosition.Y - 8][(g_mouseEvent.mousePosition.X / 2) - 9] == "16")
        {
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y, "> Golden Key <", 0x17);
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 1, "Price: Inf", 0x17);
        }
    }
}

void renderEndScreen() {
    COORD c;
    c.X = 10; c.Y = 5;
    g_Console.writeToBuffer(c, "Created by Group 12: Jun Hou, Jiu Len, Darius and Winston.", 0xF0);
    c.Y += 1;
    if (g_sChar.hp > 0) {
        g_Console.writeToBuffer(c, "YOU WIN!!!", 0xF0);
        c.Y += 1;
        g_Console.writeToBuffer(c, "The village is saved!", 0xF0);
    }
    else {
        g_Console.writeToBuffer(c, "YOU LOSE. ", 0xF0);
        c.Y += 1;
        g_Console.writeToBuffer(c, "The village mourns your death.", 0xF0);
    }
    std::ostringstream ss;
    c.Y += 1;
    ss << "Kills: " << kills;
    g_Console.writeToBuffer(c, ss.str(), 0xF0);
    ss.str("");
    c.Y += 1;
    ss << std::fixed << std::setprecision(2);
    ss << "Time: " << g_dGameTime << "secs";
    g_Console.writeToBuffer(c, ss.str(), 0xF0);
    ss.str("");
    c.Y += 1;
    ss << "HP: " << g_sChar.hp;
    g_Console.writeToBuffer(c, ss.str(), 0xF0);
    //update values
}

void renderFramerate() {                //part of gui
    COORD c;
    std::ostringstream ss;

    ss << std::fixed << std::setprecision(2);
    ss << 1.0 / g_dDeltaTime << " fps";         // displays the framerate
    c.X = g_Console.getConsoleSize().X - 9; c.Y = 40;
    g_Console.writeToBuffer(c, ss.str(), 0xF0);

    ss.str("");
    ss << g_dGameTime << "secs";             // displays the elapsed time
    c.X = 0; c.Y = 40;
    g_Console.writeToBuffer(c, ss.str(), 0xF0);
    ss.str("");

    c.X = 10; c.Y = 40;
    if (g_sChar.hp <= 0) {             // displays the player's health
        ss << "Player HP: ";
    }
    else {
        ss << "Player HP: " << std::string(g_sChar.hp, (char)3); // (char)3 is heart symbol
    }
    g_Console.writeToBuffer(c, ss.str(), 0xF4); //white background, red text

    if (level_no == 4) {
        c.X = 40; c.Y = 40;
        ss.str("");
        if (enemies[0] == nullptr) { // displays the boss's health
            ss << "Boss HP: ";
        }
        else {
            ss << "Boss HP: " << std::string(enemies[0]->get_hp(), (char)254); // (char)254 is small square
        }
        g_Console.writeToBuffer(c, ss.str(), 0xF4); //white background, red text
    }

    c.X = 100; c.Y = 40;
    ss.str("");
    ss << "Kills: " << kills;
    g_Console.writeToBuffer(c, ss.str(), 0xF1); //white background, red text

    //render objectives here
    c.X = 0; c.Y = 42;
    WORD red = 0xF4;
    WORD green = 0xF2;
    WORD ObjectiveColor;
    g_Console.writeToBuffer(c, "Objectives:", 0xF0);
    c.X += 12;
    if (SWORD_AND_CHESTPLATE_GIVEN == true) ObjectiveColor = green;
    else ObjectiveColor = red;
    g_Console.writeToBuffer(c, "Find out where you are.", ObjectiveColor); //find prisoner, he'll explain stuff
    c.Y++;
    if (kills >= 1) ObjectiveColor = green;
    else ObjectiveColor = red;
    g_Console.writeToBuffer(c, "Defeat your first enemy.", ObjectiveColor);     //kill at least 1 enemy
    c.Y++;
    if (level_no > 3) ObjectiveColor = green;
    else ObjectiveColor = red;
    g_Console.writeToBuffer(c, "Look out for traps.", ObjectiveColor);     //get past level 3
    c.Y++;
    if (golemDefeated == true) ObjectiveColor = green;
    else ObjectiveColor = 0xF5;
    g_Console.writeToBuffer(c, "Get past the golem.", ObjectiveColor);     //kill the golem to unlock gate
    c.Y++;
    g_Console.writeToBuffer(c, "Leave the temple ALIVE!", red);            //find a way to escape
}

void renderToScreen() {
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}