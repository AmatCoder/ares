inc(0x1a, regs.p.m, a),
inx(0xe8, regs.p.x, x),
iny(0xc8, regs.p.x, y) {
1:cpu_io();
  if($1) {
    regs.$2.l++;
    regs.p.n = !!(regs.$2.l & 0x80);
    regs.p.z = (regs.$2.l == 0);
  } else {
    regs.$2.w++;
    regs.p.n = !!(regs.$2.w & 0x8000);
    regs.p.z = (regs.$2.w == 0);
  }
}

dec(0x3a, regs.p.m, a),
dex(0xca, regs.p.x, x),
dey(0x88, regs.p.x, y) {
1:cpu_io();
  if($1) {
    regs.$2.l--;
    regs.p.n = !!(regs.$2.l & 0x80);
    regs.p.z = (regs.$2.l == 0);
  } else {
    regs.$2.w--;
    regs.p.n = !!(regs.$2.w & 0x8000);
    regs.p.z = (regs.$2.w == 0);
  }
}

asl(0x0a) {
1:cpu_io();
  if(regs.p.m) {
    regs.p.c = !!(regs.a.l & 0x80);
    regs.a.l <<= 1;
    regs.p.n = !!(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  } else {
    regs.p.c = !!(regs.a.w & 0x8000);
    regs.a.w <<= 1;
    regs.p.n = !!(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }
}

lsr(0x4a) {
1:cpu_io();
  if(regs.p.m) {
    regs.p.c = regs.a.l & 1;
    regs.a.l >>= 1;
    regs.p.n = !!(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  } else {
    regs.p.c = regs.a.w & 1;
    regs.a.w >>= 1;
    regs.p.n = !!(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }
}

rol(0x2a) {
1:cpu_io();
  uint16 c = regs.p.c;
  if(regs.p.m) {
    regs.p.c = !!(regs.a.l & 0x80);
    regs.a.l <<= 1;
    regs.a.l |= c;
    regs.p.n = !!(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  } else {
    regs.p.c = !!(regs.a.w & 0x8000);
    regs.a.w <<= 1;
    regs.a.w |= c;
    regs.p.n = !!(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }
}

ror(0x6a) {
1:cpu_io();
  uint16 c;
  if(regs.p.m) {
    c = (regs.p.c)?0x80:0;
    regs.p.c = regs.a.l & 1;
    regs.a.l >>= 1;
    regs.a.l |= c;
    regs.p.n = !!(regs.a.l & 0x80);
    regs.p.z = (regs.a.l == 0);
  } else {
    c = (regs.p.c)?0x8000:0;
    regs.p.c = regs.a.w & 1;
    regs.a.w >>= 1;
    regs.a.w |= c;
    regs.p.n = !!(regs.a.w & 0x8000);
    regs.p.z = (regs.a.w == 0);
  }
}

inc_addr(0xee, inc),
dec_addr(0xce, dec),
asl_addr(0x0e, asl),
lsr_addr(0x4e, lsr),
rol_addr(0x2e, rol),
ror_addr(0x6e, ror),
trb_addr(0x1c, trb),
tsb_addr(0x0c, tsb) {
1:aa.l = op_read();
2:aa.h = op_read();
3:rd.l = op_read(OPMODE_DBR, aa.w);
  if(regs.p.m)skip;
4:rd.h = op_read(OPMODE_DBR, aa.w + 1);
5:cpu_io();
  if(regs.p.m) { op_$1_b(); skip; }
  else op_$1_w();
6:op_write(OPMODE_DBR, aa.w + 1, rd.h);
7:op_write(OPMODE_DBR, aa.w,     rd.l);
}

inc_addrx(0xfe, inc),
dec_addrx(0xde, dec),
asl_addrx(0x1e, asl),
lsr_addrx(0x5e, lsr),
rol_addrx(0x3e, rol),
ror_addrx(0x7e, ror) {
1:aa.l = op_read();
2:aa.h = op_read();
3:cpu_io();
4:rd.l = op_read(OPMODE_DBR, aa.w + regs.x.w);
  if(regs.p.m)skip;
5:rd.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1);
6:cpu_io();
  if(regs.p.m) { op_$1_b(); skip; }
  else op_$1_w();
7:op_write(OPMODE_DBR, aa.w + regs.x.w + 1, rd.h);
8:op_write(OPMODE_DBR, aa.w + regs.x.w,     rd.l);
}

inc_dp(0xe6, inc),
dec_dp(0xc6, dec),
asl_dp(0x06, asl),
lsr_dp(0x46, lsr),
rol_dp(0x26, rol),
ror_dp(0x66, ror),
trb_dp(0x14, trb),
tsb_dp(0x04, tsb) {
1:dp = op_read();
2:cpu_c2();
3:rd.l = op_read(OPMODE_DP, dp);
  if(regs.p.m)skip;
4:rd.h = op_read(OPMODE_DP, dp + 1);
5:cpu_io();
  if(regs.p.m) { op_$1_b(); skip; }
  else op_$1_w();
6:op_write(OPMODE_DP, dp + 1, rd.h);
7:op_write(OPMODE_DP, dp,     rd.l);
}

inc_dpx(0xf6, inc),
dec_dpx(0xd6, dec),
asl_dpx(0x16, asl),
lsr_dpx(0x56, lsr),
rol_dpx(0x36, rol),
ror_dpx(0x76, ror) {
1:dp = op_read();
2:cpu_c2();
3:cpu_io();
4:rd.l = op_read(OPMODE_DP, dp + regs.x.w);
  if(regs.p.m)skip;
5:rd.h = op_read(OPMODE_DP, dp + regs.x.w + 1);
6:cpu_io();
  if(regs.p.m) { op_$1_b(); skip; }
  else op_$1_w();
7:op_write(OPMODE_DP, dp + regs.x.w + 1, rd.h);
8:op_write(OPMODE_DP, dp + regs.x.w,     rd.l);
}
