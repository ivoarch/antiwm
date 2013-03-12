AntiWM - README
---------------------

About
---------
AntiWM is a simple window manager inspired by ratpoison, which was inspired by GNU screen.
All interaction with the window manager is done through the following keystrokes.

`C-t C-c`
This opens a new terminal session.

`C-t C-e`
This opens a new editor session.

`C-t C-w`
This opens the program bar, which displays currently running windows.
Jump to each window by number with 'C-t C-<n>, where <n> is the number of the window.

`C-t C-m`
This opens the menu program.  **NEW**

`C-t C-b`
This run banish command. **NEW**

`C-t C-p`
This jumps to the previous window in the window list.

`C-t C-n`
This jumps to the next window in the window list.

`C-t C-<n>`
This jumps to a window by number as shown in the Program Bar.

`C-t C-t`
This toggles between the current window and the last window.

`C-t t`
Sends C-t to the current window.

`C-t C-q`
Quit Antiwm. **NEW**

NOTE: These are the default settings. There is no antiwm.conf or antirc to edit.
To configure AntiWM, edit the settings in 'conf.h', then recompile and install.

Building
--------
AntiWM comes with a Makefile that should work in most cases.
If you want to change any part of the make/install process, or if there is trouble compiling, try and edit it.

    cd /path/to/antiwm/src

The path should point to where AntiWM was extracted.

    make

This compiles the program.
Most warnings are fine, but errors should be cause to check the Makefile.

    make install

This installs AntiWM.
By default AntiWM installs to /usr/local/bin.
If this is not where AntiWM should be installed, change the path indicated in the Makefile.
NOTE: This step should be done as a priviledged user.

Running
--------
You have to create/edit the file `~/.xinitrc` with the following line.

    exec xsetroot -cursor_name left_ptr & exec antiwm

Copying
-------
AntiWM is released under the MIT/X Consortium License.
See the LICENSE file for more details.
