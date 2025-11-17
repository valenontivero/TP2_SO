// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <videodriver.h>
#include <idtLoader.h>
#include <sound.h>
#include <colors.h>
#include <homero.h>
#include <scheduler.h>
#include <processManager.h>
#include <mem_manager.h>
#include <keyboard.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

extern void save_original_regs();

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;
static void * const startMemoryAddress = (void*)0x600000;
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
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	clearBSS(&bss, &endOfKernel - &bss);
	return getStackBase();
}

int main()
{	
	load_idt(); //Setup idt before terminal runs
	
	//drawImage(homero, 100, 100);
	/* printStringColor("Press any key to start. If not found, press CTRRRRL \n\n", YELLOW); */
	//playSimpsons();

	save_original_regs();

	// Inicializar memoria en dirección fija (0x600000) en lugar de endOfKernel
	startMemoryManager(startMemoryAddress, 0x2000000); //32MB for the heap starting at 0x600000
	initKeyboard();
	initializeProcesses();
	initScheduler(getStackBase());
	char* argv[]= {0};
	createFirstProcess((void*)sampleCodeModuleAddress,0,argv);
	/* ((EntryPoint)sampleCodeModuleAddress)(); */ //Calling sampleCodeModule's main address
	while (1){}
	
	return 0;
}