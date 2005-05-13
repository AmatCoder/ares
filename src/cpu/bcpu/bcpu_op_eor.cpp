void bCPU::flags_eor_b() {
  regs.p.n = !!(regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
}

void bCPU::flags_eor_w() {
  regs.p.n = !!(regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
}

/************************
 *** 0x49: eor #const ***
 ************************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; idl
  [2a] pbr,pc+2 ; idh [1]
*/
void bCPU::op_eor_constb() {
  regs.a.l ^= op_read(); //2
  flags_eor_b();
}

void bCPU::op_eor_constw() {
  regs.a.l ^= op_read(); //2
  regs.a.h ^= op_read(); //2a
  flags_eor_w();
}

void bCPU::op_eor_const() { (regs.p.m)?op_eor_constb():op_eor_constw(); }

/**********************
 *** 0x4d: eor addr ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; aal
  [3 ] pbr,pc+2 ; aah
  [4 ] dbr,aa   ; data low
  [4a] dbr,aa+1 ; data high [1]
*/
void bCPU::op_eor_addrb() {
  aa.l = op_read();                      //2
  aa.h = op_read();                      //3
  regs.a.l ^= op_read(OPMODE_DBR, aa.w); //4
  flags_eor_b();
}

void bCPU::op_eor_addrw() {
  aa.l = op_read();                          //2
  aa.h = op_read();                          //3
  regs.a.l ^= op_read(OPMODE_DBR, aa.w);     //4
  regs.a.h ^= op_read(OPMODE_DBR, aa.w + 1); //4a
  flags_eor_w();
}

void bCPU::op_eor_addr() { (regs.p.m)?op_eor_addrb():op_eor_addrw(); }

/************************
 *** 0x5d: eor addr,x ***
 ************************
cycles:
  [1 ] pbr,pc         ; opereor
  [2 ] pbr,pc+1       ; aal
  [3 ] pbr,pc+2       ; aah
  [3a] dbr,aah,aal+xl ; io [4]
  [4 ] dbr,aa+x       ; data low
  [4a] dbr,aa+x+1     ; data high [1]
*/
void bCPU::op_eor_addrxb() {
  aa.l = op_read();                                 //2
  aa.h = op_read();                                 //3
  cpu_c4(aa.w, aa.w + regs.x.w);                    //3a
  regs.a.l ^= op_read(OPMODE_DBR, aa.w + regs.x.w); //4
  flags_eor_b();
}

void bCPU::op_eor_addrxw() {
  aa.l = op_read();                                     //2
  aa.h = op_read();                                     //3
  cpu_c4(aa.w, aa.w + regs.x.w);                        //3a
  regs.a.l ^= op_read(OPMODE_DBR, aa.w + regs.x.w);     //4
  regs.a.h ^= op_read(OPMODE_DBR, aa.w + regs.x.w + 1); //4a
  flags_eor_w();
}

void bCPU::op_eor_addrx() { (regs.p.m)?op_eor_addrxb():op_eor_addrxw(); }

/********************
 *** 0x45: eor dp ***
 ********************
cycles:
  [1 ] pbr,pc   ; opereor
  [2 ] pbr,pc+1 ; dp
  [2a] pbr,pc+1 ; io [2]
  [3 ] 0,d+dp   ; data low
  [3a] 0,d+dp+1 ; data high [1]
*/
void bCPU::op_eor_dpb() {
  dp = op_read();                     //2
  cpu_c2();                           //2a
  regs.a.l ^= op_read(OPMODE_DP, dp); //3
  flags_eor_b();
}

void bCPU::op_eor_dpw() {
  dp = op_read();                         //2
  cpu_c2();                               //2a
  regs.a.l ^= op_read(OPMODE_DP, dp);     //3
  regs.a.h ^= op_read(OPMODE_DP, dp + 1); //3a
  flags_eor_w();
}

void bCPU::op_eor_dp() { (regs.p.m)?op_eor_dpb():op_eor_dpw(); }

/**********************
 *** 0x52: eor (dp) ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; dp
  [2a] pbr,pc+1 ; io [2]
  [3 ] 0,d+dp   ; aal
  [4 ] 0,d+dp+1 ; aah
  [5 ] dbr,aa   ; data low
  [5a] dbr,aa+1 ; data high [1]
*/
void bCPU::op_eor_idpb() {
  dp = op_read();                        //2
  cpu_c2();                              //2a
  aa.l = op_read(OPMODE_DP, dp);         //3
  aa.h = op_read(OPMODE_DP, dp + 1);     //4
  regs.a.l ^= op_read(OPMODE_DBR, aa.w); //5
  flags_eor_b();
}

void bCPU::op_eor_idpw() {
  dp = op_read();                            //2
  cpu_c2();                                  //2a
  aa.l = op_read(OPMODE_DP, dp);             //3
  aa.h = op_read(OPMODE_DP, dp + 1);         //4
  regs.a.l ^= op_read(OPMODE_DBR, aa.w);     //5
  regs.a.h ^= op_read(OPMODE_DBR, aa.w + 1); //5
  flags_eor_w();
}

void bCPU::op_eor_idp() { (regs.p.m)?op_eor_idpb():op_eor_idpw(); }

/**********************
 *** 0x47: eor [dp] ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; dp
  [2a] pbr,pc+1 ; io [2]
  [3 ] 0,d+dp   ; aal
  [4 ] 0,d+dp+1 ; aah
  [5 ] 0,d+dp+2 ; aab
  [6 ] aab,aa   ; data low
  [6a] aab,aa+1 ; data high [1]
*/
void bCPU::op_eor_ildpb() {
  dp = op_read();                         //2
  cpu_c2();                               //2a
  aa.l = op_read(OPMODE_DP, dp);          //3
  aa.h = op_read(OPMODE_DP, dp + 1);      //4
  aa.b = op_read(OPMODE_DP, dp + 2);      //5
  regs.a.l ^= op_read(OPMODE_LONG, aa.d); //6
  flags_eor_b();
}

void bCPU::op_eor_ildpw() {
  dp = op_read();                             //2
  cpu_c2();                                   //2a
  aa.l = op_read(OPMODE_DP, dp);              //3
  aa.h = op_read(OPMODE_DP, dp + 1);          //4
  aa.b = op_read(OPMODE_DP, dp + 2);          //5
  regs.a.l ^= op_read(OPMODE_LONG, aa.d);     //6
  regs.a.h ^= op_read(OPMODE_LONG, aa.d + 1); //6a
  flags_eor_w();
}

void bCPU::op_eor_ildp() { (regs.p.m)?op_eor_ildpb():op_eor_ildpw(); }

/**********************
 *** 0x4f: eor long ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; aal
  [3 ] pbr,pc+2 ; aah
  [4 ] pbr,pc+3 ; aab
  [5 ] aab,aa   ; data low
  [5a] aab,aa+1 ; data high
*/
void bCPU::op_eor_longb() {
  aa.l = op_read();                       //2
  aa.h = op_read();                       //3
  aa.b = op_read();                       //4
  regs.a.l ^= op_read(OPMODE_LONG, aa.d); //5
  flags_eor_b();
}

void bCPU::op_eor_longw() {
  aa.l = op_read();                           //2
  aa.h = op_read();                           //3
  aa.b = op_read();                           //4
  regs.a.l ^= op_read(OPMODE_LONG, aa.d);     //5
  regs.a.h ^= op_read(OPMODE_LONG, aa.d + 1); //5a
  flags_eor_w();
}

void bCPU::op_eor_long() { (regs.p.m)?op_eor_longb():op_eor_longw(); }

/************************
 *** 0x5f: eor long,x ***
 ************************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; aal
  [3 ] pbr,pc+2   ; aah
  [4 ] pbr,pc+3   ; aab
  [5 ] aab,aa+x   ; data low
  [5a] aab,aa+x+1 ; data high
*/
void bCPU::op_eor_longxb() {
  aa.l = op_read();                                  //2
  aa.h = op_read();                                  //3
  aa.b = op_read();                                  //4
  regs.a.l ^= op_read(OPMODE_LONG, aa.d + regs.x.w); //5
  flags_eor_b();
}

void bCPU::op_eor_longxw() {
  aa.l = op_read();                                      //2
  aa.h = op_read();                                      //3
  aa.b = op_read();                                      //4
  regs.a.l ^= op_read(OPMODE_LONG, aa.d + regs.x.w);     //5
  regs.a.h ^= op_read(OPMODE_LONG, aa.d + regs.x.w + 1); //5a
  flags_eor_w();
}

void bCPU::op_eor_longx() { (regs.p.m)?op_eor_longxb():op_eor_longxw(); }

/************************
 *** 0x59: eor addr,y ***
 ************************
cycles:
  [1 ] pbr,pc         ; opereor
  [2 ] pbr,pc+1       ; aal
  [3 ] pbr,pc+2       ; aah
  [3a] dbr,aah,aal+yl ; io [4]
  [4 ] dbr,aa+y       ; data low
  [4a] dbr,aa+y+1     ; data high [1]
*/
void bCPU::op_eor_addryb() {
  aa.l = op_read();                                 //2
  aa.h = op_read();                                 //3
  cpu_c4(aa.w, aa.w + regs.y.w);                    //3a
  regs.a.l ^= op_read(OPMODE_DBR, aa.w + regs.y.w); //4
  flags_eor_b();
}

void bCPU::op_eor_addryw() {
  aa.l = op_read();                                     //2
  aa.h = op_read();                                     //3
  cpu_c4(aa.w, aa.w + regs.y.w);                        //3a
  regs.a.l ^= op_read(OPMODE_DBR, aa.w + regs.y.w);     //4
  regs.a.h ^= op_read(OPMODE_DBR, aa.w + regs.y.w + 1); //4a
  flags_eor_w();
}

void bCPU::op_eor_addry() { (regs.p.m)?op_eor_addryb():op_eor_addryw(); }

/**********************
 *** 0x55: eor dp,x ***
 **********************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; dp
  [2a] pbr,pc+1   ; io
  [3 ] pbr,pc+1   ; io
  [4 ] 0,d+dp+x   ; data low
  [4a] 0,d+dp+x+1 ; data high
*/
void bCPU::op_eor_dpxb() {
  dp = op_read();                                //2
  cpu_c2();                                      //2a
  cpu_io();                                      //3
  regs.a.l ^= op_read(OPMODE_DP, dp + regs.x.w); //4
  flags_eor_b();
}

void bCPU::op_eor_dpxw() {
  dp = op_read();                                    //2
  cpu_c2();                                          //2a
  cpu_io();                                          //3
  regs.a.l ^= op_read(OPMODE_DP, dp + regs.x.w);     //4
  regs.a.h ^= op_read(OPMODE_DP, dp + regs.x.w + 1); //4a
  flags_eor_w();
}

void bCPU::op_eor_dpx() { (regs.p.m)?op_eor_dpxb():op_eor_dpxw(); }

/************************
 *** 0x41: eor (dp,x) ***
 ************************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; dp
  [2a] pbr,pc+1   ; io [2]
  [3 ] pbr,pc+1   ; io
  [4 ] 0,d+dp+x   ; aal
  [5 ] 0,d+dp+x+1 ; aah
  [6 ] dbr,aa     ; data low
  [6a] dbr,aa+1   ; data high [1]
*/
void bCPU::op_eor_idpxb() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  aa.l = op_read(OPMODE_DP, dp + regs.x.w);     //4
  aa.h = op_read(OPMODE_DP, dp + regs.x.w + 1); //5
  regs.a.l ^= op_read(OPMODE_DBR, aa.w);        //6
  flags_eor_b();
}

void bCPU::op_eor_idpxw() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  aa.l = op_read(OPMODE_DP, dp + regs.x.w);     //4
  aa.h = op_read(OPMODE_DP, dp + regs.x.w + 1); //5
  regs.a.l ^= op_read(OPMODE_DBR, aa.w);        //6
  regs.a.h ^= op_read(OPMODE_DBR, aa.w + 1);    //6a
  flags_eor_w();
}

