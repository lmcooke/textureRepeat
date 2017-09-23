# Texture Repeat File Node

A shading node for Arnold that allows the user to repeat non-tileable textures without obviously visible repetition. This is inspired by http://www.iquilezles.org/www/articles/texturerepetition/texturerepetition.htm

---

### To Use

The Arnold SDK (version 4.2.14.0) is necessary in order to compile this project. It is available at https://www.solidangle.com/arnold/download/. A C++ compiler is also necessary to compile this project. In order to use this node in Maya, you also must have the MtoA plugin installed.

In order to compile this project, use the following commands. This assumes the extracted Arnold SDK exists in <arnold_SDKpath>

#### OSX

g++ -I<arnold_SDKpath>/include -L<arnold_SDKpath>bin -lai -dynamiclib fileRepeat.cpp -o fileRepeat.dylib

or 

gcc -I<arnold_SDKpath>/include -L<arnold_SDKpath>bin -lai -dynamiclib fileRepeat.cpp -o fileRepeat.dylib -lstdc++

#### Linux (not tested)

g++ -o fileRepeat.os -c -fPIC -D_LINUX -I<arnold_SDKpath>/include fileRepeat.cpp

g++ -o fileRepeat.so -shared fileRepeat.os -L<arnold_SDKpath>/bin -lai

#### Windows (not tested, apparently this requires VS 2008 or 2010)

cl /LD /I <arnold_SDKpath>\include /EHsc fileRepeat.cpp /link /LIBPATH:<arnold_SDKpath>\lib ai.lib