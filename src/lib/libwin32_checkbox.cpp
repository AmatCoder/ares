bool Checkbox::Checked() { return SendMessage(hwnd, BM_GETCHECK, 0, 0); }
void Checkbox::Check(bool state) { SendMessage(hwnd, BM_SETCHECK, (WPARAM)state, 0); }
void Checkbox::Check() { Check(true); }
void Checkbox::Uncheck() { Check(false); }
void Checkbox::ToggleCheck() { Check(!Checked()); }

bool Checkbox::Create(Window *parent_window, const char *style, int x, int y, int width, int height, const char *text) {
  if(!parent_window)return false;

  parent = parent_window;
  id     = CONTROLID_INDEX + parent->control_count;
  type   = CHECKBOX;
  state.ws     = WS_CHILD;
  state.es     = 0;
  state.x      = x;
  state.y      = y;
  state.width  = width;
  state.height = height;

string part;
  ParseStyleParam(style, part);
  for(int i = 0; i < count(part); i++) {
    if(strmatch(part[i], "visible"))state.ws |= WS_VISIBLE;
    if(strmatch(part[i], "disabled"))state.ws |= WS_DISABLED;

    if(strmatch(part[i], "left"))state.ws |= BS_LEFT;
    if(strmatch(part[i], "center"))state.ws |= BS_CENTER;
    if(strmatch(part[i], "right"))state.ws |= BS_RIGHT;
    if(strmatch(part[i], "auto"))state.ws |= BS_AUTOCHECKBOX;
  }
  if(!(state.ws & BS_AUTOCHECKBOX))state.ws |= BS_CHECKBOX;

  hwnd = CreateWindowEx(state.es, "BUTTON", text, state.ws,
    state.x, state.y, state.width, state.height,
    parent->hwnd, (HMENU)id, GetModuleHandle(0), 0);
  if(!hwnd)return false;

  PostCreate();
  return true;
}