void bCPU::op_eor_idpx() { (regs.p.m)?op_eor_idpxb():op_eor_idpxw(); }

/************************
 *** 0x51: eor (dp),y ***
 ************************
cycles:
  [1 ] pbr,pc         ; opcode
  [2 ] pbr,pc+1       ; dp
  [2a] pbr,pc+1       ; io [2]
  [3 ] 0,d+dp         ; aal
  [4 ] 0,d+dp+1       ; aah
  [4a] dbr,aah,aal+yl ; io [4]
  [5 ] dbr,aa+y       ; data low
  [5a] dbr,aa+y+1     ; data high [1]
*/
void bCPU::op_eor_idpyb() {
  dp = op_read();                                   //2
  cpu_c2();                                         //2a
  aa.l = op_read(OPMODE_DP, dp);                    //3
  aa.h = op_read(OPMODE_DP, dp + 1);                //4
  cpu_c4(aa.w, aa.w + regs.y.w);                    //4a
  regs.a.l ^= op_read(OPMODE_DBR, aa.w + regs.y.w); //5
  flags_eor_b();
}

void bCPU::op_eor_idpyw() {
  dp = op_read();                                       //2
  cpu_c2();                                             //2a
  aa.l = op_read(OPMODE_DP, dp);                        //3
  aa.h = op_read(OPMODE_DP, dp + 1);                    //4
  cpu_c4(aa.w, aa.w + regs.y.w);                        //4a
  regs.a.l ^= op_read(OPMODE_DBR, aa.w + regs.y.w);     //5
  regs.a.h ^= op_read(OPMODE_DBR, aa.w + regs.y.w + 1); //5a
  flags_eor_w();
}

