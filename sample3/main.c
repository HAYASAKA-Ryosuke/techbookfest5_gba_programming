typedef unsigned short uint16;
typedef unsigned int uint32;

#define SCREEN_WIDTH 240
#define A_BUTTON    0x0001
#define B_BUTTON    0x0002
#define SELECT_BUTTON    0x0004
#define START_BUTTON    0x0008
#define RIGHT_BUTTON    0x0010
#define LEFT_BUTTON    0x0020
#define TOP_BUTTON    0x0040
#define DOWN_BUTTON    0x0080
#define R_BUTTON    0x0100
#define L_BUTTON    0x0200

volatile uint16 *vram = (uint16 *)0x06000000;
volatile uint16 *ioram = (uint16 *)0x04000000;
volatile uint16 *input_key = (uint16 *)0x04000130;
volatile uint16 *vcount = (uint16 *)0x04000006;

int main(void){
    uint32 x = 5;
    uint32 y = 20;
    uint32 old_x = 5;
    uint32 old_y = 20;

	*ioram = 0x0403;
	vram[y * SCREEN_WIDTH + x] = 0x001F;

	while(1){
        while(*vcount >= 160);
        while(*vcount < 160);

        uint16 input_value = *input_key ^ 0x03FF;
        if((input_value & LEFT_BUTTON) != 0)
            x -= 1;
        if((input_value & RIGHT_BUTTON) != 0)
            x += 1;
        if((input_value & TOP_BUTTON) != 0)
            y -= 1;
        if((input_value & DOWN_BUTTON) != 0)
            y += 1;

        vram[old_y * SCREEN_WIDTH + old_x] = 0x0000;
        vram[y * SCREEN_WIDTH + x] = 0x001F;
        old_x = x;
        old_y = y;
    }

	return 0;
}
