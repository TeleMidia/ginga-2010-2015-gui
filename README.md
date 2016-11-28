# Ginga GUI

Easy-to-use graphical user interfaces (GUIs) for Ginga Reference Implementation
(http://www.ginga.org.br).

This repository groups three different GUIs:

The three interfaces are:

  - __desktop-gui__: A Qt-based GUI for desktop users. 
  - __fullscreen-gui__: A Qt-based GUI for emulating a TV user interface (we
                        mainly use it on minipcs).
  - __run-view__: is a [NCL
                  Composer](http://composer.telemidia.puc-rio.br)
                  plug-in supporting the execution of NCL
                  applications.

## Compiling

To compile one of the above GUIs, you can enter in its directory
(`desktop-gui`, `fullscreen-gui`, or `run-view`) and run:

  > `$ qmake`  && `make`

## TODO

  - [ ] User manual and cheat sheet.
  - [ ] Support command line arguments (--help, --configFile, ...).
  - [ ] Finish support for PBDS.

## Authors

TODO

## License

The code in this repository is available under the
[GPLv3](https://www.gnu.org/licenses/gpl-3.0.en.html) license.

