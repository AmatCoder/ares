#define RENDER_MAIN 0
#define RENDER_SUB  1

#include "ppu_render.cpp"

/**********************
 *** priority table ***
 *********************************
 *** p = $2105 bit 3           ***
 *** o = tile priority bit     ***
 *** s = sprite priority (0-3) ***
 *********************************
 *** p = 0      : p = 1        ***
 *** bg4(o = 0) : bg4(o = 0)   ***
 *** bg3(o = 0) : bg3(o = 0)   ***
 *** oam(s = 0) : oam(s = 0)   ***
 *** bg4(o = 1) : bg4(o = 1)   ***
 *** bg3(o = 1) : oam(s = 1)   ***
 *** oam(s = 1) : bg2(o = 0)   ***
 *** bg2(o = 0) : bg1(o = 0)   ***
 *** bg1(o = 0) : oam(s = 2)   ***
 *** oam(s = 2) : bg2(o = 1)   ***
 *** bg2(o = 1) : bg1(o = 1)   ***
 *** bg1(o = 1) : oam(s = 3)   ***
 *** oam(s = 3) : bg3(o = 1)   ***
 ********************************/

#define debug_ppu_render_line_bg(bgnum, depth, bg, pri)                \
  if(render.##bgnum##_enabled[DEBUG_BGENABLED_ALL] == true) {          \
    if(render.##bgnum##_enabled[DEBUG_BGENABLED_PRI##pri##] == true) { \
      ppu_render_line_bg(depth, bg, pri);                              \
    }                                                                  \
  }
#define debug_ppu_render_line_oam(bgnum, pri)                          \
  if(render.##bgnum##_enabled[DEBUG_BGENABLED_ALL] == true) {          \
    if(render.##bgnum##_enabled[DEBUG_BGENABLED_PRI##pri##] == true) { \
      ppu_render_line_oam(pri);                                        \
    }                                                                  \
  }

void ppu_render_line_mode0(void) {
  if(ppu.bg_priority_mode == 0) {
    debug_ppu_render_line_bg (bg4, COLORDEPTH_4, BG4, 0);
    debug_ppu_render_line_bg (bg3, COLORDEPTH_4, BG3, 0);
    debug_ppu_render_line_oam(oam,                    0);
    debug_ppu_render_line_bg (bg4, COLORDEPTH_4, BG4, 1);
    debug_ppu_render_line_bg (bg3, COLORDEPTH_4, BG3, 1);
    debug_ppu_render_line_oam(oam,                    1);
    debug_ppu_render_line_bg (bg2, COLORDEPTH_4, BG2, 0);
    debug_ppu_render_line_bg (bg1, COLORDEPTH_4, BG1, 0);
    debug_ppu_render_line_oam(oam,                    2);
    debug_ppu_render_line_bg (bg2, COLORDEPTH_4, BG2, 1);
    debug_ppu_render_line_bg (bg1, COLORDEPTH_4, BG1, 1);
    debug_ppu_render_line_oam(oam,                    3);
  } else {
    debug_ppu_render_line_bg (bg4, COLORDEPTH_4, BG4, 0);
    debug_ppu_render_line_bg (bg3, COLORDEPTH_4, BG3, 0);
    debug_ppu_render_line_oam(oam,                    0);
    debug_ppu_render_line_bg (bg4, COLORDEPTH_4, BG4, 1);
    debug_ppu_render_line_oam(oam,                    1);
    debug_ppu_render_line_bg (bg2, COLORDEPTH_4, BG2, 0);
    debug_ppu_render_line_bg (bg1, COLORDEPTH_4, BG1, 0);
    debug_ppu_render_line_oam(oam,                    2);
    debug_ppu_render_line_bg (bg2, COLORDEPTH_4, BG2, 1);
    debug_ppu_render_line_bg (bg1, COLORDEPTH_4, BG1, 1);
    debug_ppu_render_line_oam(oam,                    3);
    debug_ppu_render_line_bg (bg3, COLORDEPTH_4, BG3, 1);
  }
}

