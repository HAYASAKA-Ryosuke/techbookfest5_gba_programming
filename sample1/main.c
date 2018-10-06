typedef unsigned short uint16;
typedef unsigned int uint32;

#define SCREEN_WIDTH 240

volatile uint16 *vram = (volatile uint16 *)0x06000000;
volatile uint16 *ioram = (volatile uint16 *)0x04000000;

uint32 x = 30;
uint32 y = 20;

int main(void){

	*ioram = 0x0403;

	vram[y * SCREEN_WIDTH + x] = 0x001F;
	x = 35;
	y = 20;
	vram[y * SCREEN_WIDTH + x] = 0x03E0;
	x = 35;
	y = 35;
	vram[y * SCREEN_WIDTH + x] = 0x7C00;
	x = 30;
	y = 35;
	vram[y * SCREEN_WIDTH + x] = 0xFFFF;

	while(1){}

	return 0;
}
