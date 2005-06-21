HFONT hFont, hMonofont;

#include "dd_renderer.cpp"
#include "ui_window.cpp"
#include "ui_main.cpp"
#include "ui_console.cpp"
#include "ui_bp.cpp"
#include "ui_memory.cpp"

void CreateFonts() {
HDC hdc;
long height;
  hdc = GetDC(0);
  height = -MulDiv(8, GetDeviceCaps(hdc, LOGPIXELSY), 72);
  ReleaseDC(0, hdc);
  hFont = CreateFont(height, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Tahoma");

  hdc = GetDC(0);
  height = -MulDiv(8, GetDeviceCaps(hdc, LOGPIXELSY), 72);
  ReleaseDC(0, hdc);
  hMonofont = CreateFont(height, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Courier New");
}

void CreateWindows() {
  CreateWindowMain();
  CreateWindowConsole();
  CreateWindowBreakpoint();
  CreateWindowMemory();
}

void init_ui0() {
  dd_renderer = new DDRenderer();
  w_main      = new MainWindow();
  w_console   = new Console();
  w_bp        = new BreakpointEditor();
  w_memory    = new MemoryEditor();
}

void init_ui1() {
  CreateFonts();
  CreateWindows();
  SetFocus(w_main->hwnd);
  dd_renderer->set_window(w_main->hwnd);
  dd_renderer->to_windowed();
  bsnes->debugger_deactivate();
}
