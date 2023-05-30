#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <videodriver.h>
#include <idtLoader.h>
#include <sound.h>
#include "boca.h"

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	char buffer[10];

	ncPrint("[x64BareBones]");
	ncNewline();

	ncPrint("CPU Vendor:");
	ncPrint(cpuVendor(buffer));
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();

	clearBSS(&bss, &endOfKernel - &bss);

	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
	ncNewline();
	ncNewline();
	return getStackBase();
}

int main()
{	
/* 	// drawWhiteLine();
	// drawRect(10, 0, 5, 5);
	printString("Lorem Ipsum es simplemente el texto de relleno de las imprentas y archivos de texto. Lorem Ipsum ha sido el texto de relleno estandar de las industrias desde el ano 1500, ");
	printLn("");
	printString("Dignissim cras tincidunt lobortis feugiat vivamus. Amet est placerat in egestas erat imperdiet. Neque ornare aenean euismod elementum nisi quis eleifend. Dignissim cras tincidunt lobortis feugiat vivamus. Amet est placerat in egestas erat imperdiet. Neque ornare aenean euismod elementum nisi quis eleifend.");
	printString("Hola\nEsto\nes\nuna\nprueba\n");
	printLn("Hola\nEsto\nes\nuna\nprueba"); */

	load_idt(); //Setup idt before terminal runs
	// beep();
	drawImage(diego, 100, 100);
	playBSong();
	while (1) {}
	((EntryPoint)sampleCodeModuleAddress)(); //Calling sampleCodeModule's main address
	return 0;
}
