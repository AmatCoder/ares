inline uint16 bPPU::get_palette(uint8 index) {
  return *((uint16*)cgram + index);
}

inline uint16 bPPU::get_direct_color(uint8 p, uint8 t) {
//p = 00000bgr
//t = BBGGGRRR
//r = 0BBb00GGGg0RRRr0
  return ((t & 7) << 2) | ((p & 1) << 1) |
  (((t >> 3) & 7) << 7) | (((p >> 1) & 1) << 6) |
  ((t >> 6) << 13) | ((p >> 2) << 12);
}

inline uint16 bPPU::get_pixel(int x) {
_pixel *p = &pixel_cache[x];
uint16 _r, src_back = get_palette(0);
  if(p->bg_main && p->bg_sub) {
    if(p->color_exempt == false && regs.bg_color_enabled[p->bg_main & 0x7f] && sub_colorwindowtable[x]) {
      if(regs.addsub_mode) {
        _r = addsub_pixels(p->src_main, p->src_sub);
      } else {
        _r = addsub_pixel(p->src_main);
      }
    } else {
      _r = p->src_main;
    }
  } else if(p->bg_main) {
    if(p->color_exempt == false && regs.bg_color_enabled[p->bg_main & 0x7f] && sub_colorwindowtable[x]) {
      _r = addsub_pixel(p->src_main);
    } else {
      _r = p->src_main;
    }
  } else if(p->bg_sub) {
    if(regs.bg_color_enabled[BACK]) {
      if(sub_colorwindowtable[x]) {
        if(regs.addsub_mode) {
          _r = addsub_pixels(src_back, p->src_sub);
        } else {
          _r = addsub_pixel(src_back);
        }
      } else {
        _r = src_back;
      }
    } else {
      _r = 0x0000;
    }
  } else {
    if(main_colorwindowtable[x]) {
      if(regs.bg_color_enabled[BACK] && sub_colorwindowtable[x]) {
        _r = addsub_pixel(src_back);
      } else {
        _r = src_back;
      }
    } else {
      _r = 0x0000;
    }
  }
  return _r;
}

inline void bPPU::render_line_output() {
int x;
uint16 _r;
uint16 *ptr;
  ptr = (uint16*)snes->get_ppu_output_handle();

uint16 *ltable;
  ltable = (uint16*)light_table + (regs.display_brightness << 15);

  if(_screen_width == 256) {
    for(x=0;x<256;x++) {
      _r = get_pixel(x);
      *ptr++ = *(ltable + _r);
    }
  } else {
    for(x=0;x<512;x++) {
      _r = get_pixel(x);
      *ptr++ = *(ltable + _r);
    }
  }
}
