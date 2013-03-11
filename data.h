#ifndef _DATA_H
#define _DATA_H

#include <X11/X.h>
#include <X11/Xlib.h>

#define FONT_HEIGHT(f) ((f)->max_bounds.ascent + (f)->max_bounds.descent)

#define STATE_UNMAPPED 0
#define STATE_MAPPED   1

typedef struct a_window a_window;
typedef struct screen_info screen_info;

struct a_window
{
  screen_info *scr;
  Window w;
  char *name;
  int state;
  int last_access;		
  a_window *next, *prev;  
};

struct screen_info
{
  GC normal_gc;
  GC bold_gc;
  XFontStruct *font;
  XWindowAttributes root_attr;
  Window root, bar_window, key_window;
  int bar_is_raised;
  int screen_num;
  Colormap def_cmap;
};

extern  a_window *a_window_head, *a_window_tail;
extern  a_window *a_current_window;
extern screen_info *screens;
extern int num_screens;

extern Display *dpy;
extern Atom a_restart;

extern int exit_signal;
#endif
