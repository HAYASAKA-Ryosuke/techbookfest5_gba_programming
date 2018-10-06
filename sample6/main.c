typedef unsigned short uint16;
typedef unsigned int uint32;

volatile uint32 *dma1_src = (uint32 *)0x040000BC;
volatile uint32 *dma1_dst = (uint32 *)0x040000C0;
volatile uint16 *dma1_size = (uint16 *)0x040000C4;
volatile uint16 *dma1_enable = (uint16 *)0x040000C6;
volatile uint16 *master_sound_enable = (uint16 *)0x04000084;
volatile uint16 *dma_sound = (uint16 *)0x04000082;
volatile uint16 *timer0_counter = (uint16 *)0x04000100;
volatile uint16 *timer0_control = (uint16 *)0x04000102;

char sound_datas[10000];
uint16 freq = 440;

int main(){
    for(int i = 0; i< 10000; i+=4){
        sound_datas[i] = 0;
        sound_datas[i + 1] = 127;
        sound_datas[i + 2] = 0;
        sound_datas[i + 3] = -127;
    }

    *master_sound_enable = 0x0080; // 全サウンドスイッチをON
    *dma_sound = 0x0304; // 左チャンネルと右チャンネル出力・ボリューム最大

    *dma1_src = (uint32)sound_datas;
    *dma1_dst = (uint32)0x040000A0;
    *dma1_size = 2; // 送信したいデータの長さは4バイト．2バイトずつ転送するため4/2=2

    *timer0_counter = 65536 - 16777216 / freq; // カウント開始の値を設定
    *dma1_enable = 0xB200; // 16bit長ずつ転送・リピート有効・転送タイミングはFIFOバッファ・DMA開始
    *timer0_control = 0x0080; // タイマ開始

    while(1);

    return 0;
}
