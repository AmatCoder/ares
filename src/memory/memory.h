class Memory {
public:
enum { WRAP_NONE = 0, WRAP_BANK = 1, WRAP_PAGE = 2 };
  virtual uint8  read      (uint32 addr)              = 0;
  virtual void   write     (uint32 addr, uint8  data) = 0;
  virtual uint16 read_word (uint32 addr,              uint wrap = WRAP_NONE);
  virtual void   write_word(uint32 addr, uint16 data, uint wrap = WRAP_NONE);
  virtual uint32 read_long (uint32 addr,              uint wrap = WRAP_NONE);
  virtual void   write_long(uint32 addr, uint32 data, uint wrap = WRAP_NONE);
};

class MMIO {
public:
  virtual uint8 mmio_read (uint16 addr);
  virtual void  mmio_write(uint16 addr, uint8 data);
};

class MemBus : public Memory {
public:
MMIO *mmio[0x4000]; //mapped to $[00-3f|80-bf]:[2000-5fff]
bool  fastROM;
  void flush_mmio_mappers();
  bool set_mmio_mapper(uint16 addr, MMIO *mapper);

private:
//0x1000000 / 512 = 32768
//512 = 0x200, smallest block of a different-speed memory range
//ex. $4000-$41ff = 512
uint8 *speed_table,
       speed_table_slowrom[32768],
       speed_table_fastrom[32768];
  inline uint8 calc_speed(uint32 addr, bool fast);
public:
  inline uint8 speed(uint32 addr) { return speed_table[addr >> 9]; }
  void set_speed(bool fast);

  virtual void load_cart() = 0;
  virtual void unload_cart() = 0;
  virtual bool cart_loaded() = 0;

//set to true to block writes to cartridge ROM data,
//set to false to allow writes. ROM cannot be written
//on true SNES, however the debugger should be able to
//set this to false, write to the cartridge, and then
//set this back to true.
//
//needs to be set to true on power.
bool cart_write_protect_enabled;
  virtual void cart_write_protect(bool status) { cart_write_protect_enabled = status; }

  virtual void power() = 0;
  virtual void reset() = 0;

  MemBus();
  virtual ~MemBus() {}
};
