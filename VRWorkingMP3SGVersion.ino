/*
.......Hey Cyberon......
       August 2024
Development using Nano Connect 2040 
Direct drive 12 NeopPixels
UART drive Mini MP3 player
*/

//#include "Arduino.h"
#include <DSpotterSDK_MakerHL.h>
#include <NeoPixelConnect.h>

#define PIN 25
#define MAXIMUM_NUM_NEOPIXELS 12
#define S1RX 0  // Serial1 RX
#define S1TX 1  // Serial1 TX
#define opencamera 10000
#define takepicture 10001
#define playmusic 10002
#define stopmusic 10003
#define previousmusic 10004
#define nextmusic 10005

// The DSpotter License Data.
#include "CybLicense.h"
#define DSPOTTER_LICENSE g_lpdwLicense

#if defined(TARGET_NANO_RP2040_CONNECT)
#include "Model_L0.h"  // The packed level zero model file.
#endif
#define DSPOTTER_MODEL g_lpdwModel

// The VR engine object. Only can exist one, otherwise not worked.
static DSpotterSDKHL g_oDSpotterSDKHL;

NeoPixelConnect p(PIN, MAXIMUM_NUM_NEOPIXELS, pio1, 0);  //State Machine set to 1

int folder = 1;
int songNumber = 1;
int volume = 5;

void setup() {
  // Init Serial output for show debug info
  Serial.begin(9600);
  while (!Serial)
    ;
  DSpotterSDKHL::ShowDebugInfo(false);

  // Start serial connection to MP3 mini
  Serial1.begin(9600, S1RX, S1TX);

  // use SD card
  static uint8_t setSD_cmd [] = { 0X7E, 0xFF, 0x06, 0x3F, 0x00, 0x00, 0x02, 0xFE, 0xAC, 0XEF}; 
  send_cmd(setSD_cmd);
  delay(150);

  //set volume to 5
  volumeControl(volume);

  // set for first song in first folder
  folderControl(folder, songNumber);
  stopMusic();

  // Init VR engine & Audio
  if (g_oDSpotterSDKHL.Init(DSPOTTER_LICENSE, sizeof(DSPOTTER_LICENSE), DSPOTTER_MODEL, VRCallback) != DSpotterSDKHL::Success)
    return;
}
//--------------------------------------------------
void loop() {
  // Do VR
  g_oDSpotterSDKHL.DoVR();
}
//---------------------------------------------------

void playMusic() {
  p.neoPixelFill(0, 0, 0, false);
  static uint8_t play_cmd [] = { 0X7E, 0xFF, 0x06, 0X0D, 0x00, 0x00, 0x00, 0xFE, 0x00, 0XEF}; 
  send_cmd(play_cmd);
  p.neoPixelSetValue(songNumber, 0, 0, 255, true);
  songNumber += 1;
}

void stopMusic() {
  p.neoPixelFill(5, 5, 150, true);
  static uint8_t stop_cmd [] = { 0X7E, 0xFF, 0x06, 0x16, 0x00, 0x00, 0x00, 0xFE, 0xE5, 0XEF}; 
  send_cmd(stop_cmd);
  delay(150);
}

void previousMusic() {
  p.neoPixelFill(0, 0, 0, false);
  p.neoPixelSetValue(songNumber, 0, 0, 150, true);
  //myDFPlayer.previous();
  songNumber -= 1;
}

void nextMusic() {
  p.neoPixelFill(0, 0, 0, false);
  p.neoPixelSetValue(songNumber, 0, 0, 150, true);
  static uint8_t next_cmd [] = { 0X7E, 0xFF, 0x06, 0x01, 0x00, 0x00, 0x00, 0xFE, 0xFA, 0XEF};
 // next_cmd[6] = (uint8_t)(songNumber);
  send_cmd(next_cmd);
  //myDFPlayer.next();
  songNumber += 1;
}

void volumeUp() {
  volume += 2;
  p.neoPixelSetValue(volume, 0, 100, 0, true);
  //myDFPlayer.volume(volume);
}

void volumeDown() {
  volume -= 2;
  p.neoPixelSetValue(volume, 0, 100, 0, true);
  //myDFPlayer.volume(volume);
}

void nextFolder() {
  //folder +=1;
  //myDFPlayer.playFolder(1, 1);
}

void volumeControl (uint8_t volume) {
  static uint8_t volume_cmd [] = { 0X7E, 0xFF, 0x06, 0X06, 0x00, 0x00, 0x00, 0xFE, 0x00, 0XEF};
  volume_cmd[6] = (uint8_t)(volume);
  send_cmd(volume_cmd);
  delay(150);
}

  // folder = bit 5 && song = bit 6
void folderControl (uint8_t folder, uint8_t songNumber) {
  static uint8_t folder_cmd [] = { 0X7E, 0xFF, 0x06, 0x0F, 0x00, 0x00, 0x00, 0xFE, 0xE9, 0XEF};
  folder_cmd[5] = (uint8_t)(folder);
  folder_cmd[6] = (uint8_t)(songNumber);
  send_cmd(folder_cmd);
  delay(150);
}

// Send data to Mini MP3 player
void send_cmd (uint8_t *cmd) {
// find checksum
  uint8_t checksum = 0;
  for (int i=2; i<7; i++) { //add 2,3,4,5,6 (remeber array starts at ZERO)
    checksum += cmd[i];
    }
	checksum = (255 - checksum)+2;
  // insert into bit 8 of _cmd
  cmd[8] = (uint8_t)checksum;
	// send to MP3
  for (int i=0; i<10; i++) { //send cmd
		(Serial1).write (cmd[i]);
	  }
  }

void triggerStage() {
  p.neoPixelFill(10, 0, 0, true);
}

void commandStage() {
  p.neoPixelFill(200, 0, 0, true);
}

// Callback function for VR engine
void VRCallback(int nFlag, int nID, int nScore, int nSG, int nEnergy) {
  if (nFlag == DSpotterSDKHL::InitSuccess) {
    //ToDo
  } else if (nFlag == DSpotterSDKHL::GetResult) {
    switch (nID) {
      case opencamera:
        //volumeUp();
        nextFolder();
        //fireworks();
        break;
      case takepicture:
        volumeDown();
        break;
      case playmusic:
        playMusic();
        break;
      case stopmusic:
        stopMusic();
        break;
      case previousmusic:
        previousMusic();
        break;
      case nextmusic:
        nextMusic();
        break;
      default:
        break;
    }
  }

  else if (nFlag == DSpotterSDKHL::ChangeStage) {
    switch (nID) {
      case DSpotterSDKHL::TriggerStage:
        triggerStage();
        break;
      case DSpotterSDKHL::CommandStage:
        commandStage();
        break;
      default:
        break;
    }
  } else if (nFlag == DSpotterSDKHL::GetError) {
    if (nID == DSpotterSDKHL::LicenseFailed) {
      for (int num = 0; num < 13; num++) {
        p.neoPixelSetValue(num, 200, 0, 0, true);
        delay(750);
      }
    }
    g_oDSpotterSDKHL.Release();
    while (1)
      ;  //hang loop
  } else if (nFlag == DSpotterSDKHL::LostRecordFrame) {
    //ToDo
  }
}