void ppu_render_line_mode1(void) {
  if(ppu.bg_priority_mode == 0) {
    debug_ppu_render_line_bg (bg3, COLORDEPTH_4,  BG3, 0);
    debug_ppu_render_line_oam(oam,                     0);
    debug_ppu_render_line_bg (bg3, COLORDEPTH_4,  BG3, 1);
    debug_ppu_render_line_oam(oam,                     1);
    debug_ppu_render_line_bg (bg2, COLORDEPTH_16, BG2, 0);
    debug_ppu_render_line_bg (bg1, COLORDEPTH_16, BG1, 0);
    debug_ppu_render_line_oam(oam,                     2);
    debug_ppu_render_line_bg (bg2, COLORDEPTH_16, BG2, 1);
    debug_ppu_render_line_bg (bg1, COLORDEPTH_16, BG1, 1);
    debug_ppu_render_line_oam(oam,                     3);
  } else {
    debug_ppu_render_line_bg (bg3, COLORDEPTH_4,  BG3, 0);
    debug_ppu_render_line_oam(oam,                     0);
    debug_ppu_render_line_oam(oam,                     1);
    debug_ppu_render_line_bg (bg2, COLORDEPTH_16, BG2, 0);
    debug_ppu_render_line_bg (bg1, COLORDEPTH_16, BG1, 0)
    debug_ppu_render_line_oam(oam,                     2);
    debug_ppu_render_line_bg (bg2, COLORDEPTH_16, BG2, 1);
    debug_ppu_render_line_bg (bg1, COLORDEPTH_16, BG1, 1)
    debug_ppu_render_line_oam(oam,                     3);
    debug_ppu_render_line_bg (bg3, COLORDEPTH_4,  BG3, 1);
  }
}

void ppu_render_line_mode2(void) {
  if(ppu.bg_priority_mode == 0) {
    debug_ppu_render_line_oam(oam,                     0);
    debug_ppu_render_line_oam(oam,                     1);
    debug_ppu_render_line_bg (bg2, COLORDEPTH_16, BG2, 0);
    debug_ppu_render_line_bg (bg1, COLORDEPTH_16, BG1, 0);
    debug_ppu_render_line_oam(oam,                     2);
    debug_ppu_render_line_bg (bg2, COLORDEPTH_16, BG2, 1);
    debug_ppu_render_line_bg (bg1, COLORDEPTH_16, BG1, 1);
    debug_ppu_render_line_oam(oam,                     3);
  } else {
    debug_ppu_render_line_oam(oam,                     0);
    debug_ppu_render_line_oam(oam,                     1);
    debug_ppu_render_line_bg (bg2, COLORDEPTH_16, BG2, 0);
    debug_ppu_render_line_bg (bg1, COLORDEPTH_16, BG1, 0)
    debug_ppu_render_line_oam(oam,                     2);
    debug_ppu_render_line_bg (bg2, COLORDEPTH_16, BG2, 1);
    debug_ppu_render_line_bg (bg1, COLORDEPTH_16, BG1, 1)
    debug_ppu_render_line_oam(oam,                     3);
  }
}

void ppu_render_line_mode3(void) {
  if(ppu.bg_priority_mode == 0) {
    debug_ppu_render_line_oam(oam,                     0);
    debug_ppu_render_line_oam(oam,                     1);
    debug_ppu_render_line_bg(bg2, COLORDEPTH_16,  BG2, 0);
    debug_ppu_render_line_bg(bg1, COLORDEPTH_256, BG1, 0);
    debug_ppu_render_line_oam(oam,                     2);
    debug_ppu_render_line_bg(bg2, COLORDEPTH_16,  BG2, 1);
    debug_ppu_render_line_bg(bg1, COLORDEPTH_256, BG1, 1);
    debug_ppu_render_line_oam(oam,                     3);
  } else {
    debug_ppu_render_line_oam(oam,                     0);
    debug_ppu_render_line_oam(oam,                     1);
    debug_ppu_render_line_bg(bg2, COLORDEPTH_16,  BG2, 0);
    debug_ppu_render_line_bg(bg1, COLORDEPTH_256, BG1, 0);
    debug_ppu_render_line_oam(oam,                     2);
    debug_ppu_render_line_bg(bg2, COLORDEPTH_16,  BG2, 1);
    debug_ppu_render_line_bg(bg1, COLORDEPTH_256, BG1, 1);
    debug_ppu_render_line_oam(oam,                     3);
  }
}

