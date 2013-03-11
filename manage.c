/* AntiWM
 * 2008-2009, Nolan Liebert
 * See LICENSE and README. */

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/keysymdef.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "antiwm.h"

extern Atom wm_state;

void
grab_keys (screen_info *s)
{
  int i;

  for (i='0'; i<='9'; i++)
    XGrabKey(dpy, XKeysymToKeycode (dpy, i ), AnyModifier, s->key_window, True,
	     GrabModeAsync, GrabModeAsync);
  XGrabKey(dpy, XKeysymToKeycode (dpy, KEY_TERM ), AnyModifier, s->key_window, True,
	   GrabModeAsync, GrabModeAsync);
  XGrabKey(dpy, XKeysymToKeycode (dpy, KEY_EDIT ), AnyModifier, s->key_window, True,
	   GrabModeAsync, GrabModeAsync);
  XGrabKey(dpy, XKeysymToKeycode (dpy, KEY_MENU ), AnyModifier, s->key_window, True,
           GrabModeAsync, GrabModeAsync);
  XGrabKey(dpy, XKeysymToKeycode (dpy, KEY_PREVWINDOW ), AnyModifier, s->key_window, True,
	   GrabModeAsync, GrabModeAsync);
  XGrabKey(dpy, XKeysymToKeycode (dpy, KEY_NEXTWINDOW ), AnyModifier, s->key_window, True,
	   GrabModeAsync, GrabModeAsync);
  XGrabKey(dpy, XKeysymToKeycode (dpy, KEY_TOGGLEBAR ), AnyModifier, s->key_window, True,
	   GrabModeAsync, GrabModeAsync);
  XGrabKey(dpy, XKeysymToKeycode (dpy, KEY_LASTWINDOW ), AnyModifier, s->key_window, True,
	   GrabModeAsync, GrabModeAsync);
  XGrabKey(dpy, XKeysymToKeycode (dpy, KEY_PREFIX ), AnyModifier, s->key_window, True,
	   GrabModeAsync, GrabModeAsync);
}

static void
grab_prefix_key (Window w)
{
  XGrabKey(dpy, XKeysymToKeycode (dpy, KEY_PREFIX ), MODIFIER_PREFIX, w, True,
	   GrabModeAsync, GrabModeAsync);
}

void
manage (a_window *win, screen_info *s)
{
  XClassHint hint;
  XMapWindow (dpy, win->w);
  XMoveResizeWindow (dpy, win->w, 0, 0, s->root_attr.width, s->root_attr.height);
  XSelectInput (dpy, win->w, PropertyChangeMask);
  XAddToSaveSet(dpy, win->w);
  grab_prefix_key (win->w);
  win->state = STATE_MAPPED;
  if (!XGetClassHint (dpy, win->w, &hint))
    {
      fprintf (stderr, "antiwm: cannot get ClassHint\n");
      exit (EXIT_FAILURE);
    }
  free (win->name);
  if ((win->name = malloc (strlen (hint.res_name) + 1)) == NULL)
    {
      fprintf (stderr, "manage.c:manage():Out of memory!\n");
      exit (EXIT_FAILURE);
    }
  strcpy (win->name, hint.res_name);
  XFree (hint.res_name);
  XFree (hint.res_class);
#ifdef DEBUG
  printf ("window '%s' managed.\n", win->name);
#endif
}

void
unmanage (a_window *w)
{
  remove_from_window_list (w);
}

/* on startup scan windows and manage them */
void
scanwins(screen_info *s)
{
  a_window *win;
  XWindowAttributes attr;
  unsigned int i, nwins;
  Window dw1, dw2, *wins;
  XQueryTree(dpy, s->root, &dw1, &dw2, &wins, &nwins);
#ifdef DEBUG
  printf ("windows: %d\n", nwins);
#endif
  for (i = 0; i < nwins; i++)
    {
      XGetWindowAttributes(dpy, wins[i], &attr);
      if (wins[i] == s->bar_window || wins[i] == s->key_window) continue;
      win = add_to_window_list (s, wins[i]);
      manage (win, s);
    }
  XFree((void *) wins);	/* just playing nice with gcc */
}
