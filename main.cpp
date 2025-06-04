#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "header/note/note.h"         // Include the note header file
#include "header/player/player.h"     // Include the player header file
#include "header/note/note_factory.h" // Include the note factory header file
#include "header/note/note_manager.h"
#include "header/schedule.h"
#include "header/ui/health_ui.h"
#include "header/score_manager.h"
#include "header/ui/score_ui.h"
#include "header/game_manager.h"
#include "header/assets.h"
#include "header/sound_manager.h"
#include "header/input_handler.h"

using namespace std;

//----------------------------------------------------------------------------------
// Defines Structures and Typedefs
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Constant Variables Definition
//----------------------------------------------------------------------------------
const int screenWidth = 1280; // Screen width
const int screenHeight = 720; // Screen height
const Vector2 center = {screenWidth / 2.0f, screenHeight / 2.0f};

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
Camera2D camera = {0};
Vector3 cubePosition = {0};

// Input Handler
PlayCommand playCmd;
RestartCommand restartCmd;
ExitPlayCommand exitCmd;

// Player Setup
Player player;

// UI Setup
HealthUI healthUI;

// Score Setup
ScoreUI scoreUI;

// Note Setup
// Lis<Note> notes;
NoteFactory noteFactory;
NoteManager noteManager;

float timer;
float endTime;
bool pressing = false;

Texture2D background;
Texture2D crowd;
Rectangle sourceCrowdRec;
float crowdFrameCounter = 0;
float currentCrowdFrame = 0;
float crowdSpeed = 8;

Texture2D stage;

// Main menu
bool textColorFade = true;
Color clickTextColor = WHITE;

// End Menu
float endMenuHeight = 210 * 2;
float endMenuWidth = 210 * 2;

Rectangle restartButton = {center.x - 100, center.y + 100, 200, 50};
Rectangle exitButton = {center.x - 100, center.y + 180, 200, 50};

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void); // Update and draw one frame
void DrawCoordinateSystemWithLabels(int screenWidth, int screenHeight);
void NoteScheduleRunner();
void Init();
void MainMenu();
void PlayMode();
void EndGame(bool win);

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Melody Beat");

    InitAudioDevice();

    // Init music
    GameManager::GetInstance().SetMusic("resources/Assets/music/audio.mp3");

    // Init SFX
    SoundManager::GetInstance().LoadSounds();

    ScoreManager::GetInstance().Attach(&scoreUI);
    player.Attach(&healthUI);

    GameManager::GetInstance().MoveToMainMenu();
    Init();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    UnloadMusicStream(GameManager::GetInstance().GetMusic()); // Unload music stream
    UnloadTexture(background);                                // Unload background texture
    UnloadTexture(crowd);                                     // Unload crowd texture
    UnloadTexture(stage);                                     // Unload stage texture
    noteManager.CleanUpNotes();                               // Clean up notes
    noteManager.ClearNotes();                                 // Clear notes
    player.Detach(&healthUI);                                 // Detach health UI observer
    ScoreManager::GetInstance().Detach(&scoreUI);             // Detach score UI observer
    SoundManager::GetInstance().~SoundManager();              // Clean up sound manager
    GameManager::GetInstance().~GameManager();                // Clean up game manager
    Assets::GetInstance().~Assets();                          // Clean up assets manager
    healthUI.~HealthUI();                                     // Clean up health UI
    scoreUI.~ScoreUI();                                       // Clean up score UI
    noteFactory.~NoteFactory();                               // Clean up note factory

    return 0;
}

// Update and draw game frame
static void UpdateDrawFrame(void)
{
    // Update
    // --------------------------------------------------------------
    // Handling Play Mode
    if (GameManager::GetInstance().OnMainMenu())
    {
        MainMenu();
    }
    // Update
    // --------------------------------------------------------------
    // Handling Play Mode
    if (GameManager::GetInstance().OnPlayMode())
    {
        PlayMode();
    }
    // --------------------------------------------------------------
    // Handling End Game
    if (GameManager::GetInstance().OnEndGame())
    {
        EndGame(player.IsAlive());
    }
    //----------------------------------------------------------------------------------
}