void ppu_render_line_mode4(void) {
  if(ppu.bg_priority_mode == 0) {
    debug_ppu_render_line_oam(oam,                     0);
    debug_ppu_render_line_oam(oam,                     1);
    debug_ppu_render_line_bg(bg2, COLORDEPTH_4,   BG2, 0);
    debug_ppu_render_line_bg(bg1, COLORDEPTH_256, BG1, 0);
    debug_ppu_render_line_oam(oam,                     2);
    debug_ppu_render_line_bg(bg2, COLORDEPTH_4,   BG2, 1);
    debug_ppu_render_line_bg(bg1, COLORDEPTH_256, BG1, 1);
    debug_ppu_render_line_oam(oam,                     3);
  } else {
    debug_ppu_render_line_oam(oam,                     0);
    debug_ppu_render_line_oam(oam,                     1);
    debug_ppu_render_line_bg(bg2, COLORDEPTH_4,   BG2, 0);
    debug_ppu_render_line_bg(bg1, COLORDEPTH_256, BG1, 0);
    debug_ppu_render_line_oam(oam,                     2);
    debug_ppu_render_line_bg(bg2, COLORDEPTH_4,   BG2, 1);
    debug_ppu_render_line_bg(bg1, COLORDEPTH_256, BG1, 1);
    debug_ppu_render_line_oam(oam,                     3);
  }
}

void ppu_render_line_mode5(void) {
  debug_ppu_render_line_bg(bg2, COLORDEPTH_4,  BG2, 0);
  debug_ppu_render_line_bg(bg1, COLORDEPTH_16, BG1, 0);
  debug_ppu_render_line_bg(bg2, COLORDEPTH_4,  BG2, 0);
  debug_ppu_render_line_bg(bg1, COLORDEPTH_16, BG1, 1);
}

void ppu_render_line_mode6(void) {
  debug_ppu_render_line_bg(bg1, COLORDEPTH_16, BG1, 0);
  debug_ppu_render_line_bg(bg1, COLORDEPTH_16, BG1, 1);
}

void ppu_render_line_mode7(void) {
  if(ppu.bg_priority_mode == 0) {
    debug_ppu_render_line_oam(oam, 0);
    debug_ppu_render_line_oam(oam, 1);
    ppu_render_line_m7();
    debug_ppu_render_line_oam(oam, 2);
    debug_ppu_render_line_oam(oam, 3);
  } else {
    debug_ppu_render_line_oam(oam, 0);
    debug_ppu_render_line_oam(oam, 1);
    ppu_render_line_m7();
    debug_ppu_render_line_oam(oam, 2);
    debug_ppu_render_line_oam(oam, 3);
  }
}

void ppu_render_scanline(void) {
int x, y;
  ppu.hirq_triggered = false;
  ppu.vline_pos++;
  if(ppu.vline_pos > 261)ppu.vline_pos = 0;

//new screen initialize
  if(ppu.vline_pos == 0) {
    ppu.virq_triggered = false;
    ppu.active_hdma_channels = ppu.toggle_active_hdma_channels;
    if(ppu.visible_scanlines != ppu.toggle_visible_scanlines) {
      ppu.visible_scanlines = ppu.toggle_visible_scanlines;
      video_setsnesmode();
      video_setmode(false, 512, ppu.visible_scanlines * 2);
    }
    gx816->nmi_pin = 1;
  }

//screen refresh
  if(ppu.vline_pos == ppu.visible_scanlines) {
    if(render.frame_count == 0) {
      UpdateDisplay();
    }
    render.frame_count++;
    if(render.frame_count >= render.frame_skip) {
      render.frame_count = 0;
    }
    ppu.interlace_frame ^= 1;
  }

//automatic joypad read
  if(ppu.vline_pos == (ppu.visible_scanlines + 1) && ppu.auto_joypad_read == true) {
    UpdateJoypad();
  }

//nmi
  if(ppu.vline_pos == (ppu.visible_scanlines + 8) && gx816->nmi_pin == 1) {
    gx816->nmi_pin = 0;
    if(gx816->nmi_enabled == true) {
      gx816->InvokeIRQ(0xffea);
    }
  }

  UpdateHDMA(); //found in ppu_dma.cpp
  y = ppu.vline_pos;
  if(y < ppu.visible_scanlines && (render.frame_skip == 0 || render.frame_count == 0)) {
    if(ppu.display_disable == true) {
      memset(ppu.screen + y * 512, 0, 1024);
    } else {
      ppu_clear_pixel_cache();
      switch(ppu.bg_mode) {
      case 0:ppu_render_line_mode0();break;
      case 1:ppu_render_line_mode1();break;
      case 2:ppu_render_line_mode2();break;
      case 3:ppu_render_line_mode3();break;
      case 4:ppu_render_line_mode4();break;
      case 5:ppu_render_line_mode5();break;
      case 6:ppu_render_line_mode6();break;
      case 7:ppu_render_line_mode7();break;
      }
      ppu_render_line_to_screen();
    }
  }
}

