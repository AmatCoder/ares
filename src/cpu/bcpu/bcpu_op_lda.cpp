void bCPU::flags_lda_b() {
  regs.p.n = !!(regs.a.l & 0x80);
  regs.p.z = (regs.a.l == 0);
}

void bCPU::flags_lda_w() {
  regs.p.n = !!(regs.a.w & 0x8000);
  regs.p.z = (regs.a.w == 0);
}

/************************
 *** 0xa9: lda #const ***
 ************************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; idl
  [2a] pbr,pc+2 ; idh [1]
*/
void bCPU::op_lda_constb() {
  regs.a.l = op_read(); //2
  flags_lda_b();
}

void bCPU::op_lda_constw() {
  regs.a.l = op_read(); //2
  regs.a.h = op_read(); //2a
  flags_lda_w();
}

void bCPU::op_lda_const() { (regs.p.m)?op_lda_constb():op_lda_constw(); }

/**********************
 *** 0xad: lda addr ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; aal
  [3 ] pbr,pc+2 ; aah
  [4 ] dbr,aa   ; data low
  [4a] dbr,aa+1 ; data high [1]
*/
void bCPU::op_lda_addrb() {
  aa.l = op_read();                     //2
  aa.h = op_read();                     //3
  regs.a.l = op_read(OPMODE_DBR, aa.w); //4
  flags_lda_b();
}

void bCPU::op_lda_addrw() {
  aa.l = op_read();                         //2
  aa.h = op_read();                         //3
  regs.a.l = op_read(OPMODE_DBR, aa.w);     //4
  regs.a.h = op_read(OPMODE_DBR, aa.w + 1); //4a
  flags_lda_w();
}

void bCPU::op_lda_addr() { (regs.p.m)?op_lda_addrb():op_lda_addrw(); }

/************************
 *** 0xbd: lda addr,x ***
 ************************
cycles:
  [1 ] pbr,pc         ; operand
  [2 ] pbr,pc+1       ; aal
  [3 ] pbr,pc+2       ; aah
  [3a] dbr,aah,aal+xl ; io [4]
  [4 ] dbr,aa+x       ; data low
  [4a] dbr,aa+x+1     ; data high [1]
*/
void bCPU::op_lda_addrxb() {
  aa.l = op_read();                                //2
  aa.h = op_read();                                //3
  cpu_c4(aa.w, aa.w + regs.x.w);                   //3a
  regs.a.l = op_read(OPMODE_DBR, aa.w + regs.x.w); //4
  flags_lda_b();
}

void bCPU::op_lda_addrxw() {
  aa.l = op_read();                                    //2
  aa.h = op_read();                                    //3
  cpu_c4(aa.w, aa.w + regs.x.w);                       //3a
  regs.a.l = op_read(OPMODE_DBR, aa.w + regs.x.w);     //4
  regs.a.h = op_read(OPMODE_DBR, aa.w + regs.x.w + 1); //4a
  flags_lda_w();
}

void bCPU::op_lda_addrx() { (regs.p.m)?op_lda_addrxb():op_lda_addrxw(); }

/********************
 *** 0xa5: lda dp ***
 ********************
cycles:
  [1 ] pbr,pc   ; operand
  [2 ] pbr,pc+1 ; dp
  [2a] pbr,pc+1 ; io [2]
  [3 ] 0,d+dp   ; data low
  [3a] 0,d+dp+1 ; data high [1]
*/
void bCPU::op_lda_dpb() {
  dp = op_read();                    //2
  cpu_c2();                          //2a
  regs.a.l = op_read(OPMODE_DP, dp); //3
  flags_lda_b();
}

void bCPU::op_lda_dpw() {
  dp = op_read();                        //2
  cpu_c2();                              //2a
  regs.a.l = op_read(OPMODE_DP, dp);     //3
  regs.a.h = op_read(OPMODE_DP, dp + 1); //3a
  flags_lda_w();
}

