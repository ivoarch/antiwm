#ifndef _EVENTS_H
#define _EVENTS_H

extern void hup_handler ();
void handle_events ();
void delegate_event (XEvent *ev);
void key_press (XEvent *ev);
void map_request (XEvent *ev);
void unmap_notify (XEvent *ev);
#endif
