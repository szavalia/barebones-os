// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <string.h>
#include "lib.h"
#include "moduleLoader.h"
#include "idtLoader.h"
#include "video_driver.h"
#include "time.h"
#include "keyboard.h"
extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

int side = 0, context=0;

extern uint64_t stackBase;
static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;
typedef int (*EntryPoint)();

extern void saveInitRegs( uint64_t rsp);


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
	splitScreen();
	char vendor[13], brand[49]; //FIXME: magic numbers raros
	printS("[x64BareBones]");
	newline();
	cpuVendor(vendor);
	printS(vendor);
	newline();
	cpuBrand(brand);
	printS(brand);
	newline();	
	printS("[Loading modules]");
	newline();
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	printS("[Done]");
	newline();
	newline();

	printS("[Initializing kernel's binary]");
	newline();

	clearBSS(&bss, &endOfKernel - &bss);
	
	printS("  text: 0x");
	printHex((uint64_t)&text);
	newline();
	printS("  rodata: 0x");
	printHex((uint64_t)&rodata);
	newline();
	printS("  data: 0x");
	printHex((uint64_t)&data);
	newline();
	printS("  bss: 0x");
	printHex((uint64_t)&bss);
	newline();
	stackBase = getStackBase();
	printS("  Stack base: 0x");
	printHex(stackBase);
	newline();
	printS("[Done]");
	newline();
	return stackBase;
}


int main()
{
	load_idt();
	printS("[Kernel Main]");
	newline();
	printS("  Sample code module at 0x");
	printHex((uint64_t)sampleCodeModuleAddress);
	newline();
	printS("  Calling the sample code module returned: ");
	//clear();
	saveInitRegs(stackBase);
	printHex(((EntryPoint)sampleCodeModuleAddress)()); //acá llamo a main de userland
	newline();
	newline();
	printS(" Sample data module at 0x");
	printHex(((EntryPoint)sampleDataModuleAddress)());
	newline();
	printS(" Sample data module contents: ");
	printS((char*)sampleDataModuleAddress);
	newline();
	printS("[Finished]");
	newline();
	while(1);
	return 0;
}