void bCPU::op_lda_dp() { (regs.p.m)?op_lda_dpb():op_lda_dpw(); }

/**********************
 *** 0xb2: lda (dp) ***
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
void bCPU::op_lda_idpb() {
  dp = op_read();                       //2
  cpu_c2();                             //2a
  aa.l = op_read(OPMODE_DP, dp);        //3
  aa.h = op_read(OPMODE_DP, dp + 1);    //4
  regs.a.l = op_read(OPMODE_DBR, aa.w); //5
  flags_lda_b();
}

void bCPU::op_lda_idpw() {
  dp = op_read();                           //2
  cpu_c2();                                 //2a
  aa.l = op_read(OPMODE_DP, dp);            //3
  aa.h = op_read(OPMODE_DP, dp + 1);        //4
  regs.a.l = op_read(OPMODE_DBR, aa.w);     //5
  regs.a.h = op_read(OPMODE_DBR, aa.w + 1); //5
  flags_lda_w();
}

void bCPU::op_lda_idp() { (regs.p.m)?op_lda_idpb():op_lda_idpw(); }

/**********************
 *** 0xa7: lda [dp] ***
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
void bCPU::op_lda_ildpb() {
  dp = op_read();                        //2
  cpu_c2();                              //2a
  aa.l = op_read(OPMODE_DP, dp);         //3
  aa.h = op_read(OPMODE_DP, dp + 1);     //4
  aa.b = op_read(OPMODE_DP, dp + 2);     //5
  regs.a.l = op_read(OPMODE_LONG, aa.d); //6
  flags_lda_b();
}

void bCPU::op_lda_ildpw() {
  dp = op_read();                            //2
  cpu_c2();                                  //2a
  aa.l = op_read(OPMODE_DP, dp);             //3
  aa.h = op_read(OPMODE_DP, dp + 1);         //4
  aa.b = op_read(OPMODE_DP, dp + 2);         //5
  regs.a.l = op_read(OPMODE_LONG, aa.d);     //6
  regs.a.h = op_read(OPMODE_LONG, aa.d + 1); //6a
  flags_lda_w();
}

void bCPU::op_lda_ildp() { (regs.p.m)?op_lda_ildpb():op_lda_ildpw(); }

/**********************
 *** 0xaf: lda long ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; aal
  [3 ] pbr,pc+2 ; aah
  [4 ] pbr,pc+3 ; aab
  [5 ] aab,aa   ; data low
  [5a] aab,aa+1 ; data high
*/
void bCPU::op_lda_longb() {
  aa.l = op_read();                      //2
  aa.h = op_read();                      //3
  aa.b = op_read();                      //4
  regs.a.l = op_read(OPMODE_LONG, aa.d); //5
  flags_lda_b();
}

void bCPU::op_lda_longw() {
  aa.l = op_read();                          //2
  aa.h = op_read();                          //3
  aa.b = op_read();                          //4
  regs.a.l = op_read(OPMODE_LONG, aa.d);     //5
  regs.a.h = op_read(OPMODE_LONG, aa.d + 1); //5a
  flags_lda_w();
}

void bCPU::op_lda_long() { (regs.p.m)?op_lda_longb():op_lda_longw(); }

/************************
 *** 0xbf: lda long,x ***
 ************************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; aal
  [3 ] pbr,pc+2   ; aah
  [4 ] pbr,pc+3   ; aab
  [5 ] aab,aa+x   ; data low
  [5a] aab,aa+x+1 ; data high
*/
void bCPU::op_lda_longxb() {
  aa.l = op_read();                                 //2
  aa.h = op_read();                                 //3
  aa.b = op_read();                                 //4
  regs.a.l = op_read(OPMODE_LONG, aa.d + regs.x.w); //5
  flags_lda_b();
}

void bCPU::op_lda_longxw() {
  aa.l = op_read();                                     //2
  aa.h = op_read();                                     //3
  aa.b = op_read();                                     //4
  regs.a.l = op_read(OPMODE_LONG, aa.d + regs.x.w);     //5
  regs.a.h = op_read(OPMODE_LONG, aa.d + regs.x.w + 1); //5a
  flags_lda_w();
}

