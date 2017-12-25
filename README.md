# Texture Repeat File Node

A shading node for Arnold that allows the user to repeat non-tileable textures without obviously visible repetition. This is inspired by:

 http://www.iquilezles.org/www/articles/texturerepetition/texturerepetition.htm

http://graphics.cs.cmu.edu/people/efros/research/quilting/quilting.pdf

WIP project.

---

### To Compile

The Arnold SDK (version 4.2.14.0) is necessary in order to compile this project. It is available at https://www.solidangle.com/arnold/download/. A C++ compiler is also necessary to compile this project. For Windows, Visual Studio 2008 or 2010 is necessary. In order to use this node in Maya, you also must have the MtoA plugin installed.

In order to compile this project, use the following commands. This assumes the extracted Arnold SDK exists in <arnold_SDKpath>

##### OSX

g++ -I<arnold_SDKpath>/include -L<arnold_SDKpath>bin -lai -dynamiclib textureRepeat.cpp -o textureRepeat.dylib

or 

gcc -I<arnold_SDKpath>/include -L<arnold_SDKpath>bin -lai -dynamiclib textureRepeat.cpp -o textureRepeat.dylib -lstdc++

##### Linux (not tested)

g++ -o textureRepeat.os -c -fPIC -D_LINUX -I<arnold_SDKpath>/include textureRepeat.cpp

g++ -o textureRepeat.so -shared textureRepeat.os -L<arnold_SDKpath>/bin -lai

##### Windows (not tested)

cl /LD /I <arnold_SDKpath>\include /EHsc textureRepeat.cpp /link /LIBPATH:<arnold_SDKpath>\lib ai.lib

---

### To Access in Maya

Arnold looks for shaders in the ARNOLD_PLUGIN_PATH and for shader templates in MTOA_TEMPLATES_PATH, so these variables need to be set in the Maya.env file. ARNOLD_PLUGIN_PATH should point to the directory where the compiled textureRepeat.dylib/textureRepeat.os resides, and MTOA_TEMPLATES_PATH should point to the textureRepat/templates directory. Alternatively, you can also move the compiled shader and template tiles to your existing ARNOLD_PLUGIN_PATH and MTOA_TEMPLATES_PATH.

Once these are set, restart Maya, and the textureRepeat node will be in the 'Texture' folder under the Arnold category in the hypershade's 'Create Bin'. You can also create a textureRepeat node by running "shadingNode -asTexture textureRepeat;" in the script editor.

---

### To Use

Enter the path of your texture file in the 'File Name' field. Use the 'Uv Repeat' field to choose the number of repetitions in the U and V direction. Check the flip, rotate, and offset check boxes to control how your tiles are scattered across UV space. If you want to help eliminate seams between different tiles, you can choose to blur the tile edges. The blur radius controls how extreme the blur is, where 0 is no blur. The blur radius will only have an effect if blur is checked on. Colorspace issues are not yet accounted for, so you may need to plug the output color into a gamma correct node before piping the color into your shader.

---

### Testing

Maya Testing commands:

shadingNode -asShader lambert;
sets -renderable true -noSurfaceShader true -empty -name lambert2SG;
connectAttr -f lambert2.outColor lambert2SG.surfaceShader;
shadingNode -asTexture textureRepeat;
setAttr "textureRepeat1.uvRepeatX" 10;
setAttr "textureRepeat1.uvRepeatY" 10;
connectAttr -f textureRepeat1.outColor lambert2.color;
polyPlane -w 5 -h 5 -sx 10 -sy 10 -ax 0 1 0 -cuv 2 -ch 1;
sets -e -forceElement lambert2SG;


