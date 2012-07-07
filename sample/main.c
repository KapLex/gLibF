#include <pspkernel.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../glib2d.h"
#include "../glibf.h"


SceCtrlData input;

int SetupCallbacks();
void show_errmsg(const char *errmsg);

PSP_MODULE_INFO("glibF Sample", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);


int main() {

	SetupCallbacks();

	if(!gfLoadFont("times.ttf")) {
		show_errmsg("Could not load font file.");
		return 1;
	}

        g2dClear(BLACK);

	gfSetFontColor(0xff00ffff);	// yellow
	gfSetFontSize(24);		// 12 point
	gfSetFontAngle(0.0);		// no angle
	gfTextToScreen("gLib2D with flib", 50, 50);

	gfSetFontColor(0xff00ff00);	// green
	gfSetFontSize(16);		// 16 point
	gfSetFontAngle(45.0);		// rotate 45 degrees counter-clockwise
	gfTextToScreen("TTF font loaded", 50, 100);

	sceDisplayWaitVblankStart();
	g2dFlip(G2D_VSYNC);

	sceCtrlReadBufferPositive(&input, 1);
	while(!input.Buttons) {
		sceKernelDelayThread(10000);	// wait 10 milliseconds
		sceCtrlReadBufferPositive(&input, 1);
	}

	gfUnloadFont();
	sceKernelExitGame();
	return 0;
}

void show_errmsg(const char *errmsg) {
	pspDebugScreenInit();
	pspDebugScreenSetXY(0, 2);
	pspDebugScreenPrintf("\n\n%s\n\nPress START to continue.", errmsg);
	while(!(input.Buttons & PSP_CTRL_START))
		sceCtrlReadBufferPositive(&input, 1);
}

int exit_callback(int arg1, int arg2, void *common) {
	sceKernelExitGame();
	return 0;
}

int CallbackThread(SceSize args, void *argp) {
	int cbid;
	cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();
	return 0;
}

int SetupCallbacks(void) {
	int thid = 0;
	thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
	if(thid >= 0) {
		sceKernelStartThread(thid, 0, 0);
	}
	return thid;
}


