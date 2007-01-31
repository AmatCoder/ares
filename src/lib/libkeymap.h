/*
  libkeymap : version 0.02 ~byuu (2006-07-30)
*/

#ifndef __LIBKEYMAP
#define __LIBKEYMAP

class keymap {
private:
char tmp[32];

public:

uint null;
uint esc;
uint f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12;
uint print_screen, scroll_lock, pause;
uint tilde;
uint num_0, num_1, num_2, num_3, num_4;
uint num_5, num_6, num_7, num_8, num_9;
uint minus, plus, backspace;
uint ins, del, home, end, page_up, page_down;
uint a, b, c, d, e, f, g, h, i, j, k, l, m;
uint n, o, p, q, r, s, t, u, v, w, x, y, z;
uint lbracket, rbracket;
uint pipe, colon, quote, comma, period, question;
uint numpad_0, numpad_1, numpad_2, numpad_3, numpad_4;
uint numpad_5, numpad_6, numpad_7, numpad_8, numpad_9;
uint numpad_plus, numpad_minus, numpad_mul;
uint numpad_div, numpad_enter, numpad_point;
uint numlock, capslock;
uint up, down, left, right;
uint tab, enter, space;
uint lctrl, rctrl, lalt, ralt, lshift, rshift;
uint lwin, rwin, menu;

struct {
  uint up, down, left, right;
  uint button[128];
} joypad[16];

  uint find(const char *key) {
  #define match(n) if(!strcmp(#n, key))return n;
    match(null)
    match(esc)
    match(f1) match(f2) match(f3) match(f4) match(f5) match(f6)
    match(f7) match(f8) match(f9) match(f10) match(f11) match(f12)
    match(print_screen) match(scroll_lock) match(pause)
    match(tilde)
    match(num_0) match(num_1) match(num_2) match(num_3) match(num_4)
    match(num_5) match(num_6) match(num_7) match(num_8) match(num_9)
    match(minus) match(plus) match(backspace)
    match(ins) match(del) match(home)
    match(end) match(page_up) match(page_down)
    match(a) match(b) match(c) match(d) match(e) match(f)
    match(g) match(h) match(i) match(j) match(k) match(l)
    match(m) match(n) match(o) match(p) match(q) match(r)
    match(s) match(t) match(u) match(v) match(w) match(x)
    match(y) match(z)
    match(lbracket) match(rbracket)
    match(pipe) match(colon) match(quote)
    match(comma) match(period) match(question)
    match(numpad_0) match(numpad_1) match(numpad_2) match(numpad_3)
    match(numpad_4) match(numpad_5) match(numpad_6) match(numpad_7)
    match(numpad_8) match(numpad_9)
    match(numpad_plus) match(numpad_minus) match(numpad_mul)
    match(numpad_div) match(numpad_enter) match(numpad_point)
    match(numlock) match(capslock)
    match(up) match(down) match(left) match(right)
    match(tab) match(enter) match(space)
    match(lctrl) match(rctrl) match(lalt)
    match(ralt) match(lshift) match(rshift)
    match(lwin) match(rwin) match(menu)
  #undef match

    if(!memcmp(key, "joypad", 6)) {
    const char *p = key + 6;
    int joy, bn, n;
      sscanf(p, "%d%n", &joy, &n);
      p += n;
      if(*p == '.') {
        p++;
        if(!strcmp(p, "up"))    { return joypad[joy].up;    }
        if(!strcmp(p, "down"))  { return joypad[joy].down;  }
        if(!strcmp(p, "left"))  { return joypad[joy].left;  }
        if(!strcmp(p, "right")) { return joypad[joy].right; }
        if(!memcmp(p, "button", 6)) {
          p += 6;
          sscanf(p, "%d", &bn);
          return joypad[joy].button[bn];
        }
      }
    }

    return 0;
  }