void ppu_update_scanline(void) {
//starting a new screen?
  if(ppu.vline_pos > snes_time->vscan_pos) {
    while(ppu.vline_pos != 0) {
      ppu_render_scanline();
    }
  }

  while(snes_time->vscan_pos > ppu.vline_pos) {
    ppu_render_scanline();
  }

  if(!(gx816->regs.p & PF_I)) {
    if(ppu.vcounter_enabled == true && ppu.hcounter_enabled == true) {
      if(snes_time->vscan_pos == ppu.virq_pos && ppu.virq_triggered == false) {
        if(snes_time->hscan_pos >= ppu.hirq_pos && ppu.hirq_triggered == false) {
          ppu.irq_triggered      = true;
          ppu.virq_triggered     = true;
          ppu.hirq_triggered     = true;
          gx816->InvokeIRQ(0xffee);
        }
      }
    } else if(ppu.vcounter_enabled == true) {
      if(snes_time->vscan_pos == ppu.virq_pos && ppu.virq_triggered == false) {
        ppu.irq_triggered      = true;
        ppu.virq_triggered     = true;
        gx816->InvokeIRQ(0xffee);
      }
    } else if(ppu.hcounter_enabled == true) {
      if(snes_time->hscan_pos >= ppu.hirq_pos && ppu.hirq_triggered == false) {
        ppu.irq_triggered      = true;
        ppu.hirq_triggered     = true;
        gx816->InvokeIRQ(0xffee);
      }
    }
  }
}

byte oam_read(word addr) {
  addr &= 1023;
  if(addr >= 512) {
    addr &= 31;
    return ppu.oam[addr + 512];
  } else {
    return ppu.oam[addr];
  }
}

void oam_write(word addr, byte value) {
  addr &= 1023;
  if(addr >= 512) {
    addr &= 31;
    ppu.oam[addr + 512] = value;
  } else {
    ppu.oam[addr] = value;
  }
}

