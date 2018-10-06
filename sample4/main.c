typedef unsigned short uint16;
typedef unsigned int uint32;

#define SCREEN_WIDTH    240

volatile uint16 *vram = (volatile uint16 *)0x06000000;
volatile uint16 *ioram = (volatile uint16 *)0x04000000;
volatile uint16 *timer0_counter = (uint16 *)0x04000100;
volatile uint16 *timer0_control = (uint16 *)0x04000102;

uint32 x = 30, y = 20;
uint16 color;

int main(){
    *ioram = 0x0403;
    color = 0x001F;

    *timer0_counter = 0;
    *timer0_control = 0x0082;  // タイマ開始・分周を256

    while(1){
        if(*timer0_counter<32768)  // 65535まで約1秒かかるのでだいたい半分くらいで判定
            color = 0x7C00; // 青
        else
            color = 0x001F; // 赤
        vram[y * SCREEN_WIDTH + x] = color;
    };

    return 0;
}