void bCPU::op_lda_longx() { (regs.p.m)?op_lda_longxb():op_lda_longxw(); }

/************************
 *** 0xb9: lda addr,y ***
 ************************
cycles:
  [1 ] pbr,pc         ; operand
  [2 ] pbr,pc+1       ; aal
  [3 ] pbr,pc+2       ; aah
  [3a] dbr,aah,aal+yl ; io [4]
  [4 ] dbr,aa+y       ; data low
  [4a] dbr,aa+y+1     ; data high [1]
*/
void bCPU::op_lda_addryb() {
  aa.l = op_read();                                //2
  aa.h = op_read();                                //3
  cpu_c4(aa.w, aa.w + regs.y.w);                   //3a
  regs.a.l = op_read(OPMODE_DBR, aa.w + regs.y.w); //4
  flags_lda_b();
}

void bCPU::op_lda_addryw() {
  aa.l = op_read();                                    //2
  aa.h = op_read();                                    //3
  cpu_c4(aa.w, aa.w + regs.y.w);                       //3a
  regs.a.l = op_read(OPMODE_DBR, aa.w + regs.y.w);     //4
  regs.a.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1); //4a
  flags_lda_w();
}

void bCPU::op_lda_addry() { (regs.p.m)?op_lda_addryb():op_lda_addryw(); }

/**********************
 *** 0xb5: lda dp,x ***
 **********************
cycles:
  [1 ] pbr,pc     ; opcode
  [2 ] pbr,pc+1   ; dp
  [2a] pbr,pc+1   ; io
  [3 ] pbr,pc+1   ; io
  [4 ] 0,d+dp+x   ; data low
  [4a] 0,d+dp+x+1 ; data high
*/
void bCPU::op_lda_dpxb() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  regs.a.l = op_read(OPMODE_DP, dp + regs.x.w); //4
  flags_lda_b();
}

void bCPU::op_lda_dpxw() {
  dp = op_read();                                   //2
  cpu_c2();                                         //2a
  cpu_io();                                         //3
  regs.a.l = op_read(OPMODE_DP, dp + regs.x.w);     //4
  regs.a.h = op_read(OPMODE_DP, dp + regs.x.w + 1); //4a
  flags_lda_w();
}

void bCPU::op_lda_dpx() { (regs.p.m)?op_lda_dpxb():op_lda_dpxw(); }

