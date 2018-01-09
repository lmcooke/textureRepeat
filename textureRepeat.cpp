#include <ai.h>

#include <cstring>
#include <math.h>
#include <string.h>
#include <sstream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "tileTransformer.cpp"
#include "textureRepeatController.cpp"

AI_SHADER_NODE_EXPORT_METHODS(textureRepeatMethods);
 
namespace
{
 


enum textureRepeatParams 
{ 
    p_color,
    p_uv_repeat,
    p_fileName,
    p_flipTiles,
    p_offsetTiles,
    p_rotateTiles,
    p_blurEdges,
    p_blurRadius,
    p_quiltEdges,
    p_quiltWidth 
};
 
};
 
node_parameters
{
    AiParameterRGB("constantColor", 0.7f, 0.7f, 0.7f);
    AiParameterPNT2("uv_repeat", 1.0f, 1.0f);
    AiParameterStr("fileName", "");
    AiParameterBOOL("flipTiles", 0);
    AiParameterBOOL("offsetTiles", 0);
    AiParameterBOOL("rotateTiles", 0);
    AiParameterBOOL("blurEdges", 0);
    AiParameterFLT("blurRadius", 0.1f);
    AiParameterBOOL("quiltEdges", 0);
    AiParameterFLT("quiltWidth", 0.1f);
}

struct ShaderData 
{
    AtTextureHandle* texturehandle;
    AtTextureParams *textureparams;
    TextureRepeatController *textureControl;
};
 
node_initialize
{
    ShaderData *data = new ShaderData;

    TextureRepeatController *tpc;
    tpc = new TextureRepeatController();

    std::string texname = std::string(params[p_fileName].STR);
    data->texturehandle = AiTextureHandleCreate(texname.c_str());
    data->textureparams = new AtTextureParams;
    data->textureControl = tpc;

    AiTextureParamsSetDefaults(data->textureparams);
    AiNodeSetLocalData(node, data);
}
 
node_update
{
}
 
node_finish
{
    ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
    
    // clean up
    AiTextureHandleDestroy(data->texturehandle);
    delete data->textureparams;
    delete data->textureControl;
    delete data;
}
 
shader_evaluate
{
    AtColor color = AiShaderEvalParamRGB(p_color);
    AtPoint uvPt = AiShaderEvalParamVec(p_uv_repeat);
    bool flipTiles = AiShaderEvalParamBool(p_flipTiles);
    bool offsetTiles = AiShaderEvalParamBool(p_offsetTiles);
    bool rotateTiles = AiShaderEvalParamBool(p_rotateTiles);
    bool blurEdges = AiShaderEvalParamBool(p_blurEdges);
    float blurRadius = AiShaderEvalParamFlt(p_blurRadius);
    bool quiltEdges = AiShaderEvalParamBool(p_quiltEdges);
    float quiltWidth = AiShaderEvalParamFlt(p_quiltWidth);

    ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

    // set most recent node parameters
    data->textureControl->update(uvPt, data->texturehandle,
                            data->textureparams,
                            flipTiles, rotateTiles, offsetTiles,
                            blurEdges, blurRadius, quiltEdges, quiltWidth);


    sg->out.RGB = data->textureControl->calculateColor(sg);

}
 
node_loader
{
    if (i > 0)
        return false;
 
    node->methods        = textureRepeatMethods;
    node->output_type    = AI_TYPE_RGB;
    node->name           = "textureRepeat";
    node->node_type      = AI_NODE_SHADER;
    strcpy(node->version, AI_VERSION);
    return true;
}