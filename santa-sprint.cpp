/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// James Bunt - D00262403 - MSc Computing, Gaming & Extended Reality
// Software Engineering - CA1
// Santa Sprint - November 2022
// https://github.com/jbunt85/ca1-santa_sprint

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "raylib.h" //linking the external raylib library to our code

struct AnimData // declaring member variables for smarter and more efficient coding
{
    Rectangle rec; // rectangle for the image framing
    Vector2 pos; // vector frame for the positon
    int frame; // animation frame integer
    float UpdateTime; // update time float
    float RunningTime; // running time float
};

// function for ground check
bool GroundCheck(AnimData data, int windowHeight){return data.pos.y >= windowHeight - data.rec.height; } // testing whether santa is on the ground by asking is santas vertical position more than the window bottom of the window (0 y position) deducated from santas heightreturning. This will return a boolean value, returning a true response if character is on the ground or false if in the air. This will increase efficiency of code later when I am adjusting the animation based on whether character is jumping or not

// function for game start screen
bool GameStarted = false; // defining a new boolean variable and initialising to zero, ready to use for a start screen option

// function for animation frame data update
AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame) // Return anim data, called UpdateAnimData. Takes in AnimData as input parameter called data. float is used to input delta time
{
    // update running time
    data.RunningTime += deltaTime; // adding delta time variable to the running time each frame
    if (data.RunningTime >= data.UpdateTime) // checking whether the running time is less than the update time member variable, if true then...
    {
        data.RunningTime = 0.0; // if true, then reset running time back to zero
        // update animation frame
        data.rec.x = data.frame * data.rec.width; // setting the x value of the rectangle by assigning it the value of frame value multiplied by rectangle width 
        data.frame++; // update frame by incrementing by one
        if (data.frame > maxFrame) // checking whether we have hit the maximum number of frames on the sprite sheet (noting that we are using this same function with multiple different sized sprite sheets, we need to use the max frame variable which we will also define to enable greater flexibility and efficiently), and if so...
        {
            data.frame = 0; // if we have reached max number of frames on sprite sheet then we reset to zero and start cycle again
        }
    }
    return data; // return the output of the update anim data function which automates the increment and reset of the animation frame variables and cycle
}

