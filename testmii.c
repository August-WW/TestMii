#include <gccore.h>
#include <wiiuse/wpad.h>
#include <stdio.h>
#include <stdlib.h>
#include <asndlib.h>
#include <mp3player.h>
#include "sample_mp3.h"


GXRModeObj *rmode;
void *framebuffer;

// Function to initialize the video
void init_video() {
    VIDEO_Init();
    rmode = VIDEO_GetPreferredMode(NULL);
    framebuffer = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
    console_init(framebuffer, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);
    VIDEO_Configure(rmode);
    VIDEO_SetNextFramebuffer(framebuffer);
    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    if (rmode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();
}

// Function to initialize the audio subsystem
void init_audio() {
	ASND_Init();
	MP3Player_Init();
	
	MP3Player_PlayBuffer(sample_mp3, sample_mp3_size, NULL);
}


// Function to display button states
void display_buttons(u32 buttons, u32 nunchuk_buttons) {
    printf("\x1b[2J"); // Clear the screen
    printf("\x1b[3;0H"); // Move cursor to home position

    printf("TestMii - Wii Remote Button Test\n");
    printf("========================================\n");
    printf("Made by August Wolf - Press HOME to exit\n\n");

    // Wii Remote buttons
    if (buttons & WPAD_BUTTON_A) printf("A pressed\n");
    if (buttons & WPAD_BUTTON_B) printf("B pressed\n");
    if (buttons & WPAD_BUTTON_1) printf("1 pressed\n");
    if (buttons & WPAD_BUTTON_2) printf("2 pressed\n");
    if (buttons & WPAD_BUTTON_PLUS) printf("+ pressed\n");
    if (buttons & WPAD_BUTTON_MINUS) printf("- pressed\n");
    if (buttons & WPAD_BUTTON_HOME) printf("HOME pressed. Bye!\n");
    if (buttons & WPAD_BUTTON_UP) printf("UP pressed\n");
    if (buttons & WPAD_BUTTON_DOWN) printf("DOWN pressed\n");
    if (buttons & WPAD_BUTTON_LEFT) printf("LEFT pressed\n");
	if (buttons & WPAD_BUTTON_RIGHT) printf("RIGHT pressed\n");

    // Nunchuk buttons
    if (nunchuk_buttons & WPAD_NUNCHUK_BUTTON_C) printf("Nunchuk C pressed\n");
    if (nunchuk_buttons & WPAD_NUNCHUK_BUTTON_Z) printf("Nunchuk Z pressed\n");
}

int main() {
    init_video();
    WPAD_Init();
	ASND_Init();
	MP3Player_Init();
	
	MP3Player_PlayBuffer(sample_mp3, sample_mp3_size, NULL);

    while (1) {
        WPAD_ScanPads();
        u32 buttonsDown = WPAD_ButtonsDown(0);
        u32 buttonsHeld = WPAD_ButtonsHeld(0);

        WPADData *wpadData = WPAD_Data(0);
        u32 nunchuk_buttons = 0;

        if (wpadData->exp.type == WPAD_EXP_NUNCHUK) {
            nunchuk_buttons = wpadData->btns_h;
        }

        display_buttons(buttonsHeld, nunchuk_buttons);

        if (buttonsDown & WPAD_BUTTON_HOME) break;

        VIDEO_WaitVSync();
    }

    exit(0);
}
