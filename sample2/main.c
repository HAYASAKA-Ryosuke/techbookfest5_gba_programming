typedef unsigned short uint16;
typedef unsigned int uint32;

#define SCREEN_WIDTH 240

volatile uint16 *vram = (uint16 *)0x06000000;
volatile uint16 *ioram = (uint16 *)0x04000000;
volatile uint16 *vcount = (uint16 *)0x04000006;

int main(void){
    uint32 red_x = 0, red_y = 20;
    uint32 blue_x = 0, blue_y = 35;
    uint32 green_x = 0, green_y = 75;
    uint32 white_x = 0, white_y = 95;

	*ioram = 0x0403;
	vram[red_y * SCREEN_WIDTH + red_x] = 0x001F;
	vram[blue_y * SCREEN_WIDTH + blue_x] = 0x03E0;
	vram[green_y * SCREEN_WIDTH + green_x] = 0x7C00;
	vram[white_y * SCREEN_WIDTH + white_x] = 0xFFFF;

	while(1){
        while(*vcount >= 160){}
        while(*vcount < 160){}

        vram[red_y * SCREEN_WIDTH + red_x] = 0x0000;
        red_x += 1;
        vram[red_y * SCREEN_WIDTH + red_x] = 0x001F;

        vram[blue_y * SCREEN_WIDTH + blue_x] = 0x0000;
        blue_x += 1;
        vram[blue_y * SCREEN_WIDTH + blue_x] = 0x03E0;

        vram[green_y * SCREEN_WIDTH + green_x] = 0x0000;
        green_x += 1;
        vram[green_y * SCREEN_WIDTH + green_x] = 0x7C00;

        vram[white_y * SCREEN_WIDTH + white_x] = 0x0000;
        white_x += 1;
        vram[white_y * SCREEN_WIDTH + white_x] = 0xFFFF;

        if(red_x == SCREEN_WIDTH - 1){
            vram[red_y * SCREEN_WIDTH + red_x] = 0x0000;
            red_x = 0;
        }
        if(blue_x == SCREEN_WIDTH - 1){
            vram[blue_y * SCREEN_WIDTH + blue_x] = 0x0000;
            blue_x = 0;
        }
        if(green_x == SCREEN_WIDTH - 1){
            vram[green_y * SCREEN_WIDTH + green_x] = 0x0000;
            green_x = 0;
        }
        if(white_x == SCREEN_WIDTH - 1){
            vram[white_y * SCREEN_WIDTH + white_x] = 0x0000;
            white_x = 0;
        }
    }
	return 0;
}
