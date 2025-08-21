//
//  This sketch uses an M5Stack Core2 (or CoreS3) to record and then
//  playback sounds at different speeds.
//
//  License:  MIT. See the LICENSE file in the project root for
//            more details.
//


#include <M5Unified.h>
#include <M5GFX.h>

//  Button sizing
const int     buttonSize      = 50;
const int     halfBtnSize     = buttonSize / 2;

//  Sample rates for normal recording, and slower and faster
//  playback to make the voices sound funny
const size_t  normalSamples   = 16000;
const size_t  slowerSamples   = normalSamples * 0.75F;
const size_t  fasterSamples   = normalSamples * 2;
const size_t  allSamples  []  = {slowerSamples, normalSamples, fasterSamples};
const size_t  numSamples      = sizeof(allSamples) / sizeof(allSamples [0]);
const char*   samplesText []  = {"Slow  ", "Normal", "Fast  "};

//  Number of second to record
const size_t  recordSeconds   = 1;

//  The amount of memory we need to record
const size_t  recordingSize   = normalSamples * sizeof(uint16_t) * recordSeconds;

//  We need some buffer padding to prevent overruns
const size_t  bufferPadding   = recordingSize * 0.5F;

//  Record buffer
int16_t *recordBuffer;

//  Current playback samples
int sampleIndex               = 1;
int playbackSamples           = normalSamples;

//  Screen dimensions
int screenWidth;
int screenHeight;

//  Action buttons
const char *actionText    []  = {"<", ">", "+", "-"};
const int   numActions        = sizeof(actionText) / sizeof (actionText [0]);
LGFX_Button actionButtons [numActions];
enum actions  : int {slowerButton, fasterButton, recordButton, stopButton};

//  State of the program
enum appState : int {appWaiting, appRecording, appPlayback};

//  Current program state
appState    currentState = appWaiting;


//  Update the screen with the playback speed message
void UpdatePlaybackMessage (void)
{
  M5.Display.setCursor(0, 0);
  M5.Display.printf("Play : %s", samplesText [sampleIndex]);
}


void setup (void)
{
  M5.begin();

  //  Configure display
  M5.Display.setRotation(0);
  M5.Display.fillScreen(BLACK);
  M5.Lcd.setTextSize(3);
  
  //  Get screen dimensions
  screenWidth   = M5.Display.width();
  screenHeight  = M5.Display.height();

  //  Allocate the record buffer
  //
  //  Note: We allocate the record buffer slightly larger than we need, but only
  //        record to value of recordingSize. This seems to help some buffer
  //        overrun condition when recording
  //        
  //
  recordBuffer = (typeof(recordBuffer)) heap_caps_malloc(recordingSize + bufferPadding, MALLOC_CAP_8BIT);
  if (recordBuffer == nullptr)
  {
    //  We did not get the buffer
    M5.Display.fillCircle(screenWidth / 2, screenHeight / 2, screenWidth / 4, RED);
    while (true)
    {
      //  Can't do anything - stay here forever
      delay(1000);
    }
  }

  //  Clear the buffer
  memset(recordBuffer, 0, recordingSize);

  //  Set X position for action buttons. These are at the bottom of the screen
  const int buttonsOnX  = screenWidth / numActions;
  const int actionY     = screenHeight - halfBtnSize;
  int       actionX     = halfBtnSize;

  //  Add action buttons
  for (int i = 0; i < numActions; i ++)
  {
    actionButtons [i].initButton(&M5.Lcd, actionX, actionY, buttonSize, buttonSize, BLACK, WHITE, BLACK, actionText [i], 4, 4);
    actionButtons [i].drawButton();

    //  Next X positon
    actionX += buttonsOnX;
  }

  //  Update the playback speed message
  UpdatePlaybackMessage();
 
  //  Make sure the microphone and speaker are off
  M5.Mic.end();
  M5.Speaker.end();
}

void loop (void)
{
  M5.update();

  //  Get information about where the user touched the screen
  auto touchDetail = M5.Touch.getDetail();

  if (touchDetail.wasPressed())
  {
    //  See if the user pressed an action button
    for (int i = 0; i < numActions; i ++)
    {
      if (actionButtons [i].contains(touchDetail.x, touchDetail.y))
      {
        //  See what action the user wants
        switch (i)
        {
          //  Slow down playback
          case slowerButton:
            sampleIndex --;
            if (sampleIndex < 0)
            {
              sampleIndex = 0;
            }

            playbackSamples = allSamples [sampleIndex];
            UpdatePlaybackMessage();
            break;

          //  Speed up playback
          case fasterButton:
            sampleIndex ++;
            if (sampleIndex == numSamples)
            {
              sampleIndex = numSamples - 1;
            }

            playbackSamples = allSamples [sampleIndex];
            UpdatePlaybackMessage();
            break;

          case recordButton:
            if (currentState == appRecording)
            {
              //  Already recording
              break;
            }

            if (currentState == appPlayback)
            {
              //  Indicate we are waiting
              M5.Display.fillCircle(screenWidth / 2, screenHeight / 2, buttonSize, ORANGE);

              //  Wait for the speaker to finish playback
              while (M5.Speaker.isPlaying())
              {
                delay(100);
              }
            }

            currentState = appRecording;

            //  We can only record when playback has finished
            M5.Display.fillCircle(screenWidth / 2, screenHeight / 2, buttonSize, RED);

            //  Enable the microphone
            M5.Mic.begin();

            //  Clear the buffer before recording
            memset(recordBuffer, 0, recordingSize);

            //  Record data
            M5.Mic.record(recordBuffer, recordingSize, normalSamples);

            break;

          case stopButton:
            if (currentState == appPlayback)
            {
              //  Already in playback
              break;
            }

            if (currentState == appRecording)
            {
              //  Indicate we are waiting
              M5.Display.fillCircle(screenWidth / 2, screenHeight / 2, buttonSize, ORANGE);

              //  Wait for the microphone to finish
              while (M5.Mic.isRecording())
              {
                delay(100);
              }
            }

            //  Now in playback
            currentState = appPlayback;

            //  Display the microphone
            M5.Mic.end();

            //  Indicate we are playing back
            M5.Display.fillCircle(screenWidth / 2, screenHeight / 2, buttonSize, GREEN);

            //  Enable the speaker
            M5.Speaker.begin();
            M5.Speaker.setVolume(255);

            //  Play the recording
            M5.Speaker.playRaw(recordBuffer, recordingSize, playbackSamples, false, 1, 0);
            break;
        }
      }
    }
  }

  //  If we are in playback mode and have finished playback
  //  we can disable the speaker for the next recording
  if (currentState == appPlayback
  &&  !M5.Speaker.isPlaying())
  {
    //  Application state is back to waiting
    currentState = appWaiting;

    //  Disable the speaker
    M5.Speaker.end();

    //  Clear the playback circle
    M5.Display.fillCircle(screenWidth / 2, screenHeight / 2, buttonSize, BLACK);
  }
}
