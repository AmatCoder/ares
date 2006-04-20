void SNES::set_video_format(uint32 filter, uint32 pixel_format) {
//only make changes at the start of a new frame
  video_format.filter       = filter;
  video_format.pixel_format = pixel_format;
  video_format.modified     = true;
}

/*****
 * Internal function called at the start of each frame.
 * Use SNES::set_video_format to modify these options.
 *****/
void SNES::update_video_format() {
  if(video_format.modified == false)return;
  video_format.modified = false;

  video.filter = video_format.filter;
  SafeDelete(video_filter);
  switch(video.filter) {
  default:
  case VIDEOFILTER_DIRECT:  video_filter = new DirectVideoFilter();  break;
  case VIDEOFILTER_NTSC:    video_filter = new NtscVideoFilter();    break;
  case VIDEOFILTER_HQ2X:    video_filter = new HQ2xVideoFilter();    break;
  case VIDEOFILTER_SCALE2X: video_filter = new Scale2xVideoFilter(); break;
  }

  video.pixel_format = video_format.pixel_format;
  update_color_lookup_table();
}

void SNES::get_video_info(video_info *info) {
  info->filter       = video.filter;
  info->pixel_format = video.pixel_format;
  info->width        = video.width;
  info->height       = video.height;
}

void SNES::video_update() {
  if(r_ppu->renderer_enabled()) {
    update_video_format();

    video.ppu_data = (uint16*)r_ppu->output + (int(r_cpu->overscan()) << 13) + 1024;
//  video_normalize();

    video.raster_width  = (video.frame_hires     == false) ? 256 : 512;
    video.raster_height = (video.frame_interlace == false) ? 224 : 448;

    video.data = (uint16*)video_lock(video.pitch);
    if(video.data) {
      video_filter->run(color_lookup_table, video.ppu_data,
                        video.raster_width, video.raster_height,
                        video.raster_height <= 240 ? 2048 : 1024,
                        video.data, 512, 480, video.pitch,
                        512, 480, video.width, video.height,
                        video.raster_height <= 240 ? (pline_width + 1) : (iline_width + 2));
      video_unlock();
    }
  }

  video_run();

  video.frame_hires     = false;
  video.frame_interlace = false;
}

void SNES::video_scanline() {
int y = r_cpu->vcounter();
int o = int(r_cpu->overscan()) << 3;
  if(y <= (0 + o) || y >= (225 + o))return;
  y -= o;

PPU::scanline_info si;
  r_ppu->get_scanline_info(&si);

  pline_width[y] = iline_width[y * 2 + int(r_cpu->interlace_field())] =
    (si.hires == false) ? 256 : 512;
  video.frame_hires      |= si.hires;
  video.frame_interlace  |= si.interlace;
}

void SNES::video_init() {
  for(int i = 0; i < 240; i++)pline_width[i] = 256;
  for(int i = 0; i < 480; i++)iline_width[i] = 256;
  video.frame_hires     = false;
  video.frame_interlace = false;

  video.raster_data = (uint16*)malloc(512 * 480 * sizeof(uint16));
  memset(video.raster_data, 0, 512 * 480 * sizeof(uint16));
  video_filter = 0;
  set_video_format(VIDEOFILTER_DIRECT, PIXELFORMAT_RGB565);
  update_video_format();
}
