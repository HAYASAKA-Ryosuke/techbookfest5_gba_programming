#include <SPI.h>
#include <FS.h>
#include <SPIFFS.h>

uint32_t crc = 0xc387;
uint32_t hh, rr, encryptionseed;
uint32_t gba_file_length = 0;

void find_gba_and_recognition() {
  uint32_t read_from_gba = SPI.transfer32(0x00006202);
  while (read_from_gba != 0x72026202)
  {
    delay(20);
    read_from_gba = SPI.transfer32(0x00006202);
    Serial.print("Finding for GBA\r\n");
  }

  SPI.transfer32(0x00006202);
  Serial.print("Found GBA\r\n");

  SPI.transfer32(0x00006102);
  Serial.print("Recognition ok\r\n");
}

void send_headers(File fp) {
  for (int32_t i = 0; i <= 0x5f; i++) {
    uint32_t header = fp.read();
    header |= (fp.read() << 8);
    SPI.transfer32(0x0000ffff & header);
    delay(10);
  }

  SPI.transfer32(0x00006200);
  Serial.println("Transfer of header data complete");

}

void send_gba_data(File fp) {
  for (uint32_t position = 0xc0; position < gba_file_length; position += 4) {
    uint32_t gba_data = fp.read();
    gba_data |= fp.read() << 8;
    gba_data |= fp.read() << 16;
    gba_data |= fp.read() << 24;
    crc = calc_crc(gba_data);
    SPI.transfer32(encryption(gba_data, position));
    delay(10);
  }
}

void wait_response_crc_from_gba() {
  while (SPI.transfer32(0x00000065) != 0x00750065)
  {
    delay(20);
    Serial.print("Wait for GBA to respond with CRC\n\r");
  }
}

uint32_t final_crc(uint32_t data) {
  uint32_t crctemp = ((((data & 0xFF00) + rr) << 8) | 0xFFFF0000) + hh;
  crc = calc_crc(crctemp);
  return crc;
}

uint32_t calc_crc(uint32_t data)
{
  for (int32_t bit = 0 ; bit < 32 ; bit++)
  { 
    if ((crc ^ data) & 0x01)
      crc = (crc >> 1) ^ 0xc37b;
    else
      crc = (crc >> 1);
    data >>= 1;
  }
  return crc;
}

uint32_t encryption(uint32_t data, uint32_t ptr) {
  ptr = ~(ptr + 0x02000000) + 1;
  encryptionseed = (encryptionseed * 0x6F646573) + 1;

  data = (encryptionseed ^ data) ^ (ptr ^ 0x43202F2F);
  return data;
}

void init_cryption() {
  uint32_t read_from_gba = 0;
  read_from_gba = SPI.transfer32(0x000063d1);
  Serial.print("Send encryption key\r\n");

  read_from_gba = SPI.transfer32(0x000063d1);
  Serial.print("Encryption key received 0x73hh****\r\n");

  uint32_t data = read_from_gba >> 16;
  encryptionseed = ((data & 0x000000FF) << 8) | 0x0FFFF00d1;
  hh = ((data & 0x000000FF) + 0x0000020F) & 0x000000FF;

  Serial.print("Seed:"); Serial.print(encryptionseed, HEX);
  Serial.print(" hh:"); Serial.println(hh, HEX);

  SPI.transfer32(0x6400 | hh);
  Serial.println("Encryption confirmation");

  read_from_gba = SPI.transfer32(((gba_file_length - 0xC0) >> 2) - 0x34);
  Serial.println("Length information exchange");

  rr = (read_from_gba >> 16) & 0xff;

}

void setup() {
  Serial.begin(115200);
  
  pinMode(MISO, HIGH);
  pinMode(MOSI, HIGH);
  pinMode(SCK, HIGH);
  pinMode(MISO, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(SCK, OUTPUT);

  SPI.begin();
  SPI.setFrequency(3000000);
  SPI.setDataMode(SPI_MODE3);
  
  while (!Serial);

  find_gba_and_recognition();

  SPIFFS.begin(true);
  File fp = SPIFFS.open("/main.gba", FILE_READ);
  gba_file_length = fp.size();

  send_headers(fp);

  SPI.transfer32(0x00006202);
  Serial.println("Exchange master/slave info again");

  init_cryption();
  Serial.println("init cryption");

  send_gba_data(fp);
  Serial.println("Transfer of gba data");
  fp.close();

  wait_response_crc_from_gba();
  Serial.println("GBA ready with CRC");

  SPI.transfer32(0x00000066);//EOF Signal
  Serial.println("Let's exchange CRC!");
  
  uint32_t read_from_gba = SPI.transfer32(final_crc(read_from_gba));
  Serial.print("CRC is:"); Serial.println(crc, HEX);
  Serial.print("Actual:"); Serial.println(read_from_gba, HEX);

  SPI.end();
  pinMode(SS, INPUT);
  pinMode(MISO, INPUT);
  pinMode(MOSI, INPUT);
  pinMode(SCK, INPUT);
}


void loop() {

}
