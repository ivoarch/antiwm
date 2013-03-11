/* AntiWM
 * 2008-2009, Nolan Liebert
 * See LICENSE and README. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "antiwm.h"

a_window *a_window_head, *a_window_tail;
a_window *a_current_window;
a_window *
add_to_window_list (screen_info *s, Window w)
{
  a_window *new_window;
  new_window = malloc (sizeof (a_window));
  if (new_window == NULL)
    {
      fprintf (stderr, "list.c:add_to_window_list():Out of memory!\n");
      exit (EXIT_FAILURE);
    }
  new_window->w = w;
  new_window->scr = s;
  new_window->last_access = 0;
  new_window->prev = NULL;
  if ((new_window->name = malloc (strlen ("Unnamed") + 1)) == NULL)
    {
      fprintf (stderr, "list.c:add_to_window_list():Out of memory.\n");
      exit (EXIT_FAILURE);
    }
  strcpy (new_window->name, "Unnamed");
  if (a_window_head == NULL)
    {
      /* empty list */
      a_window_head = new_window;
      a_window_tail = new_window;
      new_window->next = NULL;
      return new_window;
    }
  /* prepend window to list */
  new_window->next = a_window_head;
  a_window_head->prev = new_window;
  a_window_head = new_window;
  return new_window;
}

/* check if window is already managed */
a_window *
find_window (Window w)
{
  a_window *cur;
  for (cur = a_window_head; cur; cur = cur->next)
    if (cur->w == w) return cur;
  return NULL;
}

void
remove_from_window_list (a_window *w)
{
  if (a_window_head == w) a_window_head = w->next;
  if (a_window_tail == w) a_window_tail = w->prev;
  if (w->prev != NULL) w->prev->next = w->next;
  if (w->next != NULL) w->next->prev = w->prev;
  if (a_current_window == w) a_current_window = a_window_head;
  if (a_current_window && a_current_window->state == STATE_UNMAPPED) next_window ();
  free (w);
#ifdef DEBUG
  printf ("Removed window from list.\n");
#endif
}

void
set_current_window (a_window *win)
{
  a_current_window = win;
}

void
init_window_list ()
{
  a_window_head = a_window_tail = NULL;
  a_current_window = NULL;
}

void
next_window ()
{
  if (a_current_window != NULL)
    {
      a_current_window = a_current_window->next;
      if (a_current_window == NULL)
	{
	  a_current_window = a_window_head;
	}
      if (a_current_window->state == STATE_UNMAPPED) next_window ();
      set_active_window (a_current_window);
    }
}

void
prev_window ()
{
  if (a_current_window != NULL)
    {
      set_current_window (a_current_window->prev);
      if (a_current_window == NULL)
	{
	  a_current_window = a_window_tail;
	}
      if (a_current_window->state == STATE_UNMAPPED) prev_window ();
      set_active_window (a_current_window);
    }
}

a_window *
find_window_by_number (int n)
{
  int i;
  a_window *cur;
  for (i=0, cur=a_window_head; cur; cur=cur->next)
    {
      if (cur->state != STATE_MAPPED) continue;

      if (i == n) return cur;
      else i++;
    }
  return NULL;
}

void
goto_window_number (int n)
{
  a_window *win;
  if ((win = find_window_by_number (n)) == NULL)
    {
      return;
    }
  a_current_window = win;
  set_active_window (a_current_window);
}

a_window *
find_last_accessed_window ()
{
  int last_access = 0;
  a_window *cur, *most_recent;

  /* find first window */
  for (most_recent = a_window_head; most_recent; most_recent=most_recent->next)
    {
      if (most_recent->state == STATE_MAPPED) break;
    }
  if (most_recent == NULL) return NULL;

  for (cur=a_window_head; cur; cur=cur->next)
    {
      if (cur->last_access >= last_access
	  && cur != a_current_window
	  && cur->state == STATE_MAPPED)
	{
	  most_recent = cur;
	  last_access = cur->last_access;
	}
    }
  return most_recent;
}

void
last_window ()
{
  a_current_window = find_last_accessed_window ();
  set_active_window (a_current_window);
}

void
set_active_window (a_window *a_w)
{
  static int counter = 1;	/* track last window accessed */
  if (a_w == NULL) return;
  counter++;
  a_w->last_access = counter;
  if (a_w->scr->bar_is_raised) update_window_names (a_w->scr);
  XSetInputFocus (dpy, a_w->w,
		  RevertToPointerRoot, CurrentTime);
  XRaiseWindow (dpy, a_w->w);
  update_window_names (a_w->scr);
}
