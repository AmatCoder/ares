//this is a virtual function.
//in opcode-based CPU emulators, the main emulation routine
//will only be able to call the disassemble_opcode() function
//on clean opcode edges. but with cycle-based CPU emulators,
//the CPU may be in the middle of executing an opcode when the
//emulator (e.g. debugger) wants to disassemble an opcode. this
//would mean that important registers may not reflect what they
//did at the start of the opcode (especially regs.pc), so in
//cycle-based emulators, this function should be overridden to
//reflect whether or not an opcode has only been partially
//executed. if not, the debugger should abort attempts to skip,
//disable, or disassemble the current opcode.
bool CPU::in_opcode() { return false; }

uint16 CPU::__relb(int8 offset) {
uint32 addr;
  addr = (regs.pc.d & 0xff0000) | ((regs.pc.d + 2) & 0xffff);
  return addr + offset;
}

uint16 CPU::__relw(int16 offset) {
uint32 addr;
  addr = (regs.pc.d & 0xff0000) | ((regs.pc.d + 3) & 0xffff);
  return addr + offset;
}

uint32 CPU::resolve_offset(uint8 offset_type, uint32 addr) {
uint32 r = 0;
  switch(offset_type) {
  case OPTYPE_DP:
    r = (regs.d + (addr & 0xffff)) & 0xffff;
    break;
  case OPTYPE_DPX:
    r = (regs.d + regs.x + (addr & 0xffff)) & 0xffff;
    break;
  case OPTYPE_DPY:
    r = (regs.d + regs.y + (addr & 0xffff)) & 0xffff;
    break;
  case OPTYPE_IDP:
    addr = (regs.d + (addr & 0xffff)) & 0xffff;
    r = (regs.db << 16) + r_mem->read_word(addr);
    break;
  case OPTYPE_IDPX:
    addr = (regs.d + regs.x + (addr & 0xffff)) & 0xffff;
    r = (regs.db << 16) + r_mem->read_word(addr);
    break;
  case OPTYPE_IDPY:
    addr = (regs.d + (addr & 0xffff)) & 0xffff;
    r = (regs.db << 16) + r_mem->read_word(addr) + regs.y;
    break;
  case OPTYPE_ILDP:
    addr = (regs.d + (addr & 0xffff)) & 0xffff;
    r = r_mem->read_long(addr);
    break;
  case OPTYPE_ILDPY:
    addr = (regs.d + (addr & 0xffff)) & 0xffff;
    r = r_mem->read_long(addr) + regs.y;
    break;
  case OPTYPE_ADDR:
    r = (regs.db << 16) + (addr & 0xffff);
    break;
  case OPTYPE_ADDR_PC:
    r = (regs.pc.b << 16) + (addr & 0xffff);
    break;
  case OPTYPE_ADDRX:
    r = (regs.db << 16) + (addr & 0xffff) + regs.x;
    break;
  case OPTYPE_ADDRY:
    r = (regs.db << 16) + (addr & 0xffff) + regs.y;
    break;
  case OPTYPE_IADDR_PC:
    r = (regs.pc.b << 16) + (addr & 0xffff);
    break;
  case OPTYPE_IADDRX:
    r = (regs.pc.b << 16) + ((addr + regs.x) & 0xffff);
    break;
  case OPTYPE_ILADDR:
    r = addr & 0xffffff;
    break;
  case OPTYPE_LONG:
    r = addr & 0xffffff;
    break;
  case OPTYPE_LONGX:
    r = (addr + regs.x) & 0xffffff;
    break;
  case OPTYPE_SR:
    r = (regs.s + (addr & 0xff)) & 0xffff;
    break;
  case OPTYPE_ISRY:
    addr = (regs.s + (addr & 0xff)) & 0xffff;
    r = (regs.db << 16) + r_mem->read_word(addr) + regs.y;
    break;
  }
  return r;
}

