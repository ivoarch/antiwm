#ifndef _MANAGE_H
#define _MANAGE_H

#include "data.h"

void grab_keys ();
void scanwins(screen_info *s);
void manage (a_window *w, screen_info *s);
void unmanage (a_window *w);
#endif
