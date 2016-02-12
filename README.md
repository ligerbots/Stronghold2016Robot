# Stronghold2016Robot
Robot program for FRC 2016 Stronghold

## Documentation ##

### How to turn vision assistant output into something that compiles ###
Go to the .c file, remove

    #include <stdlib.h>
    #include <string.h>
    #include <math.h>
    #include <nivision.h>
    #include <nimachinevision.h>
    #include <windows.h>

and add 

    #include <nivision.h>
    #include <string.h>
    #include <stdlib.h>
    #include <stdio.h>
    #pragma GCC diagnostic ignored "-Wformat-zero-length"
    #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
    #define BOOL int

then in the .h file remove

    #include <nimachinevision.h>

---

### Notes on various utilities / added functions ###

* LCameraServer
    * Fixes the CameraServer bug which requires opening the Labview dashboard
  * **Important**: Do not ever use CameraServer and LCameraServer at the same. They both attempt to create sockets on the same port

* `OI::registerButton(Joystick\*, int, ButtonEvent, Command\*)`
    * Internally keeps track of JoystickButton instances. Use this to attach commands to joystick buttons in OI

* `Parameter<type>`
    * Wrapper around WPILib preferences that automatically sends and receives updates. Use like a normal variable of the type. The second constructor parameter is true to make it a Preference (default) or false to make it a normal SmartDashboard value (not saved on rio memory)

* Stronghold2016Robot.h and Includes.h
    * In cpp files, use `#include <Stronghold2016Robot.h>` at the top of the file (before comments)
    * Put any C++ standard library includes in Includes.h
    * The compile script will automatically put Includes.h and all .h files into Stronghold2016Robot.h and attempt to precompile it

* `Robot::AlwaysPeriodic()`
    * Called from `TeleopPeriodic`, `AutonomousPeriodic`, and `DisabledPeriodic`
    * Use for updating the dashboard (eg the camera feed) and other tasks that always need to run


## GCC Error Translations ##

### Linker ###

- `undefined reference to <class member>` The static member is in your header file but you forgot to define it in your cpp file
- `undefined reference to vtable for <class>` There are virtual methods in that class or one of its superclasses that aren't pure (` = 0`) and aren't implemented. Add a body or make them pure.
- `Duplicate definition of <xyz>` There are stray .o files, do a build clean

### Compiler ###

- `<xyz> does not name a type` You forgot to `#include "xyz.h"`