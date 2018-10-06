typedef unsigned short uint16;
typedef unsigned int uint32;

#define SCREEN_WIDTH    240

volatile uint16 *vram = (volatile uint16 *)0x06000000;
volatile uint16 *ioram = (volatile uint16 *)0x04000000;
volatile uint16 *timer0_counter = (uint16 *)0x04000100;
volatile uint16 *timer0_control = (uint16 *)0x04000102;
volatile uint16 *interrupt_ie = (uint16 *)0x04000200;  // 割り込み許可レジスタ(どの割り込みを行なうか)
volatile uint16 *interrupt_if = (uint16 *)0x04000202; // 割り込みリクエストフラグ
volatile uint16 *interrupt_ime = (uint16 *)0x04000208; // 割り込みマスタの許可レジスタ
volatile uint32 *interrupt_hook = (uint32 *)0x03007FFC;

uint32 x = 30, y = 20;
uint16 color = 0x001F;
uint32 color_flg = 0;

void interrupt_function(void){
   uint32 flag = 0;
   *interrupt_ime = 0;
   flag = *interrupt_if;
   if(flag & 0x0008){
       if(color_flg)
           color = 0x7C00;
       else
           color = 0x001F;

       vram[y * SCREEN_WIDTH + x] = color;
       color_flg ^= 1; // 1とXORを取って0と1を交互に繰り返す
   }
   *interrupt_if = flag;
   *interrupt_ime = 1;
}

int main(){

    *ioram = 0x0403;

    *interrupt_ime = 0;
    *interrupt_hook = (uint32)interrupt_function;

    *timer0_counter = 0;
    *timer0_control = 0x00C2; // 割り込み有効・タイマ開始

    *interrupt_ie |= 0x0008; // タイマ0がオーバフローしたら割り込み
    *interrupt_ime = 1;

    while(1){}

    return 0;
}