  const char *find(uint key) {
  #define match(n) if(n == key)return #n;
    match(null)
    match(esc)
    match(f1) match(f2) match(f3) match(f4) match(f5) match(f6)
    match(f7) match(f8) match(f9) match(f10) match(f11) match(f12)
    match(print_screen) match(scroll_lock) match(pause)
    match(tilde)
    match(num_0) match(num_1) match(num_2) match(num_3) match(num_4)
    match(num_5) match(num_6) match(num_7) match(num_8) match(num_9)
    match(minus) match(plus) match(backspace)
    match(ins) match(del) match(home)
    match(end) match(page_up) match(page_down)
    match(a) match(b) match(c) match(d) match(e) match(f)
    match(g) match(h) match(i) match(j) match(k) match(l)
    match(m) match(n) match(o) match(p) match(q) match(r)
    match(s) match(t) match(u) match(v) match(w) match(x)
    match(y) match(z)
    match(lbracket) match(rbracket)
    match(pipe) match(colon) match(quote)
    match(comma) match(period) match(question)
    match(numpad_0) match(numpad_1) match(numpad_2) match(numpad_3)
    match(numpad_4) match(numpad_5) match(numpad_6) match(numpad_7)
    match(numpad_8) match(numpad_9)
    match(numpad_plus) match(numpad_minus) match(numpad_mul)
    match(numpad_div) match(numpad_enter) match(numpad_point)
    match(numlock) match(capslock)
    match(up) match(down) match(left) match(right)
    match(tab) match(enter) match(space)
    match(lctrl) match(rctrl) match(lalt)
    match(ralt) match(lshift) match(rshift)
    match(lwin) match(rwin) match(menu)
  #undef match

    for(int joy = 0; joy < 16; joy++) {
      if(joypad[joy].up    == key) { sprintf(tmp, "joypad%d.up",    joy); return tmp; }
      if(joypad[joy].down  == key) { sprintf(tmp, "joypad%d.down",  joy); return tmp; }
      if(joypad[joy].left  == key) { sprintf(tmp, "joypad%d.left",  joy); return tmp; }
      if(joypad[joy].right == key) { sprintf(tmp, "joypad%d.right", joy); return tmp; }
      for(int bn = 0; bn < 128; bn++) {
        if(joypad[joy].button[bn] == key) { sprintf(tmp, "joypad%d.button%d", joy, bn); return tmp; }
      }
    }

    return "null";
  }

  keymap() {
    null = 0;
    esc = 0;
    f1 = f2 = f3 = f4 = f5 = f6 = 0;
    f7 = f8 = f9 = f10 = f11 = f12 = 0;
    print_screen = scroll_lock = pause = 0;
    tilde = 0;
    num_0 = num_1 = num_2 = num_3 = num_4 = 0;
    num_5 = num_6 = num_7 = num_8 = num_9 = 0;
    minus = plus = backspace = 0;
    ins = del = home = end = page_up = page_down = 0;
    a = b = c = d = e = f = g = h = i = 0;
    j = k = l = m = n = o = p = q = r = 0;
    s = t = u = v = w = x = y = z = 0;
    lbracket = rbracket = 0;
    pipe = colon = quote = comma = period = question = 0;
    numpad_0 = numpad_1 = numpad_2 = numpad_3 = numpad_4 = 0;
    numpad_5 = numpad_6 = numpad_7 = numpad_8 = numpad_9 = 0;
    numpad_plus = numpad_minus = numpad_mul = 0;
    numpad_div = numpad_enter = numpad_point = 0;
    numlock = capslock = 0;
    up = down = left = right = 0;
    tab = enter = space = 0;
    lctrl = rctrl = lalt = ralt = lshift = rshift = 0;
    lwin = rwin = menu = 0;

    for(int joy = 0; joy < 16; joy++) {
      joypad[joy].up    = 0;
      joypad[joy].down  = 0;
      joypad[joy].left  = 0;
      joypad[joy].right = 0;
      memset(joypad[joy].button, 0, sizeof(joypad[joy].button));
    }
  }

};

#endif
