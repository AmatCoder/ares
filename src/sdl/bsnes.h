class bJoypad {
public:
bool up, down, left, right;
bool a, b, x, y;
bool l, r, select, start;
  bJoypad();
};

class bSNES : public SNES {
private:
uint32 run_status;
bool update_frame;
bJoypad joypad1, joypad2;

public:
enum { STOP = 0, RUN };
  void   set_status(uint32 new_status);
  uint32 get_status();
  void   snes_run();
  void   render_frame();

//input functions
  void poll_input();
  bool get_input_status(uint8 device, uint8 button);

  void  notify(uint32 message, uint32 param1 = 0, uint32 param2 = 0);
  bSNES();
};

bSNES *bsnes;