void Init()
{
    timer = 0;
    endTime = 0;
    textColorFade = true;

    noteManager.ClearNotes();
    ResetSchedule();

    // Playmode
    background = LoadTexture(Assets::GetInstance().background);
    crowd = LoadTexture(Assets::GetInstance().crowd);
    stage = LoadTexture(Assets::GetInstance().stage);

    sourceCrowdRec = {0, 0, (float)crowd.width / 2, (float)crowd.height};

    // Player Setup
    player.InitPlayer(screenWidth);

    // Score setup
    ScoreManager::GetInstance().Reset();

    // Schedule

    endTime = noteSchedule.back().time + 5.0f; // Set end time based on the last note in the schedule
}

void MainMenu()
{
    BeginDrawing();
    Rectangle sourceRec = {0.0f, 0.0f, (float)background.width, (float)background.height};
    Rectangle destRec = {0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()};
    Vector2 origin = {0.0f, 0.0f};
    float rotation = 0.0f;
    DrawTexturePro(background, sourceRec, destRec, origin, rotation, WHITE);

    DrawText("MELODY BEAT", GetScreenWidth() / 2 - MeasureText("RHYTHM GAME", 80) / 2, center.y - 100, 80, RED);

    if (textColorFade)
    {
        clickTextColor.a -= 10;
        if (clickTextColor.a <= 0)
        {
            textColorFade = false;
        }
    }
    else
    {
        clickTextColor.a += 10;
        if (clickTextColor.a >= 255)
        {
            textColorFade = true;
        }
    }

    DrawText("Click Anywhere to Start", screenWidth / 2 - MeasureText("Click Anywhere to Start", 20) / 2, center.y + 100, 20, clickTextColor);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        playCmd.Execute();
        Init();
    }
    EndDrawing();
}

void PlayMode()
{
    // handle timer
    timer += GetFrameTime();
    UpdateMusicStream(GameManager::GetInstance().GetMusic());

    bool keyPressedLeft = IsKeyPressed(KEY_Z);
    bool keyPressedRight = IsKeyPressed(KEY_X);
    bool doublePressSkip = IsKeyPressed(KEY_RIGHT);

    if (doublePressSkip || !player.IsAlive())
    {
        GameManager::GetInstance().EndGame();
    }

    // Input Handling
    bool hitLeft = IsKeyDown(KEY_Z);
    bool hitRight = IsKeyDown(KEY_X);

    // Hitbox Checker
    if (keyPressedLeft || keyPressedRight)
    {
        SoundManager::GetInstance().PlayHitSFX(GetRandomValue(0, 3)); // Play random hit sound
        pressing = true;
    }
    if (pressing)
    {
        bool hitted = noteManager.CheckHits(player, hitLeft, hitRight);
        if (!hitted)
            pressing = false;
    }

    crowdFrameCounter++;
    if (crowdFrameCounter >= (60 / crowdSpeed))
    {
        crowdFrameCounter = 0;
        currentCrowdFrame++;

        if (currentCrowdFrame > 2)
            currentCrowdFrame = 0;

        sourceCrowdRec.x = (float)currentCrowdFrame * sourceCrowdRec.width;
    }

    // Handle Note Schedule
    NoteScheduleRunner();

    noteManager.UpdateAllNotes(player);

    BeginDrawing();

    // Gambar background memenuhi layar
    Rectangle sourceRec = {0.0f, 0.0f, (float)background.width, (float)background.height};
    Rectangle destRec = {0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()};
    Vector2 origin = {0.0f, 0.0f};
    float rotation = 0.0f;
    DrawTexturePro(background, sourceRec, destRec, origin, rotation, WHITE);
    DrawTexturePro(stage, {0, 0, (float)stage.width, (float)stage.height}, destRec, origin, rotation, WHITE);

    // Draw player
    player.DrawPlayer(hitLeft, hitRight);

    noteManager.DrawAllNotes();

    Rectangle destCrowdRec = {150, 500, (float)crowd.width, (float)crowd.height + crowd.height / 2};
    DrawTexturePro(crowd, sourceCrowdRec, destCrowdRec, origin, rotation, WHITE);

    // Draw UI
    healthUI.Draw();
    scoreUI.Draw();
    scoreUI.DrawCombo();

    // Debugging
    // ------------------------------------------------------------
    // DrawFPS(10, 10);  // Uncomment to show FPS
    // Draw timer
    DrawText(TextFormat("timer: %.2f", timer), 20, 80, 15, WHITE);

    // Draw Coordinates
    // DrawCoordinateSystemWithLabels(screenWidth, screenHeight);

    EndDrawing();
}

