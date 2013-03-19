#ifndef _LIST_H
#define _LIST_H

a_window *add_to_window_list (screen_info *s, Window w);
void init_window_list ();
void remove_from_window_list (a_window *w);
void next_window ();
void prev_window ();
void last_window ();
a_window *find_window (Window w);
void set_active_window (a_window *a_w);
void set_current_window (a_window *win);
void goto_window_number (int n);
#endif