void bCPU::op_eor_idpy() { (regs.p.m)?op_eor_idpyb():op_eor_idpyw(); }

/************************
 *** 0x57: eor [dp],y ***
 ************************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; dp
  [2a] pbr,pc+1   ; io [2]
  [3 ] 0,d+dp     ; aal
  [4 ] 0,d+dp+1   ; aah
  [5 ] 0,d+dp+2   ; aab
  [6 ] aab,aa+y   ; data low
  [6a] aab,aa+y+1 ; data high [1]
*/
void bCPU::op_eor_ildpyb() {
  dp = op_read();                                    //2
  cpu_c2();                                          //2a
  aa.l = op_read(OPMODE_DP, dp);                     //3
  aa.h = op_read(OPMODE_DP, dp + 1);                 //4
  aa.b = op_read(OPMODE_DP, dp + 2);                 //5
  regs.a.l ^= op_read(OPMODE_LONG, aa.d + regs.y.w); //6
  flags_eor_b();
}

void bCPU::op_eor_ildpyw() {
  dp = op_read();                                        //2
  cpu_c2();                                              //2a
  aa.l = op_read(OPMODE_DP, dp);                         //3
  aa.h = op_read(OPMODE_DP, dp + 1);                     //4
  aa.b = op_read(OPMODE_DP, dp + 2);                     //5
  regs.a.l ^= op_read(OPMODE_LONG, aa.d + regs.y.w);     //6
  regs.a.h ^= op_read(OPMODE_LONG, aa.d + regs.y.w + 1); //6a
  flags_eor_w();
}

