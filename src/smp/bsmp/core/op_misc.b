nop(0x00) {
1:
}

sleep(0xef),
stop(0xff) {
1:
2:regs.pc--;
}

xcn(0x9f) {
1:
2:
3:
4:regs.a = (regs.a >> 4) | (regs.a << 4);
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

daa(0xdf) {
1:
2:if(regs.p.c || (regs.a) > 0x99) {
    regs.a += 0x60;
    regs.p.c = 1;
  }
  if(regs.p.h || (regs.a & 15) > 0x09) {
    regs.a += 0x06;
  }
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

das(0xbe) {
1:
2:if(!regs.p.c || (regs.a) > 0x99) {
    regs.a -= 0x60;
    regs.p.c = 0;
  }
  if(!regs.p.h || (regs.a & 15) > 0x09) {
    regs.a -= 0x06;
  }
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}

clrc(0x60, regs.p.c = 0),
clrp(0x20, regs.p.p = 0),
setc(0x80, regs.p.c = 1),
setp(0x40, regs.p.p = 1) {
1:$1;
}

clrv(0xe0) {
1:regs.p.v = 0;
  regs.p.h = 0;
}

notc(0xed) {
1:
2:regs.p.c ^= 1;
}

ei(0xa0, 1),
di(0xc0, 0) {
1:
2:regs.p.i = $1;
}

set0_dp(0x02, rd |=  0x01),
clr0_dp(0x12, rd &= ~0x01),
set1_dp(0x22, rd |=  0x02),
clr1_dp(0x32, rd &= ~0x02),
set2_dp(0x42, rd |=  0x04),
clr2_dp(0x52, rd &= ~0x04),
set3_dp(0x62, rd |=  0x08),
clr3_dp(0x72, rd &= ~0x08),
set4_dp(0x82, rd |=  0x10),
clr4_dp(0x92, rd &= ~0x10),
set5_dp(0xa2, rd |=  0x20),
clr5_dp(0xb2, rd &= ~0x20),
set6_dp(0xc2, rd |=  0x40),
clr6_dp(0xd2, rd &= ~0x40),
set7_dp(0xe2, rd |=  0x80),
clr7_dp(0xf2, rd &= ~0x80) {
1:dp = op_read();
2:rd = op_read(OPMODE_DP, dp);
3:$1;
  op_write(OPMODE_DP, dp, rd);
}

tset_addr_a(0x0e, |=),
tclr_addr_a(0x4e, &=~) {
1:dp  = op_read();
2:dp |= op_read() << 8;
3:rd = op_read(OPMODE_ADDR, dp);
4:regs.p.n = !!((rd & regs.a) & 0x80);
  regs.p.z = ((rd & regs.a) == 0);
  rd $1 regs.a;
5:op_write(OPMODE_ADDR, dp, rd);
}

push_a(0x2d, a),
push_x(0x4d, x),
push_y(0x6d, y),
push_p(0x0d, p) {
1:
2:
3:stack_write(regs.$1);
}

pop_a(0xae, a),
pop_x(0xce, x),
pop_y(0xee, y),
pop_p(0x8e, p) {
1:
2:
3:regs.$1 = stack_read();
}

mul_ya(0xcf) {
1:
2:
3:
4:
5:
6:
7:
8:ya = regs.y * regs.a;
  regs.a = ya;
  regs.y = ya >> 8;
//result is set based on y (high-byte) only
  regs.p.n = !!(regs.y & 0x80);
  regs.p.z = (regs.y == 0);
}

div_ya_x(0x9e) {
1:
2:
3:
4:
5:
6:
7:
8:
9:
10:
11:ya = regs.ya;
//overflow set if quotient >= 256
  regs.p.v = !!(regs.y >= regs.x);
  regs.p.h = !!((regs.y & 15) >= (regs.x & 15));
  if(regs.y < (regs.x << 1)) {
  //if quotient is <= 511 (will fit into 9-bit result)
    regs.a = ya / regs.x;
    regs.y = ya % regs.x;
  } else {
  //otherwise, the quotient won't fit into regs.p.v + regs.a
  //this emulates the odd behavior of the SPC700 in this case
    regs.a = 255    - (ya - (regs.x << 9)) / (256 - regs.x);
    regs.y = regs.x + (ya - (regs.x << 9)) % (256 - regs.x);
  }
//result is set based on a (quotient) only
  regs.p.n = !!(regs.a & 0x80);
  regs.p.z = (regs.a == 0);
}