int main() //main function body
{ 
    // array for window dimensions (this will be designed to play in a social media app, so going for simple, clean, fun and small)
    int WindowDimensions[2]; // creating an array for window dimensions which are then easier to reference in the rest of code
        WindowDimensions[0] = 512; // 1st array for window width of 512
        WindowDimensions[1] = 380; // 2nd array for window height of 380

    // window dimensions
    InitWindow(WindowDimensions[0], WindowDimensions[1], "Santa Sprint"); // initialise window using constant values and exciting game name in header. Note that I have used the window dimension array variables here instead of entering fresh values again for efficiency
    // load audio player
    InitAudioDevice(); // intialise audio device so we can activate sounds and Music

    // acceleration due to Gravity (pixels per second per second)
    const int Gravity{1000}; // applying acceleration due to simulated impact of Gravity using pixels per frame/ per frame

    // game audio
    Music Music = LoadMusicStream("resources/Music-comingtotown.mp3"); // loading background Music file from resources folder
    PlayMusicStream(Music); // activate the Music function in Raylib and naming it 'Music'
    Sound SoundJump = LoadSound("resources/sound-jump.mp3"); // loading jump sound file from resources folder
    Sound SoundPass = LoadSound("resources/sound-pass.mp3"); // loading pass or win sound file from resources folder
    Sound SoundFail = LoadSound("resources/sound-fail.mp3"); // loading fail or lose sound file from resources folder
    
    // santa character variables
        Texture2D santa = LoadTexture("resources/santa.png"); // initialising character by loading image in using the Raylib call function
        AnimData SantaData; // creating new AnimData variable for character and initialising all its member variables ready for efficient reuse 
            SantaData.rec.width = santa.width/6; // selecting only one of the 6 sprites on the sheet by dividing width by 6
            SantaData.rec.height = santa.height; // using full height of sprite sheet
            SantaData.rec.x = 0; // initialising character rectangle x position at zero to start
            SantaData.rec.y = 0; // initialising character rectangle y position at zero to start
            SantaData.pos.x = WindowDimensions[0]/2 - SantaData.rec.width/2; // centering my santa character's vertical position exactly in the centre noting that I need to take into account the width of the character for true center-ness 
            SantaData.pos.y = WindowDimensions[1] - SantaData.rec.height; // placing the character on the ground but taking into account actual character height to ensure feet are on floor and not below
            SantaData.frame = 0; // initalise at zero to support embedding in the character sprite sheet
            SantaData.UpdateTime = 1.0/12.0; //setting an update time variable to 12th of second for the animation update time between frames leading to a gently santa movement. Note that this is constant and will not be changed across the program
            SantaData.RunningTime = 0.0; // declaring new running time veriable and initialising it to zero at start of sequence

    // hazard variables
    Texture2D Coal = LoadTexture("resources/Coal-sprite-sheet3.png"); // initialising hazards by loading image in using the Raylib call function

    const int HazardCount{10}; // constant variable for the amount of hazards that should be included, this will be used many times across the code in every for loop so important to set a variable to prevent unecessary repeat coding which is prone to error, especially by me!
    AnimData Coals[HazardCount] {}; // creating array which recalls the amount of hazards defined in the above constant integer variable instead of seperate anim datas for efficiency, leaving this blank as I will define later within a for loop for efficiency

    for (int i = 0; i < HazardCount; i++) // setting up for loop for the hazards so I don't have to repeat code for each hazard then I will assign a starting value to each of the AnimData variables which can be reused without unecessary repeated coding. Note that here I have initialised the for loop with an integer at zero then set activation criteria that it has to be below 2, finally with the action of adding one to the i value if criteria is passed
    {
        // hazard positioning
        int VerticalOffset = GetRandomValue(0.0, 1.5); // random values enable different heights of the hazards to be used in the later y position for more variety and complexity in game
        Coals[i].rec.x = 0.0; // initialising rectangle vertical position
        Coals[i].rec.y = 0.0; // initialising rectangle horizontal position
        Coals[i].rec.width = Coal.width/8; // rectangle width value noting the 8 value sprite sheet being used
        Coals[i].rec.height = Coal.height/8; // rectangle height value noting the 8 value sprite sheet being used
        Coals[i].pos.x = WindowDimensions[0] + (i*300);  // initialising x position for each hazard with a consistent gap of 300 between hazards
        Coals[i].pos.y = WindowDimensions[1] - Coal.height/8 - VerticalOffset * SantaData.rec.height; // initialising y position value ready for the hazard to be floating off the ground. 
        Coals[i].frame = 0; // initialising the frame at zero
        Coals[i].RunningTime = 0.0; // initialising the run time at zero
        Coals[i].UpdateTime = 0.0; // initialising the update time at zero
    }

    // game finish line to pass for win
    float FinishLine{Coals[HazardCount - 1].pos.x}; // x position of an invisible finish line to activate completion of game, leading to a winning state for the player. The hazard array (Coals) has been called where I have accessed the final element in the hazard array - identified by calling the HazardCount (total number of hazards) minus one (since we are starting at zero so I need to adjust the number being recalled for the actual finish line)

    // non consistent hazard variables outside the for loop (I went for 12 here but noting that it goes up to 11 as we start from 0)
    Coals[0].pos.x = WindowDimensions[0]; // first hazard just off window
    Coals[1].pos.x = WindowDimensions[0] + 300; // phase 1 of hazards 300 pixels away from eachother
    Coals[2].pos.x = WindowDimensions[0] + 600; // phase 1 of hazards 300 pixels away from eachother
    Coals[3].pos.x = WindowDimensions[0] + 900; // phase 1 of hazards 300 pixels away from eachother
    Coals[4].pos.x = WindowDimensions[0] + 1100; // phase 2 of hazards 200 pixels away from eachother
    Coals[5].pos.x = WindowDimensions[0] + 1300; // phase 2 of hazards 200 pixels away from eachother
    Coals[6].pos.x = WindowDimensions[0] + 1500; // phase 2 of hazards 200 pixels away from eachother
    Coals[7].pos.x = WindowDimensions[0] + 1800; // phase 3 of hazards 150 pixels away from eachother
    Coals[8].pos.x = WindowDimensions[0] + 1950; // phase 3 of hazards 150 pixels away from eachother
    Coals[9].pos.x = WindowDimensions[0] + 2100; // phase 3 of hazards 150 pixels away from eachother
    Coals[10].pos.x = WindowDimensions[0] + 2450; // phase 4 of hazards - double Coal action
    Coals[11].pos.x = WindowDimensions[0] + 2500; // phase 4 of hazards - double Coal action

    // hazard x Velocity (pixels/ second)
    int CoalVel{-200}; // setting the hazard its own Velocity, noting that this is initialised in the negative since it is moving backwards from right to left
        
    // air check
    bool IsInAir = false; // checking if the rectangle is in the air and initialising at zero
   
    // jump Velocity (pixels per second)
    const int JumpVel{-600}; // assigning a high negative value which can be added to every jump action at an increased power of the jump to so Santa can jump over the Coals

    int Velocity{0}; // enabling upwards movement by reducing the y coordinates

    // background textures
    Texture2D background = LoadTexture("resources/background-winter2x.png"); // loading in the third background ready for later animation
    float bgX{}; // initialising a x coordinate variable for the background to zero using a float since we are to define a Vector 2
    Texture2D midground = LoadTexture("resources/midground-moutains2x.png"); // loading in the second background ready for later animation
    float mgX{}; // initialising a x coordinate variable for the midground to zero using a float since we are to define a Vector 2
    Texture2D foreground = LoadTexture("resources/foreground-trees2x.png"); // loading in the third background first ready for later animation
    float fgX{}; // initialising a x coordinate variable for the foreground to zero using a float since we are to define a Vector 2
    Texture2D startground = LoadTexture("resources/start-screen.png"); // loading in the start screen background

    bool Collision = false; // creating boolian for a Collision variable which will be later defined, this is initialised as false which enables a 1 life scenario (ie you lose if character collides with a single hazard)

    SetTargetFPS(60); //setting frames per second to 60 for efficiency
    while(!WindowShouldClose()) //allows user to exit upon escape button and end below while loop
        {	
            // delta time (time since last frame)
            const float dT = GetFrameTime(); // ensuring the program is frame rate interdependent as this can not be relied upon for reliability

            // start drawing
            BeginDrawing(); //start drawing command
            ClearBackground(WHITE); //set white background just as standard
            UpdateMusicStream(Music); //update Music buffer with new stream which will loop background Music

            // start screen before gameplay activates
            if (!GameStarted) // if the Game Started variable is false then...
            {
                // draw startscreen
                DrawTexture(startground, 0.0, 0.0, WHITE); // background image
                DrawText("Press Space Key to Start", (WindowDimensions[0]/4), (WindowDimensions[1]*0.87), 20, BLACK); // text instruction in black
                DrawText("Don't hit the burning Coals!", (WindowDimensions[0]/4), (WindowDimensions[1]*0.93), 20, RED); // additional text instruction in red
                
                if(IsKeyDown(KEY_SPACE)) // if user presses space bar then...
                {
                    GameStarted = true; // activate Game Start function and
                    PlaySound(SoundPass); // play pass sound so user has audio prompt of action and start of gameplay
                }
            }
            else // if the if statement above is not true then do the following
            {

            // scroll and cycle the background
            bgX -= 20 * dT; // Updating background x position by 20 pixels per second and also multiplied by delta time so it is screen rate independent which means that possible system issues or lag will not impact game quality
            if (bgX <= -background.width) // if statement to check when we have reached end of 2nd copy of far background by checking if it is equal or less than the negtive value of the background images width (at a scale of 2). When activated, the first copy of the background texture's position will be reset and will return back to starting position ready for next background cycle
            {
                bgX = 0.0; // if true then return background x position to zero, ready for next background animation cycle
            }

            // scroll and cycle the midground
            mgX -= 40* dT; // Updating background x position by 40 pixels per second and also multiplied by delta time so it is screen rate independent which means that possible system issues or lag will not impact game quality
            if (mgX <= -midground.width) // if statement to check when we have reached end of 2nd copy of far background by checking if it is equal or less than the negtive value of the background images width (at a scale of 2). When activated, the first copy of the background texture's position will be reset and will return back to starting position ready for next background cycle
            {
                mgX = 0.0; // if true then return midground x position to zero, ready for next background animation cycle
            }

            // scroll and cycle the foreground
            fgX -= 80* dT; // Updating background x position by 60 pixels per second and also multiplied by delta time so it is screen rate independent which means that possible system issues or lag will not impact game quality
            if (fgX <= -foreground.width) // if statement to check when we have reached end of 2nd copy of far background by checking if it is equal or less than the negtive value of the background images width (at a scale of 2). When activated, the first copy of the background texture's position will be reset and will return back to starting position ready for next background cycle
            {
                fgX = 0.0; // if true then return foreground x position to zero, ready for next background animation cycle
            }

            // draw background image twice
            Vector2 bg1Pos{bgX, 0.0}; // defining the vector 2 or position of the upcoming background image, this will fit the window resting at the zero point of the window. Note that the x position of the background will shift 20 pixels every second to simulate the movement
            DrawTextureEx(background, bg1Pos, 0.0, 1.0, WHITE); // similar to a texture 2d but with added variables for vector 2 and floats. Note that I am increasing scale of background image to 2 and without any tint or further adjustment like rotation
            Vector2 bg2Pos{bgX + background.width, 0.0}; //creating another vector2 for the subsquent backgrounds, also scaled by 2
            DrawTextureEx(background, bg2Pos, 0.0, 1.0, WHITE); // drawing the multiple background texture using same variables as first by with the second vector

            // draw midground image twice
            Vector2 mg1Pos{mgX, 0.0}; // defining the vector 2 or position of the upcoming midground image, this will fit the window resting at the zero point of the window. Note that the x position of the background will shift 40 pixels every second to simulate the movement
            DrawTextureEx(midground, mg1Pos, 0.0, 1.0, WHITE); // similar to a texture 2d but with added variables for vector 2 and floats. Note that I am increasing scale of midground image to 2 and without any tint or further adjustment like rotation
            Vector2 mg2Pos{mgX + midground.width, 0.0}; //creating another vector2 for the subsquent midgrounds, also scaled by 2
            DrawTextureEx(midground, mg2Pos, 0.0, 1.0, WHITE); // drawing the multiple midground texture using same variables as first by with the second vector

            // draw foreground image twice
            Vector2 fg1Pos{fgX, 0.0}; // defining the vector 2 or position of the upcoming foreground image, this will fit the window resting at the zero point of the window. Note that the x position of the foreground will shift 80 pixels every second to simulate the movement
            DrawTextureEx(foreground, fg1Pos, 0.0, 1.0, WHITE); // similar to a texture 2d but with added variables for vector 2 and floats. Note that I am increasing scale of background image to 2 and without any tint or further adjustment like rotation
            Vector2 fg2Pos{fgX + foreground.width, 0.0}; //creating another vector2 for the subsquent foregrounds, also scaled by 2
            DrawTextureEx(foreground, fg2Pos, 0.0, 1.0, WHITE); // drawing the multiple foreground texture using same variables as first by with the second vector

            // draw start screen
            DrawTextureEx(foreground, fg1Pos, 0.0, 1.0, WHITE); // similar to a texture 2d but with added variables for vector 2 and floats. Note that I am increasing scale of background image to 2 and without any tint or further adjustment like rotation

            // draw signature
            DrawText("SANTA SPRINT 2022", 10, 10, 20, RED); // game title using default font in the top left of screen. posx is 10. posy is 10. font size 20. color red.

            // ground check
            if (GroundCheck(SantaData,WindowDimensions[1])) // if statement that is calling in the ground check function using the anim data and character height, this returns a boolean result 
            {
                // rectangle is on the ground
                Velocity = 0; // resetting Velocity back to zero since we are on the ground
                IsInAir = false; // logging that we are in air so we can activate jumping feature
            }
            else // when rectangle is in the air do the following (ie apply Gravity to bring back down)
            {
                // air check
                Velocity += Gravity * dT; // apply Gravity by adding Gravity pixel everytime which brings character back down
                IsInAir = true; // logging that we are not in air so we can deactivate jumping feature
            }

            // jump check
            if (IsKeyPressed(KEY_SPACE) && IsInAir == false) // to detect and action against space bar being pressed but only if air detection is not identified which prevents double jumping
            {
                Velocity += JumpVel; // reducing Velocity by negative 10 moves upwards and simulates jump on space bar click
                PlaySound(SoundJump); // play sound file named SoundJump which was previously loaded
            }

            // reduce rising when jump is released
            if (IsKeyReleased(KEY_SPACE) && Velocity < 0) // when space bar is released and Velocity variable is below zero then...
            {
                Velocity = Velocity/2; //adjust Velocity varible by half
            }

            // update hazard position
            for (int i = 0; i < HazardCount; i++) // for loop, initialise variable i as zero, then when hazard count is less than i and one i
            {
               Coals[i].pos.x += CoalVel * dT;  // update x position every frame for each of the incoming hazards and making frame independent by multipying delta time which changes to pixels by frame
            }

            // update finish line
            FinishLine += CoalVel * dT; // scrolling finish line to left at the same speed as the hazards, achieved by using the hazards Velocity multiplied by the delta time

            // update character position
            SantaData.pos.y += Velocity * dT; // update y position every frame for the movement and making frame independent by multipying delta time which changes to pixels by frame

            // update character update frame
            if (!IsInAir) // using an if statement, only enabling animation through update of running time if the is in air check is not activated - this means that santa character will stop running when they are jumping for more realism to the character movement
            {
                // update character running time
                SantaData = updateAnimData(SantaData, dT,5); // call update anim data function and use its return value to update santa animation frame data
            }
            
            // update animation frame for all hazards 
            for (int i = 0; i < HazardCount; i++) // using a for loop which will loop over every element of the hazard Coals array then update its animation frame in a single location which is a more efficient practice rather than coding the animation frame updates for each hazard 
            {
            // update hazard animation frame
                Coals[i] = updateAnimData(Coals[i], dT, 7); // call update anim data function and use its return value to update santa animation frame data
            }

            // Collision rectangle to enable accurate Collision detection between character and hazard
            for(AnimData Coal : Coals) // range based for loop to check for Collision
            {
                float padding{60}; // in order for the Collision rectangle to more accurately pick the hazard image and not its blank border in the sprite sheet, I am defining the border around the hazard images in the sprite sheet of 60 to later trim for better gameplay (i.e. Collision is with actual hazard and not area around hazard)
                // hazard Collision rectangle
                Rectangle CoalRec //rectangle is used to locate the hazard on the sprite sheet
                    { 
                        Coal.pos.x + padding, // x position of hazard image in the sprite sheet plus padding, so we're closer to the hazard image by 20, trimming its blank border
                        Coal.pos.y + padding, // y position of hazard image in the sprite sheet plus padding, so we're closer to the hazard image by 20, trimming its blank border
                        Coal.rec.width - 2*padding, // width of Collision rectangle minus the padding around the hazard image in the sprite sheet
                        Coal.rec.height - 2*padding // height of Collision rectangle minus the padding around the hazard image in the sprite sheet
                    };
                // character Collision rectangle
                Rectangle santaRec //rectangle is used to locate the character on the sprite sheet
                    { 
                        SantaData.pos.x, // x position of hazard image in the sprite sheet
                        SantaData.pos.y, // y position of hazard image in the sprite sheet
                        SantaData.rec.width, // width of Collision rectangle
                        SantaData.rec.height // height of Collision rectangle 
                    };
                if (CheckCollisionRecs(santaRec, CoalRec)) // if statement to activate if the character rectangle collides with hazard rectangle, and if true...
                {
                    Collision = true; // update the Collision variable to true if Collision occurs (note that this will activate a lose outcome)
                }
            }

            if (Collision) // if statement for when Collision variable is activated
            {
                // lose the game
                PlaySound(SoundFail); // play fail sound
                DrawText("SAD SANTA!",WindowDimensions[0]/5, 2*WindowDimensions[1]/5,50,BLACK); // write text dark shadow in black
                DrawText("SAD SANTA!",WindowDimensions[0]/5-4, 2*WindowDimensions[1]/5-3,50,WHITE); // write text light shadow in white
                DrawText("SAD SANTA!",WindowDimensions[0]/5-5, 2*WindowDimensions[1]/5-4,50,RED); // write text front in green
                SantaData.pos.x = 0;

                /* NOTE: In this section I tried to problem solve a way to restart the game. I could have left this out and pretended everything was resolved but I wanted to demonstrate that I pushed myself in creativity and problem solving beyond my experience, with most things working but this being the last feature that I did not have time to work on until resolution. The logic here was that I need to reset all the functions and variables (ie - santa hazard Collision off, finish line reset to zero and game start to zero - however this was not enough so I removed this BUT I will return to resolve one day! Leaving in here so you seem my issue and attempts)
                // restart the game
                if(IsKeyDown(KEY_SPACE)) // if space key is pressed then...
                {
                    // reset all variables on game loss
                    Collision = false; //reset Collision variable to allow game restart
                    GameStarted = false; //reset game started variable to return to start screen
                    // rest all moving variables
                    SantaData.pos.x = 0; // reset santa position
                    data.RunningTime = 0.0; // reset counting run time
                    data.UpdateTime = 0.0; / reset counting update time
                    // hazard return
                    Coals[0].pos.x = WindowDimensions[0]; // first hazard just off window
                    Coals[1].pos.x = WindowDimensions[0] + 300; // phase 1 of hazards 300 pixels away from eachother
                    Coals[2].pos.x = WindowDimensions[0] + 600; // phase 1 of hazards 300 pixels away from eachother
                    Coals[3].pos.x = WindowDimensions[0] + 900; // phase 1 of hazards 300 pixels away from eachother
                    Coals[4].pos.x = WindowDimensions[0] + 1100; // phase 2 of hazards 200 pixels away from eachother
                    Coals[5].pos.x = WindowDimensions[0] + 1300; // phase 2 of hazards 200 pixels away from eachother
                    Coals[6].pos.x = WindowDimensions[0] + 1500; // phase 2 of hazards 200 pixels away from eachother
                    Coals[7].pos.x = WindowDimensions[0] + 1800; // phase 3 of hazards 150 pixels away from eachother
                    Coals[8].pos.x = WindowDimensions[0] + 1950; // phase 3 of hazards 150 pixels away from eachother
                    Coals[9].pos.x = WindowDimensions[0] + 2100; // phase 3 of hazards 150 pixels away from eachother
                }
                */
            }
            else if (SantaData.pos.x >= FinishLine) // if above if statement is not true then...
            {
                // win the game
                PlaySound(SoundPass); // play sound file called SoundPass
                DrawText("HO HO HOORAY!",WindowDimensions[0]/7, 2*WindowDimensions[1]/5,50,BLACK); // write text dark shadow in black
                DrawText("HO HO HOORAY!",WindowDimensions[0]/7-4, 2*WindowDimensions[1]/5-3,50,WHITE);  // write text light shadow in white
                DrawText("HO HO HOORAY!",WindowDimensions[0]/7-5, 2*WindowDimensions[1]/5-4,50,GREEN);  // write text front in green
            }
            else // if any of the above conditions are not activated then
            {
                // draw hazard for loop (only if Collision has not occured)
                for (int i = 0; i < HazardCount; i++) // standard for loop, we are naming the integer variable i for index, initialising it at zero, setting the condition to be less than the hazard count constant variable then, if it passes the condition, adding the increment of 1
                {
                    DrawTextureRec(Coal, Coals[i].rec, Coals[i].pos, WHITE); // calling upon the hazard variables for image, size and position without tint
                }

                // draw santa character (only if Collision has not occured)
                DrawTextureRec(santa, SantaData.rec, SantaData.pos, WHITE); // calling upon the character variables for image, size and position without tint
            }
            }
            EndDrawing(); //stop all drawing activity
        }
    // unloading data processing upon program closure
    UnloadTexture(santa); //unloading the character texture and shutting things down properly for efficiency
    UnloadTexture(Coal); //unloading the hazard texture and shutting things down properly for efficiency
    UnloadTexture(background); //unloading the background texture and shutting things down properly for efficiency
    UnloadTexture(midground); //unloading the midground texture and shutting things down properly for efficiency
    UnloadTexture(foreground); //unloading the foreground texture and shutting things down properly for efficiency
    UnloadTexture(startground); //unloading the start screen texture and shutting things down properly for efficiency
    CloseAudioDevice(); //close audio function before program termination for efficiency
    CloseWindow(); //close window function before program termination for efficiency
}