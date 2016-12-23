# RGB-LED-Beat-Detection

For those of you who asked to see the code, here it is.

Feel free to message or email me if you have any questions.

Disclaimer: this was the first Java/Processing program I ever wrote, 
having only knowledge in basic C, and zero prior experience with any object oriented language. 
The GUI code is a giant, barely readable block of variables and redundant code sections.
I'll rewrite this someday, but for now this is it.  
If you want to make your own version I recommend you start over with just the beat detection
code (and actually use classes for GUI elements).

The beat detection is copied directly from the code
by kctess5 (https://github.com/kctess5/Processing-Beat-Detection)

The sketch's default sound input source is your mic. On windows you can use 
'Stereo Mix' to redirect your sound card output into mic. This will
generally give you more accurate beat detection.

# Files
The data folder contains two text files, port.txt and settings.txt.  
port.txt just contains the COM port your arduino is on.
settings.txt stores the settings for the program.

The Arduino folder contains the arduino sketch that that is receiving the HSI brightness values.
It converts the values to RGB and uses 3 pwm pins as output. 


