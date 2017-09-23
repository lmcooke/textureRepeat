#include <ai.h>

#include <cstring>
#include <math.h>
#include <string.h>
#include <sstream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "tileTransformer.h"

AI_SHADER_NODE_EXPORT_METHODS(fileRepeatMethods);
 
namespace
{
 


enum fileRepeatParams 
{ 
    p_color,
    p_uv_repeat,
    p_fileName 
};
 
};
 
node_parameters
{
    AiParameterRGB("constantColor", 0.7f, 0.7f, 0.7f);
    AiParameterPNT2("uv_repeat", 1.0f, 1.0f);
    AiParameterStr("fileName", "");
}

struct ShaderData 
{
    AtTextureHandle* texturehandle;
    AtTextureParams *textureparams;
    // TileTransformer tileTransform;
};
 
node_initialize
{
    ShaderData *data = new ShaderData;
    // TileTransformer *tt;
    // tt = new TileTransformer();
    std::string texname = std::string(params[p_fileName].STR);
    data->texturehandle = AiTextureHandleCreate(texname.c_str());
    data->textureparams = new AtTextureParams;



    AiTextureParamsSetDefaults(data->textureparams);

    AiNodeSetLocalData(node, data);
}
 
node_update
{
}
 
node_finish
{
    ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);
    AiTextureHandleDestroy(data->texturehandle);
    delete data->textureparams;
    delete data;

}
 
shader_evaluate
{
    AtColor color = AiShaderEvalParamRGB(p_color);
    AtPoint uvPt = AiShaderEvalParamVec(p_uv_repeat);

    ShaderData *data = (ShaderData*)AiNodeGetLocalData(node);

    // get UVs with repeating value
    float repeatingUV_u = sg->u * uvPt.x;
    float repeatingUV_v = sg->v * uvPt.y;

    // get current UV tile
    int tile_uI = floor(repeatingUV_u);
    int tile_vI = floor(repeatingUV_v);

    float tile_u = static_cast<float>(floor(repeatingUV_u));
    float tile_v = static_cast<float>(floor(repeatingUV_v));


    // current uv fraction value
    float frac_u = tile_u - repeatingUV_u;
    float frac_v = tile_v - repeatingUV_v;
    

    sg->u = tile_u / uvPt.x;
    sg->v = tile_v / uvPt.y;

    bool success1;
    AtColor seedColor = AiTextureHandleAccess(sg, data->texturehandle, data->textureparams, &success1).rgb();
    float greyScaleVal = (seedColor.r * 0.3f) + (seedColor.g * 0.59f) + (seedColor.b * 0.11f);

    float randVal;
    if (greyScaleVal < .125f) {
        randVal = 1;
    } else if (greyScaleVal < .25f) {
        randVal = 2;
    } else if (greyScaleVal < .375f) {
        randVal = 3;
    } else if (greyScaleVal < .5f) {
        randVal = 4;
    } else if (greyScaleVal < .625f) {
        randVal = 5;
    } else if (greyScaleVal < .75f) {
        randVal = 6;
    } else if (greyScaleVal < .875f) {
        randVal = 7;
    } else {
        // randVal = 0;
    }

    // randomization 1
    // int tileInd = (tile_vI * static_cast<int>(uvPt.x)) + tile_uI;
    
    // int repeatVal = static_cast<int>(uvPt.x) + 3;
    // repeatVal = 8;

    // int randVal = tileInd % repeatVal;

    // randomization 2



    if (randVal == 1) {
        color = AiColor(1.0f, 0.0f, 0.0f);

        // A1

    } else if (randVal == 2){
        color = AiColor(0.0f, 0.0f, 1.0f);

        // A2
        float tempU = frac_u;
        frac_u = frac_v;
        frac_v = tempU;

        frac_v = 1.f - frac_v;

    } else if (randVal == 3) {
        color = AiColor(0.0f, 1.0f, 0.0f);

        // B1
        frac_u = 1.f - frac_u;

    } else if (randVal == 4) {
        color = AiColor(1.0f, 1.0f, 0.0f);

        // B2
        float tempU = frac_u;
        frac_u = frac_v;
        frac_v = tempU;

        frac_u = 1.f - frac_u;
        frac_v = 1.f - frac_v;


    } else if (randVal == 5) {
        color = AiColor(0.0f, 1.0f, 1.0f);

        // C1
        frac_v = 1.f - frac_v;

    } else if (randVal == 6) {
        color = AiColor(1.0f, 0.0f, 1.0f);

        // C2
        float tempU = frac_u;
        frac_u = frac_v;
        frac_v = tempU;


    } else if (randVal == 7) {
        color = AiColor(1.0f, 1.0f, 1.0f);

        // D1
        float tempU = frac_u;
        frac_u = frac_v;
        frac_v = tempU;

        frac_u = 1.f - frac_u;

    } else {
        color = AiColor(0.0f, 0.0f, 0.0f);

        // D2
        frac_u = 1.f - frac_u;
        frac_v = 1.f - frac_v;
    }

 
    
    sg->u = frac_u;
    sg->v = frac_v;


    // do repeating texture logic
    // sg->u = sg->u * uvPt.x;
    // sg->v = sg->v * uvPt.y;

    bool success;
    sg->out.RGB = AiTextureHandleAccess(sg, data->texturehandle, data->textureparams, &success).rgb();
    // sg->out.RGB = color;

}
 
node_loader
{
    if (i > 0)
        return false;
 
    node->methods        = fileRepeatMethods;
    node->output_type    = AI_TYPE_RGB;
    node->name           = "fileRepeat";
    node->node_type      = AI_NODE_SHADER;
    strcpy(node->version, AI_VERSION);
    return true;
}