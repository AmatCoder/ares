void load_video_settings(uint profile) {
string line, part;
  if(profile > 7)profile = 0;

  switch(profile) {
  case 0: strcpy(line, config::video.profile_0.sget()); break;
  case 1: strcpy(line, config::video.profile_1.sget()); break;
  case 2: strcpy(line, config::video.profile_2.sget()); break;
  case 3: strcpy(line, config::video.profile_3.sget()); break;
  case 4: strcpy(line, config::video.profile_4.sget()); break;
  case 5: strcpy(line, config::video.profile_5.sget()); break;
  case 6: strcpy(line, config::video.profile_6.sget()); break;
  case 7: strcpy(line, config::video.profile_7.sget()); break;
  }

  split(part, ";", line);
VideoSettings *v = &video_settings[profile];
int i = 0;
  v->software_filter      = strdec(part[i++]);
  v->hardware_filter      = strdec(part[i++]);
  v->video_standard       = strdec(part[i++]);
  v->multiplier           = strdec(part[i++]);
  v->correct_aspect_ratio = strmatch(part[i++], "true");
  v->enable_scanlines     = strmatch(part[i++], "true");
  v->manual_render_size   = strmatch(part[i++], "true");
  v->render_width         = strdec(part[i++]);
  v->render_height        = strdec(part[i++]);
  v->fullscreen           = strmatch(part[i++], "true");
  v->triple_buffering     = strmatch(part[i++], "true");
  v->resolution_width     = strdec(part[i++]);
  v->resolution_height    = strdec(part[i++]);
  v->refresh_rate         = strdec(part[i++]);

  if(v->render_width  < 256)v->render_width  = 256;
  if(v->render_height < 224)v->render_height = 224;
}

void save_video_settings(uint profile) {
string line;
char   part[64];
  if(profile > 7)profile = 0;

VideoSettings *v = &video_settings[profile];
  strcpy(line, "");
  sprintf(part, "%d", v->software_filter);                         strcat(line, part); strcat(line, ";");
  sprintf(part, "%d", v->hardware_filter);                         strcat(line, part); strcat(line, ";");
  sprintf(part, "%d", v->video_standard);                          strcat(line, part); strcat(line, ";");
  sprintf(part, "%d", v->multiplier);                              strcat(line, part); strcat(line, ";");
  sprintf(part, "%s", v->correct_aspect_ratio ? "true" : "false"); strcat(line, part); strcat(line, ";");
  sprintf(part, "%s", v->enable_scanlines     ? "true" : "false"); strcat(line, part); strcat(line, ";");
  sprintf(part, "%s", v->manual_render_size   ? "true" : "false"); strcat(line, part); strcat(line, ";");
  sprintf(part, "%d", v->render_width);                            strcat(line, part); strcat(line, ";");
  sprintf(part, "%d", v->render_height);                           strcat(line, part); strcat(line, ";");
  sprintf(part, "%s", v->fullscreen           ? "true" : "false"); strcat(line, part); strcat(line, ";");
  sprintf(part, "%s", v->triple_buffering     ? "true" : "false"); strcat(line, part); strcat(line, ";");
  sprintf(part, "%d", v->resolution_width);                        strcat(line, part); strcat(line, ";");
  sprintf(part, "%d", v->resolution_height);                       strcat(line, part); strcat(line, ";");
  sprintf(part, "%d", v->refresh_rate);                            strcat(line, part);

  switch(profile) {
  case 0: config::video.profile_0.sset(strptr(line)); break;
  case 1: config::video.profile_1.sset(strptr(line)); break;
  case 2: config::video.profile_2.sset(strptr(line)); break;
  case 3: config::video.profile_3.sset(strptr(line)); break;
  case 4: config::video.profile_4.sset(strptr(line)); break;
  case 5: config::video.profile_5.sset(strptr(line)); break;
  case 6: config::video.profile_6.sset(strptr(line)); break;
  case 7: config::video.profile_7.sset(strptr(line)); break;
  }
}

void Video::update_video_settings() {
uint profile = uint(config::video.profile);
  if(profile > 7)profile = 0;

  load_video_settings(profile);
VideoSettings *v = &video_settings[profile];
  snes->set_video_format(v->software_filter, v->video_standard, SNES::PIXELFORMAT_RGB565);

  if(v->manual_render_size == true) {
    settings.render_width  = v->render_width;
    settings.render_height = v->render_height;
  } else {
    settings.render_width  = 256;
    settings.render_height = (v->video_standard == SNES::VIDEOSTANDARD_NTSC) ? 224 : 239;

    settings.render_width  *= (v->multiplier + 1);
    settings.render_height *= (v->multiplier + 1);

    if(v->correct_aspect_ratio == true) {
      settings.render_width = (uint)( (double)settings.render_height / 3.0 * 4.0 );
    }
  }

  settings.fullscreen        = v->fullscreen;
  settings.triple_buffering  = v->triple_buffering;

  settings.hardware_filter   = v->hardware_filter;
  settings.enable_scanlines  = v->enable_scanlines;

  if(v->fullscreen == true) {
    settings.resolution_width  = (v->resolution_width)  ? v->resolution_width  : screen_width();
    settings.resolution_height = (v->resolution_height) ? v->resolution_height : screen_height();
    settings.refresh_rate      = v->refresh_rate;
  } else {
    settings.resolution_width  = settings.render_width;
    settings.resolution_height = settings.render_height;
    settings.refresh_rate      = 0;
  }

  if(settings.render_width  <= settings.resolution_width) {
    settings.rx = (settings.resolution_width  - settings.render_width)  >> 1;
    settings.rw = settings.render_width;
  } else {
    settings.rx = 0;
    settings.rw = settings.resolution_width;
  }

  if(settings.render_height <= settings.resolution_height) {
    settings.ry = (settings.resolution_height - settings.render_height) >> 1;
    settings.rh = settings.render_height;
  } else {
    settings.ry = 0;
    settings.rh = settings.resolution_height;
  }
}
