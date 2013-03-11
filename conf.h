/* Configure AntiWM here, recompile, and install. */

/* key definitions */
#define KEY_PREFIX      't'          /* prefix key */
#define MODIFIER_PREFIX ControlMask  /* modifier prefix */
#define KEY_TERM        'c'          /* launch a terminal */
#define KEY_EDIT        'e'          /* launch an editor */
#define KEY_MENU        'm'          /* launch a menu program */
#define KEY_BANISH      'b'          /* sends the cursor to the bottom right corner of the screen */
#define KEY_PREVWINDOW  'p'          /* focus previous window */
#define KEY_NEXTWINDOW  'n'          /* focus next windows */
#define KEY_LASTWINDOW  't'	     /* toggle current and previous windows */
#define KEY_TOGGLEBAR   'w'	     /* toggle display of the bar */

/* default commands */
#define TERM_PROG      "xterm"
#define EDIT_PROG      "vim"
#define MENU_PROG      "dmenu_run"

/* appearance */
#define BAR_FG_COLOR    "Gray60"     /* foreground color */
#define BAR_BG_COLOR    "Lightgreen" /* bacground color */
#define BAR_BOLD_COLOR  "Black"      /* indicate current window */
#define FONT_NAME       "fixed"	     /* font to use */
#define BAR_PADDING     1	     /* top and bottom bar padding  */
#define BAR_LOCATION    1	     /* 0=bottom-left 1=top-left 2=bottom-right 3=top-right */
#define BAR_TIMEOUT     5	     /* time before bar autohides 0=don't autohide */
