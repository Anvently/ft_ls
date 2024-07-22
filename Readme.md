# FT_LS

### Features

# Filtering flags

- **a** / **--all** => show files starting with **.**, including **.** and **..**
- **A** / **--almost-all** => show files starting with **.**, excepting **.** and **..**
- **d** / **--directory** => only list directory information instead of their contents

# Formatting flags

- **l** => long listing, show extended information
- **g** => enable long listing and skip owner user id in long listing
- **G** => skip group id in long listing
- **--color[=auto (default); never; always]** => colorize filename from ***LS_COLOR*** environment variable
  - auto => detect if **STDOUT** is a tty.
- **C** => force colon formatting, even with none tty outputs
- **1** => disable colon formatting and output one file per line
- **w** / **--width** => force a fixed width for colon formatting instead of the deafault behaviour of adapting colon widths to terminal size
- **h** / **--human-readable** => express file size using units instead of the default behaviour of using block size.
- 

# Data flags

- **i** / **--inode** => output inode number of each file, available in long listing and colon formatting
- **--time=[atime (default); mtime; ctime; btime]** => set the timestamp used when sorting files per time or the timestamp printed when using long listing
- **u** => enable time sorting and use access time instead of modification time (equivalent to ***-t*** + ***--time=mtime***)
- **L** / **--dereference** => display information about files pointed by symlink instead of information about the symlinks themselves

# Sorting flags

- **U** => do not sort
- **t** => sort by time (default is *mtime*)
- **u** => enable time sorting and use access time instead of modification time (equivalent to ***-t*** + ***--time=mtime***)
- **S** => sort by file size, largest first
- **--sort=[time; size; ascii; alpha; extension]** => specify sorting criteria
  - ascii => sort by filename by strict comparison using ASCII table
  - alpha => sort by filename using case-insensitive comparison and ignoring leading dots
  - extension => sort by extension using case-insentive comparison
- **r** => reverse sort

# Other flags

- **R** / **--recursive** => enable recursive file listing, list every subfolder
- **f** / **--fast** => optimize the output for fast printing. Disable colors, enable ***r, U, 1***
