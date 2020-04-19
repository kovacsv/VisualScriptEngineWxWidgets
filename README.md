# VisualScriptEngineWxWidgets

[![Build Status](https://ci.appveyor.com/api/projects/status/0yniyf4ojyl1omy2?svg=true)](https://ci.appveyor.com/project/kovacsv/visualscriptenginewxwidgets)

VisualScriptEngineWxWidgets is a utility module for [VisualScriptEngine](https://github.com/kovacsv/VisualScriptEngine) which provides helper classes for embedding the engine in a wxWidgets application.

# Build

You can build with Visual Studio by following these steps.
* Build [wxWidgets](https://www.wxwidgets.org):
  * [Download and install wxWidgets v3.1.2 from here](https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.2/wxMSW-3.1.2-Setup.exe).
  * Open the solution from `build\msw\wx_vc15.sln` from the wxWidgets installation folder.
  * Build with Debug/x64 and Release/x64 configuration.
* Build [VisualScriptEngine](https://github.com/kovacsv/VisualScriptEngine):
  * [Download the latest version of VisualScriptEngine from here](https://github.com/kovacsv/VisualScriptEngine/archive/master.zip).
  * Generate the project with cmake, and build it with Debug/x64 and Release/x64 configuration.
  * Build the `INSTALL` project for both configurations.
* Build VisualScriptEngineWxWidgets:
  * Set the following environment variables:
    * `WXWIDGETS_DIR`: the wxWidgets installation directory (by default C:\wxWidgets-3.1.2)
    * `VSE_DEVKIT_DIR`: the VisualScriptEngine development kit directory (the DevKit folder next to the generated solution)
  * Generate the project with cmake and build it.
