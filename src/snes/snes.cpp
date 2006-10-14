#include "../base.h"

SNES *snes;

#include "tracer/tracer.cpp"

#include "video/filter.cpp"
#include "video/video.cpp"
#include "audio/audio.cpp"
#include "input/input.cpp"

void SNES::run() {
  if(sync.counter <= 0) {
    r_cpu->run();
  uint32 clocks = r_cpu->clocks_executed();
    sync.counter += sync.apu_multbl[clocks];
  } else {
    r_smp->run();
  uint32 clocks = r_smp->clocks_executed();
    sync.counter -= sync.cpu_multbl[clocks];
  //1024000(SPC700) / 32000(DSP) = 32spc/dsp ticks
  //24576000(Sound clock crystal) / 32000(DSP) = 768crystal/dsp ticks
    sync.dsp_counter += clocks;
    while(sync.dsp_counter >= 768) {
      sync.dsp_counter -= 768;
      audio_update(r_dsp->run());
    }
  }
}

void SNES::runtoframe() {
  while(r_ppu->status.frame_executed == false) {
    SNES::run();
  }
  r_ppu->status.frame_executed = false;
}

void SNES::init() {
  srtc = new SRTC();
  sdd1 = new SDD1();
  c4   = new C4();
  dsp1 = new DSP1();
  dsp2 = new DSP2();
  obc1 = new OBC1();

  srtc->init();
  sdd1->init();
  c4->init();
  dsp1->init();
  dsp2->init();
  obc1->init();

  video_init();
  audio_init();
  input_init();
}

void SNES::term() {
  audio_term();
}

void SNES::power() {
  r_cpu->power();
  r_smp->power();
  r_dsp->power();
  r_ppu->power();
  r_mem->power();

  if(cartridge.info.srtc)srtc->power();
  if(cartridge.info.sdd1)sdd1->power();
  if(cartridge.info.c4)  c4->power();
  if(cartridge.info.dsp1)dsp1->power();
  if(cartridge.info.dsp2)dsp2->power();
  if(cartridge.info.obc1)obc1->power();

  r_mem->flush_mmio_mappers();
  for(int i = 0x2100; i <= 0x213f; i++)r_mem->set_mmio_mapper(i, r_ppu);
  for(int i = 0x2140; i <= 0x217f; i++)r_mem->set_mmio_mapper(i, r_cpu);
  for(int i = 0x2180; i <= 0x2183; i++)r_mem->set_mmio_mapper(i, r_cpu);
  for(int i = 0x4016; i <= 0x4017; i++)r_mem->set_mmio_mapper(i, r_cpu);
  for(int i = 0x4200; i <= 0x421f; i++)r_mem->set_mmio_mapper(i, r_cpu);
  for(int i = 0x4300; i <= 0x437f; i++)r_mem->set_mmio_mapper(i, r_cpu);

  if(cartridge.info.srtc)srtc->enable();
  if(cartridge.info.sdd1)sdd1->enable();
  if(cartridge.info.c4)  c4->enable();
  if(cartridge.info.dsp1)dsp1->enable();
  if(cartridge.info.dsp2)dsp2->enable();
  if(cartridge.info.obc1)obc1->enable();

  video_update();
}

void SNES::reset() {
  sync.counter     = -sync.cpu_multbl[32];
  sync.dsp_counter = 0;

  r_cpu->reset();
  r_smp->reset();
  r_dsp->reset();
  r_ppu->reset();
  r_mem->reset();

  if(cartridge.info.srtc)srtc->reset();
  if(cartridge.info.sdd1)sdd1->reset();
  if(cartridge.info.c4)  c4->reset();
  if(cartridge.info.dsp1)dsp1->reset();
  if(cartridge.info.dsp2)dsp2->reset();
  if(cartridge.info.obc1)obc1->reset();

  video_update();
}

void SNES::scanline() {
  video_scanline();

//draw before the start of the next frame, to make the
//video output seem more responsive to controller input
  if(r_cpu->vcounter() == 241) {
    video_update();
  }
}

void SNES::frame() {}

/*****
 * PAL/NTSC
 *****/

void SNES::set_region(uint8 new_region) {
  if(new_region == NTSC) {
    snes_region = NTSC;
  } else if(new_region == PAL) {
    snes_region = PAL;
  } else {
    alert("Unsupported region : %0.2x", new_region);
  }

  update_timing();
}

uint8 SNES::region() { return snes_region; }

/*****
 * Timing
 *
 * Note: Stock PAL CPU speed is currently unknown.
 * It is thought to be 21281370hz, but this causes
 * sound errors in certain games. Therefore, the PAL
 * CPU is currently clocked 40000hz slower, at
 * 21241370hz.
 *****/

void SNES::update_timing() {
  sync.counter     = 0;
  sync.dsp_counter = 0;

  sync.cpu_freq = (snes_region == NTSC) ? config::cpu.ntsc_clock_rate : config::cpu.pal_clock_rate;
  sync.apu_freq = (snes_region == NTSC) ? config::smp.ntsc_clock_rate : config::smp.pal_clock_rate;

  for(int64 i = 0; i < 1024; i++) {
    sync.cpu_multbl[i] = i * sync.cpu_freq;
    sync.apu_multbl[i] = i * sync.apu_freq;
  }
}

SNES::SNES() {
  snes_region = NTSC;
  update_timing();
}