void CPU::disassemble_opcode(char *output) {
char *s;
char t[256];
uint8 op, op0, op1, op2;
static CPUReg24 pc;
  s = output;

  if(in_opcode() == true) {
    strcpy(s, "?????? <CPU within opcode>");
    return;
  }

  pc.d = regs.pc.d;
  sprintf(s, "%0.6x ", pc.d);

  op  = r_mem->read(pc.d); pc.w++;
  op0 = r_mem->read(pc.d); pc.w++;
  op1 = r_mem->read(pc.d); pc.w++;
  op2 = r_mem->read(pc.d);

  switch(op) {
  case 0x00:sprintf(t, "brk #$%0.2x               ", op0);break;
  case 0x01:sprintf(t, "ora ($%0.2x,x)   [$%0.6x]", op0, resolve_offset(OPTYPE_IDPX, op0));break;
  case 0x02:sprintf(t, "cop #$%0.2x               ", op0);break;
  case 0x03:sprintf(t, "ora $%0.2x,s     [$%0.6x]", op0, resolve_offset(OPTYPE_SR, op0));break;
  case 0x04:sprintf(t, "tsb $%0.2x       [$%0.6x]", op0, resolve_offset(OPTYPE_DP, op0));break;
  case 0x05:sprintf(t, "ora $%0.2x       [$%0.6x]", op0, resolve_offset(OPTYPE_DP, op0));break;
  case 0x06:sprintf(t, "asl $%0.2x       [$%0.6x]", op0, resolve_offset(OPTYPE_DP, op0));break;
  case 0x07:sprintf(t, "ora [$%0.2x]     [$%0.6x]", op0, resolve_offset(OPTYPE_ILDP, op0));break;
  case 0x08:sprintf(t, "php                    ");break;
  case 0x09:
    if(regs.p.m)sprintf(t, "ora #$%0.2x               ", op0);
    else        sprintf(t, "ora #$%0.4x             ", op0|op1<<8);break;
  case 0x0a:sprintf(t, "asl a                  ");break;
  case 0x0b:sprintf(t, "phd                    ");break;
  case 0x0c:sprintf(t, "tsb $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR, (op0|op1<<8)));break;
  case 0x0d:sprintf(t, "ora $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR, (op0|op1<<8)));break;
  case 0x0e:sprintf(t, "asl $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR, (op0|op1<<8)));break;
  case 0x0f:sprintf(t, "ora $%0.6x   [$%0.6x]", op0|op1<<8|op2<<16, resolve_offset(OPTYPE_LONG, (op0|op1<<8|op2<<16)));break;
  case 0x10:sprintf(t, "bpl $%0.4x     [$%0.6x]", __relb(op0)&0xffff, __relb(op0)&0xffffff);break;
  case 0x11:sprintf(t, "ora ($%0.2x),y   [$%0.6x]", op0, resolve_offset(OPTYPE_IDPY, op0));break;
  case 0x12:sprintf(t, "ora ($%0.2x)     [$%0.6x]", op0, resolve_offset(OPTYPE_IDP, op0));break;
  case 0x13:sprintf(t, "ora ($%0.2x,s),y [$%0.6x]", op0, resolve_offset(OPTYPE_ISRY, op0));break;
  case 0x14:sprintf(t, "trb $%0.2x       [$%0.6x]", op0, resolve_offset(OPTYPE_DP, op0));break;
  case 0x15:sprintf(t, "ora $%0.2x,x     [$%0.6x]", op0, resolve_offset(OPTYPE_DPX, op0));break;
  case 0x16:sprintf(t, "asl $%0.2x,x     [$%0.6x]", op0, resolve_offset(OPTYPE_DPX, op0));break;
  case 0x17:sprintf(t, "ora [$%0.2x],y   [$%0.6x]", op0, resolve_offset(OPTYPE_ILDPY, op0));break;
  case 0x18:sprintf(t, "clc                    ");break;
  case 0x19:sprintf(t, "ora $%0.4x,y   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDRY, (op0|op1<<8)));break;
  case 0x1a:sprintf(t, "inc                    ");break;
  case 0x1b:sprintf(t, "tcs                    ");break;
  case 0x1c:sprintf(t, "trb $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR, (op0|op1<<8)));break;
  case 0x1d:sprintf(t, "ora $%0.4x,x   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDRX, (op0|op1<<8)));break;
  case 0x1e:sprintf(t, "asl $%0.4x,x   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDRX, (op0|op1<<8)));break;
  case 0x1f:sprintf(t, "ora $%0.6x,x [$%0.6x]", op0|op1<<8|op2<<16, resolve_offset(OPTYPE_LONGX, (op0|op1<<8|op2<<16)));break;
  case 0x20:sprintf(t, "jsr $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR_PC, (op0|op1<<8)));break;
  case 0x21:sprintf(t, "and ($%0.2x,x)   [$%0.6x]", op0, resolve_offset(OPTYPE_IDPX, op0));break;
  case 0x22:sprintf(t, "jsl $%0.6x   [$%0.6x]", op0|op1<<8|op2<<16, resolve_offset(OPTYPE_LONG, (op0|op1<<8|op2<<16)));break;
  case 0x23:sprintf(t, "and $%0.2x,s     [$%0.6x]", op0, resolve_offset(OPTYPE_SR, op0));break;
  case 0x24:sprintf(t, "bit $%0.2x       [$%0.6x]", op0, resolve_offset(OPTYPE_DP, op0));break;
  case 0x25:sprintf(t, "and $%0.2x       [$%0.6x]", op0, resolve_offset(OPTYPE_DP, op0));break;
  case 0x26:sprintf(t, "rol $%0.2x       [$%0.6x]", op0, resolve_offset(OPTYPE_DP, op0));break;
  case 0x27:sprintf(t, "and [$%0.2x]     [$%0.6x]", op0, resolve_offset(OPTYPE_ILDP, op0));break;
  case 0x28:sprintf(t, "plp                    ");break;
  case 0x29:
    if(regs.p.m)sprintf(t, "and #$%0.2x               ", op0);
    else        sprintf(t, "and #$%0.4x             ", op0|op1<<8);break;
  case 0x2a:sprintf(t, "rol a                  ");break;
  case 0x2b:sprintf(t, "pld                    ");break;
  case 0x2c:sprintf(t, "bit $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR, (op0|op1<<8)));break;
  case 0x2d:sprintf(t, "and $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR, (op0|op1<<8)));break;
  case 0x2e:sprintf(t, "rol $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR, (op0|op1<<8)));break;
  case 0x2f:sprintf(t, "and $%0.6x   [$%0.6x]", op0|op1<<8|op2<<16, resolve_offset(OPTYPE_LONG, (op0|op1<<8|op2<<16)));break;
  case 0x30:sprintf(t, "bmi $%0.4x     [$%0.6x]", __relb(op0)&0xffff, __relb(op0)&0xffffff);break;
  case 0x31:sprintf(t, "and ($%0.2x),y   [$%0.6x]", op0, resolve_offset(OPTYPE_IDPY, op0));break;
  case 0x32:sprintf(t, "and ($%0.2x)     [$%0.6x]", op0, resolve_offset(OPTYPE_IDP, op0));break;
  case 0x33:sprintf(t, "and ($%0.2x,s),y [$%0.6x]", op0, resolve_offset(OPTYPE_ISRY, op0));break;
  case 0x34:sprintf(t, "bit $%0.2x,x     [$%0.6x]", op0, resolve_offset(OPTYPE_DPX, op0));break;
  case 0x35:sprintf(t, "and $%0.2x,x     [$%0.6x]", op0, resolve_offset(OPTYPE_DPX, op0));break;
  case 0x36:sprintf(t, "rol $%0.2x,x     [$%0.6x]", op0, resolve_offset(OPTYPE_DPX, op0));break;
  case 0x37:sprintf(t, "and [$%0.2x],y   [$%0.6x]", op0, resolve_offset(OPTYPE_ILDPY, op0));break;
  case 0x38:sprintf(t, "sec                    ");break;
  case 0x39:sprintf(t, "and $%0.4x,y   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDRY, (op0|op1<<8)));break;
  case 0x3a:sprintf(t, "dec                    ");break;
  case 0x3b:sprintf(t, "tsc                    ");break;
  case 0x3c:sprintf(t, "bit $%0.4x,x   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDRX, (op0|op1<<8)));break;
  case 0x3d:sprintf(t, "and $%0.4x,x   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDRX, (op0|op1<<8)));break;
  case 0x3e:sprintf(t, "rol $%0.4x,x   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDRX, (op0|op1<<8)));break;
  case 0x3f:sprintf(t, "and $%0.6x,x [$%0.6x]", op0|op1<<8|op2<<16, resolve_offset(OPTYPE_LONGX, (op0|op1<<8|op2<<16)));break;
  case 0x40:sprintf(t, "rti                    ");break;
  case 0x41:sprintf(t, "eor ($%0.2x,x)   [$%0.6x]", op0, resolve_offset(OPTYPE_IDPX, op0));break;
  case 0x42:sprintf(t, "wdm                    ");break;
  case 0x43:sprintf(t, "eor $%0.2x,s     [$%0.6x]", op0, resolve_offset(OPTYPE_SR, op0));break;
  case 0x44:sprintf(t, "mvp $%0.2x,$%0.2x            ", op1, op0);break;
  case 0x45:sprintf(t, "eor $%0.2x       [$%0.6x]", op0, resolve_offset(OPTYPE_DP, op0));break;
  case 0x46:sprintf(t, "lsr $%0.2x       [$%0.6x]", op0, resolve_offset(OPTYPE_DP, op0));break;
  case 0x47:sprintf(t, "eor [$%0.2x]     [$%0.6x]", op0, resolve_offset(OPTYPE_ILDP, op0));break;
  case 0x48:sprintf(t, "pha                    ");break;
  case 0x49:
    if(regs.p.m)sprintf(t, "eor #$%0.2x               ", op0);
    else        sprintf(t, "eor #$%0.4x             ", op0|op1<<8);break;
  case 0x4a:sprintf(t, "lsr a                  ");break;
  case 0x4b:sprintf(t, "phk                    ");break;
  case 0x4c:sprintf(t, "jmp $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR_PC, (op0|op1<<8)));break;
  case 0x4d:sprintf(t, "eor $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR, (op0|op1<<8)));break;
  case 0x4e:sprintf(t, "lsr $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR, (op0|op1<<8)));break;
  case 0x4f:sprintf(t, "eor $%0.6x   [$%0.6x]", op0|op1<<8|op2<<16, resolve_offset(OPTYPE_LONG, (op0|op1<<8|op2<<16)));break;
  case 0x50:sprintf(t, "bvc $%0.4x     [$%0.6x]", __relb(op0)&0xffff, __relb(op0)&0xffffff);break;
  case 0x51:sprintf(t, "eor ($%0.2x),y   [$%0.6x]", op0, resolve_offset(OPTYPE_IDPY, op0));break;
  case 0x52:sprintf(t, "eor ($%0.2x)     [$%0.6x]", op0, resolve_offset(OPTYPE_IDP, op0));break;
  case 0x53:sprintf(t, "eor ($%0.2x,s),y [$%0.6x]", op0, resolve_offset(OPTYPE_ISRY, op0));break;
  case 0x54:sprintf(t, "mvn $%0.2x,$%0.2x            ", op1, op0);break;
  case 0x55:sprintf(t, "eor $%0.2x,x     [$%0.6x]", op0, resolve_offset(OPTYPE_DPX, op0));break;
  case 0x56:sprintf(t, "lsr $%0.2x,x     [$%0.6x]", op0, resolve_offset(OPTYPE_DPX, op0));break;
  case 0x57:sprintf(t, "eor [$%0.2x],y   [$%0.6x]", op0, resolve_offset(OPTYPE_ILDPY, op0));break;
  case 0x58:sprintf(t, "cli                    ");break;
  case 0x59:sprintf(t, "eor $%0.4x,y   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDRY, (op0|op1<<8)));break;
  case 0x5a:sprintf(t, "phy                    ");break;
  case 0x5b:sprintf(t, "tcd                    ");break;
  case 0x5c:sprintf(t, "jml $%0.6x   [$%0.6x]", op0|op1<<8|op2<<16, resolve_offset(OPTYPE_LONG, (op0|op1<<8|op2<<16)));break;
  case 0x5d:sprintf(t, "eor $%0.4x,x   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDRX, (op0|op1<<8)));break;
  case 0x5e:sprintf(t, "lsr $%0.4x,x   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDRX, (op0|op1<<8)));break;
  case 0x5f:sprintf(t, "eor $%0.6x,x [$%0.6x]", op0|op1<<8|op2<<16, resolve_offset(OPTYPE_LONGX, (op0|op1<<8|op2<<16)));break;
  case 0x60:sprintf(t, "rts                    ");break;
  case 0x61:sprintf(t, "adc ($%0.2x,x)   [$%0.6x]", op0, resolve_offset(OPTYPE_IDPX, op0));break;
  case 0x62:sprintf(t, "per $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR, (op0|op1<<8)));break;
  case 0x63:sprintf(t, "adc $%0.2x,s     [$%0.6x]", op0, resolve_offset(OPTYPE_SR, op0));break;
  case 0x64:sprintf(t, "stz $%0.2x       [$%0.6x]", op0, resolve_offset(OPTYPE_DP, op0));break;
  case 0x65:sprintf(t, "adc $%0.2x       [$%0.6x]", op0, resolve_offset(OPTYPE_DP, op0));break;
  case 0x66:sprintf(t, "ror $%0.2x       [$%0.6x]", op0, resolve_offset(OPTYPE_DP, op0));break;
  case 0x67:sprintf(t, "adc [$%0.2x]     [$%0.6x]", op0, resolve_offset(OPTYPE_ILDP, op0));break;
  case 0x68:sprintf(t, "pla                    ");break;
  case 0x69:
    if(regs.p.m)sprintf(t, "adc #$%0.2x               ", op0);
    else        sprintf(t, "adc #$%0.4x             ", op0|op1<<8);break;
  case 0x6a:sprintf(t, "ror a                  ");break;
  case 0x6b:sprintf(t, "rtl                    ");break;
  case 0x6c:sprintf(t, "jmp ($%0.4x)   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_IADDR_PC, (op0|op1<<8)));break;
  case 0x6d:sprintf(t, "adc $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR, (op0|op1<<8)));break;
  case 0x6e:sprintf(t, "ror $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR, (op0|op1<<8)));break;
  case 0x6f:sprintf(t, "adc $%0.6x   [$%0.6x]", op0|op1<<8|op2<<16, resolve_offset(OPTYPE_LONG, (op0|op1<<8|op2<<16)));break;
  case 0x70:sprintf(t, "bvs $%0.4x     [$%0.6x]", __relb(op0)&0xffff, __relb(op0)&0xffffff);break;
  case 0x71:sprintf(t, "adc ($%0.2x),y   [$%0.6x]", op0, resolve_offset(OPTYPE_IDPY, op0));break;
  case 0x72:sprintf(t, "adc ($%0.2x)     [$%0.6x]", op0, resolve_offset(OPTYPE_IDP, op0));break;
  case 0x73:sprintf(t, "adc ($%0.2x,s),y [$%0.6x]", op0, resolve_offset(OPTYPE_ISRY, op0));break;
  case 0x74:sprintf(t, "stz $%0.2x,x     [$%0.6x]", op0, resolve_offset(OPTYPE_DPX, op0));break;
  case 0x75:sprintf(t, "adc $%0.2x,x     [$%0.6x]", op0, resolve_offset(OPTYPE_DPX, op0));break;
  case 0x76:sprintf(t, "ror $%0.2x,x     [$%0.6x]", op0, resolve_offset(OPTYPE_DPX, op0));break;
  case 0x77:sprintf(t, "adc [$%0.2x],y   [$%0.6x]", op0, resolve_offset(OPTYPE_ILDPY, op0));break;
  case 0x78:sprintf(t, "sei                    ");break;
  case 0x79:sprintf(t, "adc $%0.4x,y   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDRY, (op0|op1<<8)));break;
  case 0x7a:sprintf(t, "ply                    ");break;
  case 0x7b:sprintf(t, "tdc                    ");break;
  case 0x7c:sprintf(t, "jmp ($%0.4x,x) [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_IADDRX, (op0|op1<<8)));break;
  case 0x7d:sprintf(t, "adc $%0.4x,x   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDRX, (op0|op1<<8)));break;
  case 0x7e:sprintf(t, "ror $%0.4x,x   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDRX, (op0|op1<<8)));break;
  case 0x7f:sprintf(t, "adc $%0.6x,x [$%0.6x]", op0|op1<<8|op2<<16, resolve_offset(OPTYPE_LONGX, (op0|op1<<8|op2<<16)));break;
  case 0x80:sprintf(t, "bra $%0.4x     [$%0.6x]", __relb(op0)&0xffff, __relb(op0)&0xffffff);break;
  case 0x81:sprintf(t, "sta ($%0.2x,x)   [$%0.6x]", op0, resolve_offset(OPTYPE_IDPX, op0));break;
  case 0x82:sprintf(t, "brl $%0.4x     [$%0.6x]", __relw(op0|op1<<8)&0xffff, __relw(op0|op1<<8)&0xffffff);break;
  case 0x83:sprintf(t, "sta $%0.2x,s     [$%0.6x]", op0, resolve_offset(OPTYPE_SR, op0));break;
  case 0x84:sprintf(t, "sty $%0.2x       [$%0.6x]", op0, resolve_offset(OPTYPE_DP, op0));break;
  case 0x85:sprintf(t, "sta $%0.2x       [$%0.6x]", op0, resolve_offset(OPTYPE_DP, op0));break;
  case 0x86:sprintf(t, "stx $%0.2x       [$%0.6x]", op0, resolve_offset(OPTYPE_DP, op0));break;
  case 0x87:sprintf(t, "sta [$%0.2x]     [$%0.6x]", op0, resolve_offset(OPTYPE_ILDP, op0));break;
  case 0x88:sprintf(t, "dey                    ");break;
  case 0x89:
    if(regs.p.m)sprintf(t, "bit #$%0.2x               ", op0);
    else        sprintf(t, "bit #$%0.4x             ", op0|op1<<8);break;
  case 0x8a:sprintf(t, "txa                    ");break;
  case 0x8b:sprintf(t, "phb                    ");break;
  case 0x8c:sprintf(t, "sty $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR, (op0|op1<<8)));break;
  case 0x8d:sprintf(t, "sta $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR, (op0|op1<<8)));break;
  case 0x8e:sprintf(t, "stx $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR, (op0|op1<<8)));break;
  case 0x8f:sprintf(t, "sta $%0.6x   [$%0.6x]", op0|op1<<8|op2<<16, resolve_offset(OPTYPE_LONG, (op0|op1<<8|op2<<16)));break;
  case 0x90:sprintf(t, "bcc $%0.4x     [$%0.6x]", __relb(op0)&0xffff, __relb(op0)&0xffffff);break;
  case 0x91:sprintf(t, "sta ($%0.2x),y   [$%0.6x]", op0, resolve_offset(OPTYPE_IDPY, op0));break;
  case 0x92:sprintf(t, "sta ($%0.2x)     [$%0.6x]", op0, resolve_offset(OPTYPE_IDP, op0));break;
  case 0x93:sprintf(t, "sta ($%0.2x,s),y [$%0.6x]", op0, resolve_offset(OPTYPE_ISRY, op0));break;
  case 0x94:sprintf(t, "sty $%0.2x,x     [$%0.6x]", op0, resolve_offset(OPTYPE_DPX, op0));break;
  case 0x95:sprintf(t, "sta $%0.2x,x     [$%0.6x]", op0, resolve_offset(OPTYPE_DPX, op0));break;
  case 0x96:sprintf(t, "stx $%0.2x,y     [$%0.6x]", op0, resolve_offset(OPTYPE_DPY, op0));break;
  case 0x97:sprintf(t, "sta [$%0.2x],y   [$%0.6x]", op0, resolve_offset(OPTYPE_ILDPY, op0));break;
  case 0x98:sprintf(t, "tya                    ");break;
  case 0x99:sprintf(t, "sta $%0.4x,y   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDRY, (op0|op1<<8)));break;
  case 0x9a:sprintf(t, "txs                    ");break;
  case 0x9b:sprintf(t, "txy                    ");break;
  case 0x9c:sprintf(t, "stz $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR, (op0|op1<<8)));break;
  case 0x9d:sprintf(t, "sta $%0.4x,x   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDRX, (op0|op1<<8)));break;
  case 0x9e:sprintf(t, "stz $%0.4x,x   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDRX, (op0|op1<<8)));break;
  case 0x9f:sprintf(t, "sta $%0.6x,x [$%0.6x]", op0|op1<<8|op2<<16, resolve_offset(OPTYPE_LONGX, (op0|op1<<8|op2<<16)));break;
  case 0xa0:
    if(regs.p.x)sprintf(t, "ldy #$%0.2x               ", op0);
    else        sprintf(t, "ldy #$%0.4x             ", op0|op1<<8);break;
  case 0xa1:sprintf(t, "lda ($%0.2x,x)   [$%0.6x]", op0, resolve_offset(OPTYPE_IDPX, op0));break;
  case 0xa2:
    if(regs.p.x)sprintf(t, "ldx #$%0.2x               ", op0);
    else        sprintf(t, "ldx #$%0.4x             ", op0|op1<<8);break;
  case 0xa3:sprintf(t, "lda $%0.2x,s     [$%0.6x]", op0, resolve_offset(OPTYPE_SR, op0));break;
  case 0xa4:sprintf(t, "ldy $%0.2x       [$%0.6x]", op0, resolve_offset(OPTYPE_DP, op0));break;
  case 0xa5:sprintf(t, "lda $%0.2x       [$%0.6x]", op0, resolve_offset(OPTYPE_DP, op0));break;
  case 0xa6:sprintf(t, "ldx $%0.2x       [$%0.6x]", op0, resolve_offset(OPTYPE_DP, op0));break;
  case 0xa7:sprintf(t, "lda [$%0.2x]     [$%0.6x]", op0, resolve_offset(OPTYPE_ILDP, op0));break;
  case 0xa8:sprintf(t, "tay                    ");break;
  case 0xa9:
    if(regs.p.m)sprintf(t, "lda #$%0.2x               ", op0);
    else        sprintf(t, "lda #$%0.4x             ", op0|op1<<8);break;
  case 0xaa:sprintf(t, "tax                    ");break;
  case 0xab:sprintf(t, "plb                    ");break;
  case 0xac:sprintf(t, "ldy $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR, (op0|op1<<8)));break;
  case 0xad:sprintf(t, "lda $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR, (op0|op1<<8)));break;
  case 0xae:sprintf(t, "ldx $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR, (op0|op1<<8)));break;
  case 0xaf:sprintf(t, "lda $%0.6x   [$%0.6x]", op0|op1<<8|op2<<16, resolve_offset(OPTYPE_LONG, (op0|op1<<8|op2<<16)));break;
  case 0xb0:sprintf(t, "bcs $%0.4x     [$%0.6x]", __relb(op0)&0xffff, __relb(op0)&0xffffff);break;
  case 0xb1:sprintf(t, "lda ($%0.2x),y   [$%0.6x]", op0, resolve_offset(OPTYPE_IDPY, op0));break;
  case 0xb2:sprintf(t, "lda ($%0.2x)     [$%0.6x]", op0, resolve_offset(OPTYPE_IDP, op0));break;
  case 0xb3:sprintf(t, "lda ($%0.2x,s),y [$%0.6x]", op0, resolve_offset(OPTYPE_ISRY, op0));break;
  case 0xb4:sprintf(t, "ldy $%0.2x,x     [$%0.6x]", op0, resolve_offset(OPTYPE_DPX, op0));break;
  case 0xb5:sprintf(t, "lda $%0.2x,x     [$%0.6x]", op0, resolve_offset(OPTYPE_DPX, op0));break;
  case 0xb6:sprintf(t, "ldx $%0.2x,y     [$%0.6x]", op0, resolve_offset(OPTYPE_DPY, op0));break;
  case 0xb7:sprintf(t, "lda [$%0.2x],y   [$%0.6x]", op0, resolve_offset(OPTYPE_ILDPY, op0));break;
  case 0xb8:sprintf(t, "clv                    ");break;
  case 0xb9:sprintf(t, "lda $%0.4x,y   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDRY, (op0|op1<<8)));break;
  case 0xba:sprintf(t, "tsx                    ");break;
  case 0xbb:sprintf(t, "tyx                    ");break;
  case 0xbc:sprintf(t, "ldy $%0.4x,x   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDRX, (op0|op1<<8)));break;
  case 0xbd:sprintf(t, "lda $%0.4x,x   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDRX, (op0|op1<<8)));break;
  case 0xbe:sprintf(t, "ldx $%0.4x,y   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDRY, (op0|op1<<8)));break;
  case 0xbf:sprintf(t, "lda $%0.6x,x [$%0.6x]", op0|op1<<8|op2<<16, resolve_offset(OPTYPE_LONGX, (op0|op1<<8|op2<<16)));break;
  case 0xc0:
    if(regs.p.x)sprintf(t, "cpy #$%0.2x               ", op0);
    else        sprintf(t, "cpy #$%0.4x             ", op0|op1<<8);break;
  case 0xc1:sprintf(t, "cmp ($%0.2x,x)   [$%0.6x]", op0, resolve_offset(OPTYPE_IDPX, op0));break;
  case 0xc2:sprintf(t, "rep #$%0.2x               ", op0);break;
  case 0xc3:sprintf(t, "cmp $%0.2x,s     [$%0.6x]", op0, resolve_offset(OPTYPE_SR, op0));break;
  case 0xc4:sprintf(t, "cpy $%0.2x       [$%0.6x]", op0, resolve_offset(OPTYPE_DP, op0));break;
  case 0xc5:sprintf(t, "cmp $%0.2x       [$%0.6x]", op0, resolve_offset(OPTYPE_DP, op0));break;
  case 0xc6:sprintf(t, "dec $%0.2x       [$%0.6x]", op0, resolve_offset(OPTYPE_DP, op0));break;
  case 0xc7:sprintf(t, "cmp [$%0.2x]     [$%0.6x]", op0, resolve_offset(OPTYPE_ILDP, op0));break;
  case 0xc8:sprintf(t, "iny                    ");break;
  case 0xc9:
    if(regs.p.m)sprintf(t, "cmp #$%0.2x               ", op0);
    else        sprintf(t, "cmp #$%0.4x             ", op0|op1<<8);break;
  case 0xca:sprintf(t, "dex                    ");break;
  case 0xcb:sprintf(t, "wai                    ");break;
  case 0xcc:sprintf(t, "cpy $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR, (op0|op1<<8)));break;
  case 0xcd:sprintf(t, "cmp $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR, (op0|op1<<8)));break;
  case 0xce:sprintf(t, "dec $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR, (op0|op1<<8)));break;
  case 0xcf:sprintf(t, "cmp $%0.6x   [$%0.6x]", op0|op1<<8|op2<<16, resolve_offset(OPTYPE_LONG, (op0|op1<<8|op2<<16)));break;
  case 0xd0:sprintf(t, "bne $%0.4x     [$%0.6x]", __relb(op0)&0xffff, __relb(op0)&0xffffff);break;
  case 0xd1:sprintf(t, "cmp ($%0.2x),y   [$%0.6x]", op0, resolve_offset(OPTYPE_IDPY, op0));break;
  case 0xd2:sprintf(t, "cmp ($%0.2x)     [$%0.6x]", op0, resolve_offset(OPTYPE_IDP, op0));break;
  case 0xd3:sprintf(t, "cmp ($%0.2x,s),y [$%0.6x]", op0, resolve_offset(OPTYPE_ISRY, op0));break;
  case 0xd4:sprintf(t, "pei ($%0.2x)     [$%0.6x]", op0, resolve_offset(OPTYPE_IDP, op0));break;
  case 0xd5:sprintf(t, "cmp $%0.2x,x     [$%0.6x]", op0, resolve_offset(OPTYPE_DPX, op0));break;
  case 0xd6:sprintf(t, "dec $%0.2x,x     [$%0.6x]", op0, resolve_offset(OPTYPE_DPX, op0));break;
  case 0xd7:sprintf(t, "cmp [$%0.2x],y   [$%0.6x]", op0, resolve_offset(OPTYPE_ILDPY, op0));break;
  case 0xd8:sprintf(t, "cld                    ");break;
  case 0xd9:sprintf(t, "cmp $%0.4x,y   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDRY, (op0|op1<<8)));break;
  case 0xda:sprintf(t, "phx                    ");break;
  case 0xdb:sprintf(t, "stp                    ");break;
  case 0xdc:sprintf(t, "jmp [$%0.4x]   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ILADDR, (op0|op1<<8)));break;
  case 0xdd:sprintf(t, "cmp $%0.4x,x   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDRX, (op0|op1<<8)));break;
  case 0xde:sprintf(t, "dec $%0.4x,x   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDRX, (op0|op1<<8)));break;
  case 0xdf:sprintf(t, "cmp $%0.6x,x [$%0.6x]", op0|op1<<8|op2<<16, resolve_offset(OPTYPE_LONGX, (op0|op1<<8|op2<<16)));break;
  case 0xe0:
    if(regs.p.x)sprintf(t, "cpx #$%0.2x               ", op0);
    else        sprintf(t, "cpx #$%0.4x             ", op0|op1<<8);break;
  case 0xe1:sprintf(t, "sbc ($%0.2x,x)   [$%0.6x]", op0, resolve_offset(OPTYPE_IDPX, op0));break;
  case 0xe2:sprintf(t, "sep #$%0.2x               ", op0);break;
  case 0xe3:sprintf(t, "sbc $%0.2x,s     [$%0.6x]", op0, resolve_offset(OPTYPE_SR, op0));break;
  case 0xe4:sprintf(t, "cpx $%0.2x       [$%0.6x]", op0, resolve_offset(OPTYPE_DP, op0));break;
  case 0xe5:sprintf(t, "sbc $%0.2x       [$%0.6x]", op0, resolve_offset(OPTYPE_DP, op0));break;
  case 0xe6:sprintf(t, "inc $%0.2x       [$%0.6x]", op0, resolve_offset(OPTYPE_DP, op0));break;
  case 0xe7:sprintf(t, "sbc [$%0.2x]     [$%0.6x]", op0, resolve_offset(OPTYPE_ILDP, op0));break;
  case 0xe8:sprintf(t, "inx                    ");break;
  case 0xe9:
    if(regs.p.m)sprintf(t, "sbc #$%0.2x               ", op0);
    else        sprintf(t, "sbc #$%0.4x             ", op0|op1<<8);break;
  case 0xea:sprintf(t, "nop                    ");break;
  case 0xeb:sprintf(t, "xba                    ");break;
  case 0xec:sprintf(t, "cpx $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR, (op0|op1<<8)));break;
  case 0xed:sprintf(t, "sbc $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR, (op0|op1<<8)));break;
  case 0xee:sprintf(t, "inc $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR, (op0|op1<<8)));break;
  case 0xef:sprintf(t, "sbc $%0.6x   [$%0.6x]", op0|op1<<8|op2<<16, resolve_offset(OPTYPE_LONG, (op0|op1<<8|op2<<16)));break;
  case 0xf0:sprintf(t, "beq $%0.4x     [$%0.6x]", __relb(op0)&0xffff, __relb(op0)&0xffffff);break;
  case 0xf1:sprintf(t, "sbc ($%0.2x),y   [$%0.6x]", op0, resolve_offset(OPTYPE_IDPY, op0));break;
  case 0xf2:sprintf(t, "sbc ($%0.2x)     [$%0.6x]", op0, resolve_offset(OPTYPE_IDP, op0));break;
  case 0xf3:sprintf(t, "sbc ($%0.2x,s),y [$%0.6x]", op0, resolve_offset(OPTYPE_ISRY, op0));break;
  case 0xf4:sprintf(t, "pea $%0.4x     [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDR, (op0|op1<<8)));break;
  case 0xf5:sprintf(t, "sbc $%0.2x,x     [$%0.6x]", op0, resolve_offset(OPTYPE_DPX, op0));break;
  case 0xf6:sprintf(t, "inc $%0.2x,x     [$%0.6x]", op0, resolve_offset(OPTYPE_DPX, op0));break;
  case 0xf7:sprintf(t, "sbc [$%0.2x],y   [$%0.6x]", op0, resolve_offset(OPTYPE_ILDPY, op0));break;
  case 0xf8:sprintf(t, "sed                    ");break;
  case 0xf9:sprintf(t, "sbc $%0.4x,y   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDRY, (op0|op1<<8)));break;
  case 0xfa:sprintf(t, "plx                    ");break;
  case 0xfb:sprintf(t, "xce                    ");break;
  case 0xfc:sprintf(t, "jsr ($%0.4x,x) [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_IADDRX, (op0|op1<<8)));break;
  case 0xfd:sprintf(t, "sbc $%0.4x,x   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDRX, (op0|op1<<8)));break;
  case 0xfe:sprintf(t, "inc $%0.4x,x   [$%0.6x]", op0|op1<<8, resolve_offset(OPTYPE_ADDRX, (op0|op1<<8)));break;
  case 0xff:sprintf(t, "sbc $%0.6x,x [$%0.6x]", op0|op1<<8|op2<<16, resolve_offset(OPTYPE_LONGX, (op0|op1<<8|op2<<16)));break;
  }
  strcat(s, t);
  strcat(s, " ");

  sprintf(t, "A:%0.4x X:%0.4x Y:%0.4x S:%0.4x D:%0.4x DB:%0.2x ",
    regs.a.w, regs.x.w, regs.y.w, regs.s.w, regs.d.w, regs.db);
  strcat(s, t);
  if(regs.e) {
    sprintf(t, "%c%c%c%c%c%c%c%c",
      (regs.p.n)?'N':'n', (regs.p.v)?'V':'v',
      (regs.p.m)?'1':'0', (regs.p.x)?'B':'b',
      (regs.p.d)?'D':'d', (regs.p.i)?'I':'i',
      (regs.p.z)?'Z':'z', (regs.p.c)?'C':'c');
  } else {
    sprintf(t, "%c%c%c%c%c%c%c%c",
      (regs.p.n)?'N':'n', (regs.p.v)?'V':'v',
      (regs.p.m)?'M':'m', (regs.p.x)?'X':'x',
      (regs.p.d)?'D':'d', (regs.p.i)?'I':'i',
      (regs.p.z)?'Z':'z', (regs.p.c)?'C':'c');
  }
  strcat(s, t);
}

/*
  opcode_length() retrieves the length of the next opcode
  to be executed. It is used by the debugger to step over,
  disable, and proceed cpu opcodes.

  5 and 6 are special cases, 5 is used for #consts based on
  the A register size, 6 for the X/Y register size. the
  rest are literal sizes. There's no need to test for
  emulation mode, as regs.p.m/regs.p.x should *always* be
  set in emulation mode.
*/
uint8 CPU::opcode_length() {
uint8 op, len;
static uint8 op_len_tbl[256] = {
//0,1,2,3,  4,5,6,7,  8,9,a,b,  c,d,e,f
  2,2,2,2,  2,2,2,2,  1,5,1,1,  3,3,3,4, //0x0n
  2,2,2,2,  2,2,2,2,  1,3,1,1,  3,3,3,4, //0x1n
  3,2,4,2,  2,2,2,2,  1,5,1,1,  3,3,3,4, //0x2n
  2,2,2,2,  2,2,2,2,  1,3,1,1,  3,3,3,4, //0x3n

  1,2,2,2,  3,2,2,2,  1,5,1,1,  3,3,3,4, //0x4n
  2,2,2,2,  3,2,2,2,  1,3,1,1,  4,3,3,4, //0x5n
  1,2,3,2,  2,2,2,2,  1,5,1,1,  3,3,3,4, //0x6n
  2,2,2,2,  2,2,2,2,  1,3,1,1,  3,3,3,4, //0x7n

  2,2,3,2,  2,2,2,2,  1,5,1,1,  3,3,3,4, //0x8n
  2,2,2,2,  2,2,2,2,  1,3,1,1,  3,3,3,4, //0x9n
  6,2,6,2,  2,2,2,2,  1,5,1,1,  3,3,3,4, //0xan
  2,2,2,2,  2,2,2,2,  1,3,1,1,  3,3,3,4, //0xbn

  6,2,2,2,  2,2,2,2,  1,5,1,1,  3,3,3,4, //0xcn
  2,2,2,2,  2,2,2,2,  1,3,1,1,  3,3,3,4, //0xdn
  6,2,2,2,  2,2,2,2,  1,5,1,1,  3,3,3,4, //0xen
  2,2,2,2,  3,2,2,2,  1,3,1,1,  3,3,3,4  //0xfn
};
  if(in_opcode() == true) {
    return 0;
  }

  op = r_mem->read(regs.pc.d);
  len = op_len_tbl[op];
  if(len == 5)return (regs.p.m)?2:3;
  if(len == 6)return (regs.p.x)?2:3;
  return len;
}
