#Virtual Guitar Player

## Features

* Powered by [MuseScore](https://github.com/musescore/MuseScore), the free music notation and composition software
* Guitar Fretboard showing the notes as they are being played
* UI Optimized for learning to play
* WYSIWYG design, notes are entered on a "virtual notepaper"
* TrueType font(s) for printing & display allows for high quality scaling to all sizes
* easy & fast note entry
* many editing functions
* MusicXML import/export
* Midi (SMF) import/export
* MuseData import
* Midi input for note entry
* integrated sequencer and software synthesizer to play the score
* print or create pdf files

## License
VirtualGuitar is licensed under GPL version 2.0. See LICENSE.GPL in the same directory.


## Building
**Read the MuseScore developer handbook for a [complete build walkthrough](http://musescore.org/en/developers-handbook/compilation) and a list of dependencies.**

### Getting sources
If using git to download repo of entire code history, type:

    git clone https://github.com/vguitar/vguitar.git
    cd vguitar

Else can just download the latest source release tarball from https://github.com/vguitar/vguitar/releases, and then from your download directory type:

    tar xzf vguitar-x.x.x.tar.gz
    cd vguitar-x.x.x

### Release Build
To compile Virtual Guitar Player, type:

    make release

If something goes wrong, then remove the whole build subdirectory with `make clean` and start new with `make release`.

### Running
To start Virtual Guitar Player, type:

    ./build.release/mscore/mscore

The Start Center window will appear on every invocation, until you disable that setting via the "Preferences" dialog.

### Installing 
To install to default prefix using root user, type:

    sudo make install

### Debug Build
A debug version can be built by doing `make debug` instead of `make release`.

To run the debug version, type:

    ./build.debug/mscore/mscore

### Program Documentation
To generate program documentation using DoxyGen, first do `make debug`, then type:

    make doxy

Browse the documentation in subdirectory Doc/html/index.html using any html browser.
