namespace config {

struct System {
  static Setting regulate_speed;
  static Setting speed_slowest, speed_slow, speed_normal, speed_fast, speed_fastest;
} system;
Setting System::regulate_speed(&config_file, "system.regulate_speed", "Regulate speed to 60hz (NTSC) / 50hz (PAL)", true, Setting::TRUE_FALSE);
Setting System::speed_slowest (&config_file, "system.speed_slowest",  "Slowest speed setting (in hz)", 16000, Setting::DEC);
Setting System::speed_slow    (&config_file, "system.speed_slow",     "Slow speed setting",            24000, Setting::DEC);
Setting System::speed_normal  (&config_file, "system.speed_normal",   "Normal speed setting",          32000, Setting::DEC);
Setting System::speed_fast    (&config_file, "system.speed_fast",     "Fast speed setting",            48000, Setting::DEC);
Setting System::speed_fastest (&config_file, "system.speed_fastest",  "Fastest speed setting",         64000, Setting::DEC);

struct Video {
  static Setting renderer;
  static Setting profile, fullscreen;
  static Setting profile_0, profile_1, profile_2, profile_3;
  static Setting profile_4, profile_5, profile_6, profile_7;

  static Setting use_vram, triple_buffering;
  static Setting pscanline_intensity, iscanline_intensity;
} video;
Setting Video::renderer(&config_file, "video.renderer", "Video renderer\n"
  "\"dd\" (DirectDraw7 -- faster, less features)\n"
  "\"d3d\" (Direct3D9 -- slower, more features)", "d3d");
Setting Video::profile(&config_file, "video.profile", "", 2, Setting::DEC);
Setting Video::fullscreen(0, "video.fullscreen", "", false, Setting::TRUE_FALSE);

/* software_filter
 * hardware_filter
 * video_standard
 * multiplier
 * correct_aspect_ratio
 * enable_scanlines
 * manual_render_size
 * render_width
 * render_height
 * fullscreen
 * resolution_width
 * resolution_height
 * refresh_rate
 * triple_buffering
 */
Setting Video::profile_0(&config_file, "video.profile_0", "Video profile 0-7 configuration\n"
  "Please use bsnes GUI configuration editor to modify video profile settings\n"
  "Format: software_filter;hardware_filter;video_standard;multiplier-1;correct_aspect_ratio;\n"
  "        enable_scanlines;manual_render_size;render_width;render_height;\n"
  "        resolution_width;resolution_height;refresh_rate;triple_buffering"
  "",                                                         "0;0;0;0;false;false;false;256;224;0;0;0;false");
Setting Video::profile_1(&config_file, "video.profile_1", "", "0;0;0;1;false;false;false;512;448;0;0;0;false");
Setting Video::profile_2(&config_file, "video.profile_2", "", "0;1;0;1;true;false;false;597;448;0;0;0;false");
Setting Video::profile_3(&config_file, "video.profile_3", "", "0;1;0;2;true;false;false;896;672;0;0;0;false");
Setting Video::profile_4(&config_file, "video.profile_4", "", "0;1;0;3;true;false;false;1195;896;0;0;0;false");
Setting Video::profile_5(&config_file, "video.profile_5", "", "0;0;0;0;false;false;false;256;224;0;0;0;false");
Setting Video::profile_6(&config_file, "video.profile_6", "", "0;0;0;0;false;false;false;256;224;0;0;0;false");
Setting Video::profile_7(&config_file, "video.profile_7", "", "0;0;0;0;false;false;false;256;224;0;0;0;false");

Setting Video::use_vram(&config_file, "video.use_vram", "Use Video RAM instead of System RAM", true, Setting::TRUE_FALSE);
Setting Video::triple_buffering(&config_file, "video.triple_buffering", "Use triple buffering", false, Setting::TRUE_FALSE);
Setting Video::pscanline_intensity(&config_file, "video.pscanline_intensity",
  "Progressive scanline intensity\n"
  "Value is percentage of intensity from 0 to 100", 30, Setting::DEC);
Setting Video::iscanline_intensity(&config_file, "video.iscanline_intensity",
  "Interlace scanline intensity", 50, Setting::DEC);

struct Input {
  static Setting axis_resistance;
  struct Joypad1 {
    static Setting allow_invalid_input;
    static Setting up, down, left, right, a, b, x, y, l, r, select, start;
    static Setting map;
  } joypad1;
  struct Joypad2 {
    static Setting allow_invalid_input;
    static Setting up, down, left, right, a, b, x, y, l, r, select, start;
    static Setting map;
  } joypad2;
} input;

Setting Input::axis_resistance(&config_file, "input.axis_resistance",
  "Axis resistance for all analog joypads\n"
  "Affects responsiveness of analog stick movement by specifying what percentage\n"
  "in any given direction the axis must be pressed to trigger a button press.\n"
  "In other words, this determines how hard you have to press the analog stick to\n"
  "simulate pressing e.g. left or right on a digital joypad.\n"
  "Value is a percentage, from 0 (axis will trigger with virtually any axis movement)\n"
  "up to 100 (axis must be pressed fully to given corner).\n"
  "Value affects all four directions of the axis equally.\n"
  "Note: Values below 10 or above 90 are not recommended and may not work at all.",
  75, Setting::DEC);

Setting Input::Joypad1::allow_invalid_input(&config_file, "input.joypad1.allow_invalid_input",
  "Allow up+down and left+right key combinations for joypad 1 (not recommended)", false, Setting::TRUE_FALSE);

Setting Input::Joypad1::up    (0, "input.joypad1.up",     "", 0, Setting::HEX);
Setting Input::Joypad1::down  (0, "input.joypad1.down",   "", 0, Setting::HEX);
Setting Input::Joypad1::left  (0, "input.joypad1.left",   "", 0, Setting::HEX);
Setting Input::Joypad1::right (0, "input.joypad1.right",  "", 0, Setting::HEX);
Setting Input::Joypad1::a     (0, "input.joypad1.a",      "", 0, Setting::HEX);
Setting Input::Joypad1::b     (0, "input.joypad1.b",      "", 0, Setting::HEX);
Setting Input::Joypad1::x     (0, "input.joypad1.x",      "", 0, Setting::HEX);
Setting Input::Joypad1::y     (0, "input.joypad1.y",      "", 0, Setting::HEX);
Setting Input::Joypad1::l     (0, "input.joypad1.l",      "", 0, Setting::HEX);
Setting Input::Joypad1::r     (0, "input.joypad1.r",      "", 0, Setting::HEX);
Setting Input::Joypad1::select(0, "input.joypad1.select", "", 0, Setting::HEX);
Setting Input::Joypad1::start (0, "input.joypad1.start",  "", 0, Setting::HEX);

Setting Input::Joypad1::map(&config_file, "input.joypad1.map", "Joypad 1 button map\n"
  "Format: Up; Down; Left; Right; A; B; X; Y; L; R; Select; Start",
  "up | joypad0.up; "       "down | joypad0.down; "       "left | joypad0.left; "     "right | joypad0.right; "
   "x | joypad0.button3; "     "z | joypad0.button2; "       "s | joypad0.button1; "      "a | joypad0.button0; "
   "d | joypad0.button6; "     "c | joypad0.button7; "  "rshift | joypad0.button4; "  "enter | joypad0.button5");

//

Setting Input::Joypad2::allow_invalid_input(&config_file, "input.joypad2.allow_invalid_input",
  "Allow up+down and left+right key combinations for joypad 2 (not recommended)", false, Setting::TRUE_FALSE);

Setting Input::Joypad2::up    (0, "input.joypad2.up",     "", 0, Setting::HEX);
Setting Input::Joypad2::down  (0, "input.joypad2.down",   "", 0, Setting::HEX);
Setting Input::Joypad2::left  (0, "input.joypad2.left",   "", 0, Setting::HEX);
Setting Input::Joypad2::right (0, "input.joypad2.right",  "", 0, Setting::HEX);
Setting Input::Joypad2::a     (0, "input.joypad2.a",      "", 0, Setting::HEX);
Setting Input::Joypad2::b     (0, "input.joypad2.b",      "", 0, Setting::HEX);
Setting Input::Joypad2::x     (0, "input.joypad2.x",      "", 0, Setting::HEX);
Setting Input::Joypad2::y     (0, "input.joypad2.y",      "", 0, Setting::HEX);
Setting Input::Joypad2::l     (0, "input.joypad2.l",      "", 0, Setting::HEX);
Setting Input::Joypad2::r     (0, "input.joypad2.r",      "", 0, Setting::HEX);
Setting Input::Joypad2::select(0, "input.joypad2.select", "", 0, Setting::HEX);
Setting Input::Joypad2::start (0, "input.joypad2.start",  "", 0, Setting::HEX);

Setting Input::Joypad2::map(&config_file, "input.joypad2.map", "Joypad 2 button map\n"
  "Format: Up; Down; Left; Right; A; B; X; Y; L; R; Select; Start",
  "t | joypad1.up; "       "g | joypad1.down; "            "f | joypad1.left; "            "h | joypad1.right; "
  "k | joypad1.button3; "  "j | joypad1.button2; "         "i | joypad1.button1; "         "u | joypad1.button0; "
  "o | joypad1.button6; "  "l | joypad1.button7; "  "lbracket | joypad1.button4; "  "rbracket | joypad1.button5");

};
