OpenGL D Object Viewer
=====================

Description
-----------

This is code I wrote for a project while in school.  I didn't want to leave it sitting on my computer when it could potentially help someone else.  The code is C++, and probably not great C++ :)  If I have some time I will go through and add additional comments.

Right now it is a working XCode 4.0 project, but it doesn't require XCode or OSX.  The code was tested on Windows and Linux.

Requirements
------------

* OpenGL
* GLUT
* C++ compiler
* D Object Files (Included)

There is no make file right now.  If someone does put one together for another system I will add it to this repository.

Build the application then run it:
    ./application skull.d

Known Issues
------------

XCode and the default debugging options do not work with the STL and cause the application to crash.  If compiling under XCode, compile with the standard release build options and not the debug options.
