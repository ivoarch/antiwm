#ifndef _ANTIWM_H
#define _ANTIWM_H

#include "conf.h"
#include "data.h"
#include "manage.h"
#include "list.h"
#include "bar.h"
#include "events.h"

void clean_up ();
screen_info *find_screen (Window w);
#endif
