#!/bin/bash
tmux splitw -h -p 50 "gdbserver :12345 ./curses"
tmux selectp -t 0
gdb -x debug.gdb
