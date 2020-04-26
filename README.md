# photoViewer

Supply a PhotoViewer.ini file with these fields:

# where the photos are
Directory="<directory name of photos>"

# If true, sleep for 1/2 the display time (give up the CPU). This is for
# the Raspberry Pi, that was getting hot. Default false
SleepMode= <boolean>

# Time to show each photo, default 30 seconds
DisplayTime = <seconds>

# Display the file name, default true
DisplayFileName = <boolean>