void EndGame(bool win)
{
    BeginDrawing();
    Rectangle rectMenu = {
        0, 0,
        (float)GetScreenWidth(),
        (float)GetScreenHeight()};
    DrawRectangleRec(rectMenu, BLACK);
    if (win)
    {
        DrawText("You Win!", center.x - MeasureText("You Win!", 100) / 2, center.y - 200, 100, GREEN);
        // Draw total score
        int finalScore = ScoreManager::GetInstance().GetScore();
        int comboScore = ScoreManager::GetInstance().GetMaxCombo();
        DrawText(TextFormat("Your Score: %d", finalScore), center.x - MeasureText(TextFormat("Your Score: %d", finalScore), 30) / 2, center.y - 50, 30, WHITE);
        DrawText(TextFormat("Max Combo: %d", comboScore), center.x - MeasureText(TextFormat("Max Combo: %d", comboScore), 30) / 2, center.y, 30, WHITE);
    }
    else
    {
        DrawText("Game Over", center.x - MeasureText("Game Over", 100) / 2, center.y - 100, 100, RED);
    }
    // Draw restart button
    DrawRectangleRec(restartButton, DARKGRAY);
    DrawText("Restart", restartButton.x + 40, restartButton.y + 10, 30, WHITE);

    // Draw exit button
    DrawRectangleRec(exitButton, DARKGRAY);
    DrawText("Exit", exitButton.x + 70, exitButton.y + 10, 30, WHITE);

    // Handle click
    Vector2 mouse = GetMousePosition();
    if (CheckCollisionPointRec(mouse, restartButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        // Restart logic
        restartCmd.Execute();
        Init();
    }
    else if (CheckCollisionPointRec(mouse, exitButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        exitCmd.Execute();
    }

    EndDrawing();
}

// Note Schedul Runner
// This code, run the schedule that defined on top of code.
void NoteScheduleRunner()
{
    if (!GameManager::GetInstance().OnPlayMode())
        return;

    if (timer >= endTime)
    {
        GameManager::GetInstance().EndGame();
    }

    for (auto &note : noteSchedule)
    {
        if (!note.spawned && timer >= note.time)
        {
            noteManager.AddNote(noteFactory.CreateNote(note.type, note.shipping, note.size, note.color));
            note.spawned = true;
        }
    }
}

void DrawCoordinateSystemWithLabels(int screenWidth, int screenHeight)
{
    Vector2 center = {screenWidth / 2.0f, screenHeight / 2.0f};

    // Draw X and Y axis
    DrawLine(0, center.y, screenWidth, center.y, RED);    // X-axis
    DrawLine(center.x, 0, center.x, screenHeight, GREEN); // Y-axis

    int spacing = 50;
    Color gridColor = Fade(LIGHTGRAY, 0.3f);

    // Draw vertical grid lines and X labels
    for (int x = 0; x <= screenWidth; x += spacing)
    {
        DrawLine(x, 0, x, screenHeight, gridColor);

        int xCoord = x - center.x;
        if (xCoord != 0) // Skip 0,0 label redundancy
            DrawText(TextFormat("%d", xCoord), x + 2, center.y + 2, 10, DARKGRAY);
    }

    // Draw horizontal grid lines and Y labels
    for (int y = 0; y <= screenHeight; y += spacing)
    {
        DrawLine(0, y, screenWidth, y, gridColor);

        int yCoord = center.y - y;
        if (yCoord != 0)
            DrawText(TextFormat("%d", yCoord), center.x + 2, y + 2, 10, DARKGRAY);
    }

    // Draw center (0,0)
    DrawCircleV(center, 4, BLACK);
    DrawText("0,0", center.x + 5, center.y + 5, 10, BLACK);
}
