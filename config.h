#pragma once
/* This is a configuration file (duh)
 * You can read what each line does, and if you
 * want to enable or diable the feature, just
 * remove the comment and recompile kadaif.
 */

// Removes the "title" when editing a file. By "title" I mean the name of the file being edited.
#define NO_TITLE false

// Makes the dynamic button panel look cooler. Disabled by default as it depends on wether you are
// using a light or dark theme. The default is a lame flat panel.
#define COOL_PANEL true

// do not enable under any circumstance
//#define KAWAII

// Adds more BASS youtu.be/bZWzm3aFhTU
//#define MANGOONAFORK

// Optimize code for rendering pixelart. Use bilinear filtering when disabled.
#define PIXELART true

// Maximum size of images when zooming in/out
#define MAX_SCALE 5.0
#define MIN_SCALE 0.2

// Relative amount to zoom in/out by for each step
#define SCALE_FACTOR 1.25

// What to use for EOL. "\n" for LF, "\r\n" for CRLF.
// Default is CRLF because most people use windows (sadly) and don't know what this means.
#define NEWLINE "\r\n"

// What fonts do you want to use. I could have made it editable without recompiling, but i chose not to.
// Qt will try to find the closest match if an exact one can't be found.
#define FONT_SERIF "Courier"
#define FONT_SANS "sans-serif"
