/*
  $211a : mode7 settings register
  ab0000yx

  ab:
   00 = use screen repetition if outside screen area
   01 = ???
   10 = use character 0x00 repetition if outside screen area
   11 = use back color if outside screen area
  y: vertical screen flip
  x: horizontal screen flip
*/
void mmio_w211a(byte value) {
  ppu.mode7_vflip = (value & 0x02)?true:false;
  ppu.mode7_hflip = (value & 0x01)?true:false;
}

byte m7_latch = 0x00;

/*
  $211b : m7a / 16-bit source operand for signed multiplication
*/
void mmio_w211b(byte value) {
static byte latch = 0;
  ppu.m7a  = (value << 8) | m7_latch;
  m7_latch = value;

  if(!latch) { ppu.smul_a = (ppu.smul_a & 0xff00) | value;        }
  else       { ppu.smul_a = (ppu.smul_a & 0x00ff) | (value << 8); }

  latch ^= 1;
}

/*
  $211c : m7b / 8-bit source operand for signed multiplication
*/
void mmio_w211c(byte value) {
  ppu.m7b  = (value << 8) | m7_latch;
  m7_latch = value;

  ppu.smul_b = value;
}

/*
  $211d : m7c
*/
void mmio_w211d(byte value) {
  ppu.m7c  = (value << 8) | m7_latch;
  m7_latch = value;
}

/*
  $211e : m7d
*/
void mmio_w211e(byte value) {
  ppu.m7d  = (value << 8) | m7_latch;
  m7_latch = value;
}

/*
  $211f : m7x
*/
void mmio_w211f(byte value) {
  ppu.m7x  = (value << 8) | m7_latch;
  m7_latch = value;
}

/*
  $2120 : m7y
*/
void mmio_w2120(byte value) {
  ppu.m7y  = (value << 8) | m7_latch;
  m7_latch = value;
}
