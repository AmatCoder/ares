#include "cpuregs.h"

class CPU {
public:
MMIO *mmio;
CPURegs regs;
  enum {
    FLAG_N = 0x80, FLAG_V = 0x40,
    FLAG_M = 0x20, FLAG_X = 0x10,
    FLAG_D = 0x08, FLAG_I = 0x04,
    FLAG_Z = 0x02, FLAG_C = 0x01
  };
  virtual uint8 pio_status() = 0;
  virtual void  run() = 0;
  virtual void  scanline() = 0;
  virtual void  frame() = 0;
  virtual void  power() = 0;
  virtual void  reset() = 0;

//opcode disassembler
  enum {
    OPTYPE_DP = 0,    //dp
    OPTYPE_DPX,       //dp,x
    OPTYPE_DPY,       //dp,y
    OPTYPE_IDP,       //(dp)
    OPTYPE_IDPX,      //(dp,x)
    OPTYPE_IDPY,      //(dp),y
    OPTYPE_ILDP,      //[dp]
    OPTYPE_ILDPY,     //[dp],y
    OPTYPE_ADDR,      //addr
    OPTYPE_ADDRX,     //addr,x
    OPTYPE_ADDRY,     //addr,y
    OPTYPE_IADDRX,    //(addr,x)
    OPTYPE_ILADDR,    //[addr]
    OPTYPE_LONG,      //long
    OPTYPE_LONGX,     //long, x
    OPTYPE_SR,        //sr,s
    OPTYPE_ISRY,      //(sr,s),y
    OPTYPE_ADDR_PC,   //pbr:addr
    OPTYPE_IADDR_PC   //pbr:(addr)
  };
  void   disassemble_opcode(char *output);
  uint32 resolve_offset(uint8 offset_type, uint32 addr);
  uint8  opcode_length();
  uint16 __relb(int8 offset);
  uint16 __relw(int16 offset);

  CPU();
};