void bCPU::op_eor_ildpy() { (regs.p.m)?op_eor_ildpyb():op_eor_ildpyw(); }

/**********************
 *** 0x43: eor sr,s ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; sp
  [3 ] pbr,pc+1 ; io
  [4 ] 0,s+sp   ; data low
  [4a] 0,s+sp+1 ; data high [1]
*/
void bCPU::op_eor_srb() {
  sp = op_read();                     //2
  cpu_io();                           //3
  regs.a.l ^= op_read(OPMODE_SP, sp); //4
  flags_eor_b();
}

void bCPU::op_eor_srw() {
  sp = op_read();                         //2
  cpu_io();                               //3
  regs.a.l ^= op_read(OPMODE_SP, sp);     //4
  regs.a.h ^= op_read(OPMODE_SP, sp + 1); //4a
  flags_eor_w();
}

void bCPU::op_eor_sr() { (regs.p.m)?op_eor_srb():op_eor_srw(); }

/**************************
 *** 0x53: eor (sr,s),y ***
 **************************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; sp
  [3 ] pbr,pc+1   ; io
  [4 ] 0,s+sp     ; aal
  [5 ] 0,s+sp+1   ; aah
  [6 ] 0,s+sp+1   ; io
  [7 ] dbr,aa+y   ; data low
  [7a] dbr,aa+y+1 ; data high [1]
*/
void bCPU::op_eor_isryb() {
  sp = op_read();                                   //2
  cpu_io();                                         //3
  aa.l = op_read(OPMODE_SP, sp);                    //4
  aa.h = op_read(OPMODE_SP, sp + 1);                //5
  cpu_io();                                         //6
  regs.a.l ^= op_read(OPMODE_DBR, aa.w + regs.y.w); //7
  flags_eor_b();
}

void bCPU::op_eor_isryw() {
  sp = op_read();                                       //2
  cpu_io();                                             //3
  aa.l = op_read(OPMODE_SP, sp);                        //4
  aa.h = op_read(OPMODE_SP, sp + 1);                    //5
  cpu_io();                                             //6
  regs.a.l ^= op_read(OPMODE_DBR, aa.w + regs.y.w);     //7
  regs.a.h ^= op_read(OPMODE_DBR, aa.w + regs.y.w + 1); //7a
  flags_eor_w();
}

void bCPU::op_eor_isry() { (regs.p.m)?op_eor_isryb():op_eor_isryw(); }