void PPUInit(byte first_time) {
int i, l;
byte r, g, b;
double m;
word *ptr;
  if(first_time == 1) {
    ppu.screen      = (word*)malloc(512 * 480 * 2);
    ppu.vram        = (byte*)malloc(0x10000);
    ppu.cgram       = (byte*)malloc(512);
    ppu.oam         = (byte*)malloc(544);
    ppu.light_table = (word*)malloc(16 * 65536 * 2);
    ppu_init_tiledata_cache();

    for(l=0;l<16;l++) {
      ppu.mosaic_table[l] = (word*)malloc(4096 * 2);
      for(i=0;i<4096;i++) {
        ppu.mosaic_table[l][i] = (i / (l + 1)) * (l + 1);
      }
    }

    ptr = (word*)ppu.light_table;
    for(l=0;l<16;l++) {
      m = (double)l / 15.0;
      for(i=0;i<65536;i++) {
        r = (i      ) & 31;
        g = (i >>  5) & 31;
        b = (i >> 10) & 31;
        if(l == 0) { r = g = b = 0; }
        else if(l == 15);
        else {
          r = (double)r * m;
          g = (double)g * m;
          b = (double)b * m;
        }
        *ptr++ = (r) | (g << 5) | (b << 10);
      }
    }
  }
  ppu_clear_pixel_cache();
  memset(ppu.screen, 0, 512 * 480 * 2);
  memset(ppu.vram,   0, 0x10000);
  memset(ppu.cgram,  0, 512);
  memset(ppu.oam,    0, 544);
  ppu.ppu_cycles               = 0;
  ppu.ppu_prev_cycles          = 0;
  ppu.display_disable          = true;
  ppu.display_brightness       = 15;
  ppu.visible_scanlines        = 224;
  ppu.toggle_visible_scanlines = 224;

  ppu.interlace        = false;
  ppu.interlace_frame  = 0;
  ppu.hline_pos        = 0;
  ppu.vline_pos        = 261;
  ppu.irq_triggered    = false;
  ppu.virq_triggered   = false;
  ppu.hirq_triggered   = false;
  ppu.vram_write_pos   = 0;
  ppu.cgram_write_pos  = 0;
  ppu.wram_write_pos   = 0;
  ppu.vram_inc_size    = 2;
  ppu.vram_inc_reg     = 0;
  ppu.oam_write_pos    = 0;
  ppu.oam_tiledata_loc = 0;

  ppu.bg_enabled[OAM]              = false;
  ppu.ss_bg_enabled[OAM]           = false;
  ppu.mosaic_size                  = 0;
  ppu.mosaic_enabled[BG4]          = false;
  ppu.mosaic_enabled[BG3]          = false;
  ppu.mosaic_enabled[BG2]          = false;
  ppu.mosaic_enabled[BG1]          = false;
  ppu.bg_windowing_enabled[OAM]    = false;
  ppu.ss_bg_windowing_enabled[OAM] = false;
  ppu.bg_window1_enabled[OAM]      = false;
  ppu.bg_window2_enabled[OAM]      = false;
  ppu.bg_window1_clipmode[OAM]     = false;
  ppu.bg_window2_clipmode[OAM]     = false;
  ppu.bg_window_mask[OAM]          = false;

  for(i=0;i<4;i++) {
    ppu.bg_enabled[i]              = false;
    ppu.ss_bg_enabled[i]           = false;
    ppu.bg_window1_enabled[i]      = false;
    ppu.bg_window2_enabled[i]      = false;
    ppu.bg_windowing_enabled[i]    = false;
    ppu.ss_bg_windowing_enabled[i] = false;
    ppu.bg_window1_clipmode[i]     = 0;
    ppu.bg_window2_clipmode[i]     = 0;
    ppu.bg_window_mask[i]          = 0;
    ppu.bg_tilemap_loc[i]          = 0;
    ppu.bg_tiledata_loc[i]         = 0;
    ppu.bg_hscroll_pos[i]          = 0;
    ppu.bg_vscroll_pos[i]          = 0;
  }
  ppu.bg_priority_mode    = 0;
  ppu.oam_base_size       = 0;
  ppu.oam_name_sel        = 0;
  ppu.bg_mode             = 0;

  ppu.mul_a               = 0;
  ppu.mul_b               = 0;
  ppu.div_a               = 0;
  ppu.div_b               = 0;
  ppu.r_4214              = 0;
  ppu.r_4216              = 0;

  ppu.smul_a              = 0;
  ppu.smul_b              = 0;
  ppu.smul_r              = 0;

  ppu.window1_left        = 0;
  ppu.window1_right       = 0;
  ppu.window2_left        = 0;
  ppu.window2_right       = 0;

  ppu.color_window1_enabled  = 0;
  ppu.color_window2_enabled  = 0;
  ppu.color_window1_clipmode = 0;
  ppu.color_window2_clipmode = 0;
  ppu.color_window_mask      = 0;
  ppu.color_mask             = 0;
  ppu.ss_color_mask          = 0;
  ppu.addsub_mode            = 0;
  ppu.color_mode             = 0;
  ppu.color_halve            = 0;
  for(i=0;i<6;i++) {
    ppu.bg_color_enabled[i]  = false;
  }
  ppu.color_r = 0;
  ppu.color_g = 0;
  ppu.color_b = 0;

  ppu.toggle_active_hdma_channels = 0;
  ppu.active_hdma_channels        = 0;
  for(i=0;i<8;i++) {
    memset(&dma_channel[i], 0, sizeof(dmachannel));
    ppu.hdma_completed[i]           = false;
    ppu.hdma_scanlines_remaining[i] = 0;
    ppu.hdma_index_pointer[i]       = 0;
  }

  ppu.vcounter_enabled = false;
  ppu.hcounter_enabled = false;
  ppu.hirq_pos         = 0;
  ppu.virq_pos         = 0;

  ppu.auto_joypad_read    = false;
  ppu.joypad_strobe_value = 0;

  ppu.latch_toggle        = 0;
  ppu.latch_vpos          = 0;
  ppu.latch_hpos          = 0;

  ppu.m7a = ppu.m7b =
  ppu.m7c = ppu.m7d =
  ppu.m7x = ppu.m7y = 0;

  ppu.m7hofs = ppu.m7vofs = 0x0000;

  ppu.mode7_hflip = false;
  ppu.mode7_vflip = false;
}
