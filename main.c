/* AntiWM
 * 2008-2009, Nolan Liebert
 * See LICENSE and README. */

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xproto.h>

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include "antiwm.h"

static void init_screen (screen_info *s, int screen_num);
Atom wm_state;
Atom wm_change_state;
Atom wm_protocols;
Atom wm_delete;
Atom wm_take_focus;
Atom wm_colormaps;
Atom a_restart;

screen_info *screens;
int num_screens;
Display *dpy;
int exit_signal = 0;		
static XFontStruct *font;

char **myargv;

void
sighandler ()
{
  fprintf (stderr, "antiwm: sighander failure\n"); 
  clean_up ();
  exit (EXIT_FAILURE);
}

void
hup_handler ()
{
  fprintf (stderr, "antiwm: restarting\n"); 
  clean_up ();
  execvp(myargv[0], myargv);
}

void
alrm_handler ()
{
  int i;
#ifdef DEBUG
  printf ("alarm recieved\n");
#endif
  for (i=0; i<num_screens; i++)
    {
      hide_bar (&screens[i]);
    }
  XFlush (dpy);
}

int
handler (Display *d, XErrorEvent *e)
{
  char msg[100];
  if (e->request_code == X_ChangeWindowAttributes && e->error_code == BadAccess) {
    fprintf(stderr, "antiwm: another window manager is running\n");
    exit(EXIT_FAILURE);
  }  
  XGetErrorText (d, e->error_code, msg, sizeof (msg));
  fprintf (stderr, "antiwm: %s!\n", msg);
  exit (EXIT_FAILURE);
}

int
main (int argc, char *argv[])
{
  int i;
  myargv = argv;
  if (!(dpy = XOpenDisplay (NULL)))
    {
      fprintf (stderr, "cannot open display\n");
      return EXIT_FAILURE;
    }
  init_window_list ();
  font = XLoadQueryFont (dpy, FONT_NAME);
  if (font == NULL)
    {
      fprintf (stderr, "antiwm: cannot load font %s.\n", FONT_NAME);
      exit (EXIT_FAILURE);
    }
  num_screens = ScreenCount (dpy);
  if ((screens = (screen_info *)malloc (sizeof (screen_info) * num_screens)) == NULL)
    {
      fprintf (stderr, "antiwm:main.c:Out of memory!\n");
      exit (EXIT_FAILURE);
    }  
  printf ("%d screens.\n", num_screens);
  /* initialize screens */
  for (i=0; i<num_screens; i++)
    {
      init_screen (&screens[i], i);
    }
  /* setup signal handlers */  
  if (signal (SIGALRM, alrm_handler) == SIG_IGN) signal (SIGALRM, SIG_IGN);
  if (signal (SIGTERM, sighandler) == SIG_IGN) signal (SIGTERM, SIG_IGN);
  if (signal (SIGINT, sighandler) == SIG_IGN) signal (SIGINT, SIG_IGN);
  if (signal (SIGHUP, hup_handler) == SIG_IGN) 
    {
      printf ("Ignoring HUP.\n");
      signal (SIGHUP, SIG_IGN);
    }
  /* set atoms */
  wm_state = XInternAtom(dpy, "WM_STATE", False);
  wm_change_state = XInternAtom(dpy, "WM_CHANGE_STATE", False);
  wm_protocols = XInternAtom(dpy, "WM_PROTOCOLS", False);
  wm_delete = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
  wm_take_focus = XInternAtom(dpy, "WM_TAKE_FOCUS", False);
  wm_colormaps = XInternAtom(dpy, "WM_COLORMAP_WINDOWS", False);
  a_restart = XInternAtom (dpy, "A_RESTART", False);

  XSync (dpy, False);

  /* set initial window as active */
  a_current_window = a_window_head;
  set_active_window (a_current_window);
  
  handle_events ();
  return EXIT_SUCCESS;
}

static void
init_screen (screen_info *s, int screen_num)
{
  XColor fg_color, bg_color, bold_color, junk;
  XGCValues gv;
  s->screen_num = screen_num;
  s->root = RootWindow (dpy, screen_num);
  s->def_cmap = DefaultColormap (dpy, screen_num);
  s->font = font;
  XGetWindowAttributes (dpy, s->root, &s->root_attr);
  /* retrieve bar colors */
  if (!XAllocNamedColor (dpy, s->def_cmap, BAR_FG_COLOR, &fg_color, &junk))
    {
      fprintf (stderr, "Unknown color '%s'\n", BAR_FG_COLOR);
    }
  if (!XAllocNamedColor (dpy, s->def_cmap, BAR_BG_COLOR, &bg_color, &junk))
    {
      fprintf (stderr, "Unknown color '%s'\n", BAR_BG_COLOR);
    }

  if (!XAllocNamedColor (dpy, s->def_cmap, BAR_BOLD_COLOR, &bold_color, &junk))
    {
      fprintf (stderr, "Unknown color '%s'\n", BAR_BOLD_COLOR);
    }
  gv.foreground = fg_color.pixel;
  gv.background = bg_color.pixel;
  gv.function = GXcopy;
  gv.line_width = 1;
  gv.subwindow_mode = IncludeInferiors;
  gv.font = font->fid;
  s->normal_gc = XCreateGC(dpy, s->root, 
			   GCForeground | GCBackground | GCFunction 
			   | GCLineWidth | GCSubwindowMode | GCFont, 
			   &gv);
  gv.foreground = bold_color.pixel;
  s->bold_gc = XCreateGC(dpy, s->root, 
			 GCForeground | GCBackground | GCFunction 
			 | GCLineWidth | GCSubwindowMode | GCFont, 
			 &gv);
  XSelectInput(dpy, s->root,
               PropertyChangeMask | ColormapChangeMask
               | SubstructureRedirectMask | KeyPressMask 
               | SubstructureNotifyMask );
  XSync (dpy, 0);
  /* create bar window */
  s->bar_is_raised = 0;
  s->bar_window = XCreateSimpleWindow (dpy, s->root, 0, 0,
				       1, 1, 1, fg_color.pixel, bg_color.pixel);
  /* setup window to recieve keystrokes */
  s->key_window = XCreateSimpleWindow (dpy, s->root, 0, 0, 1, 1, 0, WhitePixel (dpy, 0), BlackPixel (dpy, 0));
  XSelectInput (dpy, s->bar_window, StructureNotifyMask);
  XMapWindow (dpy, s->key_window);
  grab_keys (s);
  scanwins (s);
}

void
clean_up ()
{
  XSetInputFocus (dpy, PointerRoot, RevertToPointerRoot, CurrentTime);
  XCloseDisplay (dpy);
}

/* given a root window, return screen_info struct */
screen_info *
find_screen (Window w)
{
  int i;
  for (i=0; i<num_screens; i++)
    if (screens[i].root == w) return &screens[i];
  return NULL;
}
