enum {
  HARDWARE_FILTER_PIXEL,
  HARDWARE_FILTER_BILINEAR,
};

struct VideoSettings {
  uint software_filter;
  uint hardware_filter;
  uint video_standard;
  uint multiplier;
  bool correct_aspect_ratio;
  bool enable_scanlines;
  bool manual_render_size;
  uint render_width;
  uint render_height;
  uint resolution_width;
  uint resolution_height;
  uint refresh_rate;
  bool triple_buffering;
} video_settings[8];

void load_video_settings(uint profile);
void save_video_settings(uint profile);

class Video {
public:
SNES::video_info vi;

struct {
  uint render_width;
  uint render_height;

  uint hardware_filter;
  bool triple_buffering;
  bool enable_scanlines;

  uint resolution_width;
  uint resolution_height;
  uint refresh_rate;

  uint rx, ry, rw, rh;
} settings;
  void update_video_settings();

  virtual uint16 *lock(uint &pitch) = 0;
  virtual void    unlock() = 0;

  virtual uint    screen_width()  = 0;
  virtual uint    screen_height() = 0;

  virtual bool    update_video_profile() = 0;
  virtual void    clear_video() = 0;

  virtual void    pause_enable() {}
  virtual void    pause_disable() {}
  virtual void    update_hardware_filter() {}
  virtual void    update_scanlines() {}

  virtual bool    capture_screenshot() { return false; }

  virtual void    redraw() = 0;
  virtual void    update() = 0;
  virtual void    init() = 0;
  virtual void    term() = 0;
} *uiVideo;
