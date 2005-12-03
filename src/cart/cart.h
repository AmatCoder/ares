class Cartridge {
public:
bool cart_loaded;
char rom_fn[4096], sram_fn[4096];

uint8 *base_rom, *rom, *sram;
uint32 rom_size;

enum {
//header fields
  CART_NAME = 0x00,
  MAPPER    = 0x15,
  ROM_TYPE  = 0x16,
  ROM_SIZE  = 0x17,
  SRAM_SIZE = 0x18,
  REGION    = 0x19,
  LICENSE   = 0x1a,
  VERSION   = 0x1b,
  ICKSUM    = 0x1c,
  CKSUM     = 0x1e,

//regions
  NTSC = 0,
  PAL  = 1,

//memory mappers
  LOROM   = 0x20,
  HIROM   = 0x21,
  EXLOROM = 0x22,
  EXHIROM = 0x25,
};

struct {
  uint32 header_index;

  char   name[32];
  uint32 rom_size;
  uint32 sram_size;
  bool   region;

  uint32 mapper;
  bool   srtc;
  bool   sdd1;
  bool   c4;
} cart;

  void load_rom(Reader *rf);
  void load_sram();
  void save_sram();
  void find_header();
  void read_header();
  bool load(const char *fn);
  bool unload();

  Cartridge();
  ~Cartridge();
};

extern Cartridge cartridge;
