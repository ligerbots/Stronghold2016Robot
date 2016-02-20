# Setting Up SmartDashboard 2.0 #

1. Copy all of the plugins from `plugins/` to `%userprofile%\wpilib\tools\plugins`
2. Open the save file `Stronghold2016.fxsdash` in SmartDashboard

## Setting DriverStation to launch SmartDashboard 2.0 ##

SmartDashboard 2.0 doesn't like being launched from DriverStation, so following the WPILib screensteps instructions won't work.

Put this in `%userprofile%\wpilib\tools\sfx.bat`

    java -jar "%userprofile%\wpilib\tools\sfx.jar" "<your workspace path>\Stronghold2016Robot\Dashboard\Stronghold2016.fxsdash"

In `C:\Users\Public\Documents\FRC\FRC DS Data Storage.ini` set `DashboardCmdLine` as

    DashboardCmdLine = ""<absolute path to your user profile folder>\\wpilib\\tools\\sfx.bat""

Double quotes and double slashes are necessary. Using `%userprofile%` doesn't work (the path has to be absolute or DS won't launch it).


## Screen size we have for SmartDashboard ##

`1440x690`