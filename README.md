# photoViewer

I wasn't able to find a good slide show program for the Raspberry Pi,
so I wrote my own.

Over time the plan is to expand this into a full image viewer application,
but right now it's just a slide show.

---------------------------------------------------------------------------
Just compile with cmake and supply an .ini file (see below).
Remember to copy the qt plugins directory "imageformats" here.

---------------------------------------------------------------------------
Supply a PhotoViewer.ini file with these fields:

# where the photos are
Directory="<directory name of photos>"

# If true, sleep for 1/2 the display time (give up the CPU). This is for
# the Raspberry Pi (it was getting hot). Default false.
SleepMode= <boolean>

# Time to show each photo, default 30 seconds.
DisplayTime = <seconds>

# Display the file name, default true.
DisplayFileName = <boolean>

---------------------------------------------------------------------------
Orientation code copied from:
https://www.eastfist.com/qt_tutorials/index.php/2017/02/07/how-to-read-exif-orientation-from-jpeg-in-qt-4-8-5/
