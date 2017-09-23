# Texture Repeat File Node

A shading node for Arnold that allows the user to repeat non-tileable textures without obviously visible repetition. This is inspired by http://www.iquilezles.org/www/articles/texturerepetition/texturerepetition.htm

---

### To Compile

The Arnold SDK (version 4.2.14.0) is necessary in order to compile this project. It is available at https://www.solidangle.com/arnold/download/. A C++ compiler is also necessary to compile this project. In order to use this node in Maya, you also must have the MtoA plugin installed.

In order to compile this project, use the following commands. This assumes the extracted Arnold SDK exists in <arnold_SDKpath>

##### OSX

g++ -I<arnold_SDKpath>/include -L<arnold_SDKpath>bin -lai -dynamiclib fileRepeat.cpp -o fileRepeat.dylib

or 

gcc -I<arnold_SDKpath>/include -L<arnold_SDKpath>bin -lai -dynamiclib fileRepeat.cpp -o fileRepeat.dylib -lstdc++

##### Linux (not tested)

g++ -o fileRepeat.os -c -fPIC -D_LINUX -I<arnold_SDKpath>/include fileRepeat.cpp

g++ -o fileRepeat.so -shared fileRepeat.os -L<arnold_SDKpath>/bin -lai

##### Windows (not tested, apparently this requires VS 2008 or 2010)

cl /LD /I <arnold_SDKpath>\include /EHsc fileRepeat.cpp /link /LIBPATH:<arnold_SDKpath>\lib ai.lib

---

### To Use in Maya

Arnold looks for shaders in the ARNOLD_PLUGIN_PATH and for shader templates in MTOA_TEMPLATES_PATH, so these variables need to be set in the Maya.env file. ARNOLD_PLUGIN_PATH should point to the directory where the compiled fileRepeat.dylib/fileRepeat.os resides, and MTOA_TEMPLATES_PATH should point to the textureRepat/templates directory.

Once these are set, restart Maya, and the fileRepeat node will be in the 'Texture' folder under the Arnold category in the hypershade's 'Create Bin'. You can also create a fileRepeat node by running "shadingNode -asTexture fileRepeat;" in the script editor.

---

### Testing

Maya Testing commands:

shadingNode -asShader lambert;
sets -renderable true -noSurfaceShader true -empty -name lambert2SG;
connectAttr -f lambert2.outColor lambert2SG.surfaceShader;
shadingNode -asTexture fileRepeat;
setAttr "fileRepeat1.uvRepeatX" 10;
setAttr "fileRepeat1.uvRepeatY" 10;
connectAttr -f fileRepeat1.outColor lambert2.color;
polyPlane -w 5 -h 5 -sx 10 -sy 10 -ax 0 1 0 -cuv 2 -ch 1;
sets -e -forceElement lambert2SG;