/************************
 *** 0xa1: lda (dp,x) ***
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
void bCPU::op_lda_idpxb() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  aa.l = op_read(OPMODE_DP, dp + regs.x.w);     //4
  aa.h = op_read(OPMODE_DP, dp + regs.x.w + 1); //5
  regs.a.l = op_read(OPMODE_DBR, aa.w);         //6
  flags_lda_b();
}

void bCPU::op_lda_idpxw() {
  dp = op_read();                               //2
  cpu_c2();                                     //2a
  cpu_io();                                     //3
  aa.l = op_read(OPMODE_DP, dp + regs.x.w);     //4
  aa.h = op_read(OPMODE_DP, dp + regs.x.w + 1); //5
  regs.a.l = op_read(OPMODE_DBR, aa.w);         //6
  regs.a.h = op_read(OPMODE_DBR, aa.w + 1);     //6a
  flags_lda_w();
}

void bCPU::op_lda_idpx() { (regs.p.m)?op_lda_idpxb():op_lda_idpxw(); }

/************************
 *** 0xb1: lda (dp),y ***
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
void bCPU::op_lda_idpyb() {
  dp = op_read();                                  //2
  cpu_c2();                                        //2a
  aa.l = op_read(OPMODE_DP, dp);                   //3
  aa.h = op_read(OPMODE_DP, dp + 1);               //4
  cpu_c4(aa.w, aa.w + regs.y.w);                   //4a
  regs.a.l = op_read(OPMODE_DBR, aa.w + regs.y.w); //5
  flags_lda_b();
}

void bCPU::op_lda_idpyw() {
  dp = op_read();                                      //2
  cpu_c2();                                            //2a
  aa.l = op_read(OPMODE_DP, dp);                       //3
  aa.h = op_read(OPMODE_DP, dp + 1);                   //4
  cpu_c4(aa.w, aa.w + regs.y.w);                       //4a
  regs.a.l = op_read(OPMODE_DBR, aa.w + regs.y.w);     //5
  regs.a.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1); //5a
  flags_lda_w();
}

void bCPU::op_lda_idpy() { (regs.p.m)?op_lda_idpyb():op_lda_idpyw(); }

/************************
 *** 0xb7: lda [dp],y ***
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
void bCPU::op_lda_ildpyb() {
  dp = op_read();                                   //2
  cpu_c2();                                         //2a
  aa.l = op_read(OPMODE_DP, dp);                    //3
  aa.h = op_read(OPMODE_DP, dp + 1);                //4
  aa.b = op_read(OPMODE_DP, dp + 2);                //5
  regs.a.l = op_read(OPMODE_LONG, aa.d + regs.y.w); //6
  flags_lda_b();
}

void bCPU::op_lda_ildpyw() {
  dp = op_read();                                       //2
  cpu_c2();                                             //2a
  aa.l = op_read(OPMODE_DP, dp);                        //3
  aa.h = op_read(OPMODE_DP, dp + 1);                    //4
  aa.b = op_read(OPMODE_DP, dp + 2);                    //5
  regs.a.l = op_read(OPMODE_LONG, aa.d + regs.y.w);     //6
  regs.a.h = op_read(OPMODE_LONG, aa.d + regs.y.w + 1); //6a
  flags_lda_w();
}

void bCPU::op_lda_ildpy() { (regs.p.m)?op_lda_ildpyb():op_lda_ildpyw(); }

/**********************
 *** 0xa3: lda sr,s ***
 **********************
cycles:
  [1 ] pbr,pc   ; opcode
  [2 ] pbr,pc+1 ; sp
  [3 ] pbr,pc+1 ; io
  [4 ] 0,s+sp   ; data low
  [4a] 0,s+sp+1 ; data high [1]
*/
void bCPU::op_lda_srb() {
  sp = op_read();                    //2
  cpu_io();                          //3
  regs.a.l = op_read(OPMODE_SP, sp); //4
  flags_lda_b();
}

void bCPU::op_lda_srw() {
  sp = op_read();                        //2
  cpu_io();                              //3
  regs.a.l = op_read(OPMODE_SP, sp);     //4
  regs.a.h = op_read(OPMODE_SP, sp + 1); //4a
  flags_lda_w();
}

void bCPU::op_lda_sr() { (regs.p.m)?op_lda_srb():op_lda_srw(); }

/**************************
 *** 0xb3: lda (sr,s),y ***
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
void bCPU::op_lda_isryb() {
  sp = op_read();                                  //2
  cpu_io();                                        //3
  aa.l = op_read(OPMODE_SP, sp);                   //4
  aa.h = op_read(OPMODE_SP, sp + 1);               //5
  cpu_io();                                        //6
  regs.a.l = op_read(OPMODE_DBR, aa.w + regs.y.w); //7
  flags_lda_b();
}

void bCPU::op_lda_isryw() {
  sp = op_read();                                      //2
  cpu_io();                                            //3
  aa.l = op_read(OPMODE_SP, sp);                       //4
  aa.h = op_read(OPMODE_SP, sp + 1);                   //5
  cpu_io();                                            //6
  regs.a.l = op_read(OPMODE_DBR, aa.w + regs.y.w);     //7
  regs.a.h = op_read(OPMODE_DBR, aa.w + regs.y.w + 1); //7a
  flags_lda_w();
}

void bCPU::op_lda_isry() { (regs.p.m)?op_lda_isryb():op_lda_isryw(); }
