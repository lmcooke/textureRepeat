#ifndef TILETRANSFORMER_CPP
#define TILETRANSFORMER_CPP

#include <math.h>

#include "tileTransformer.h"

// TODO : use shared_ptr to initialize member vars
TileTransformer::TileTransformer() :
	m_uvRepeat(),
	m_textureHandle(),
	m_textureParams()
{

}

// TODO : make constructor include texture stuff also
TileTransformer::TileTransformer(AtPoint uvRepeat) :
	m_uvRepeat(uvRepeat),
	m_textureHandle(),
	m_textureParams()
{

}

TileTransformer::~TileTransformer()
{

}

AtColor TileTransformer::calculateColor(AtShaderGlobals* sg)
{
	// get UVs with repeating value
	float repeatingUV_u = sg->u * m_uvRepeat.x;
	float repeatingUV_v = sg->v * m_uvRepeat.y;

	// get current UV tile
	int tile_uI = floor(repeatingUV_u);
    int tile_vI = floor(repeatingUV_v);

    float tile_u = static_cast<float>(floor(repeatingUV_u));
    float tile_v = static_cast<float>(floor(repeatingUV_v));

    // current uv fraction value
    float frac_u = tile_u - repeatingUV_u;
    float frac_v = tile_v - repeatingUV_v;

    sg->u = tile_u / m_uvRepeat.x;
    sg->v = tile_v / m_uvRepeat.y;

    bool success1;
    AtColor seedColor = AiTextureHandleAccess(sg, m_textureHandle, 
    											m_textureParams, 
    											&success1).rgb();
    float greyScaleVal = (seedColor.r * 0.3f) + 
    						(seedColor.g * 0.59f) +
    						(seedColor.b * 0.11f);

	float randVal;
	// TODO : make this prettier
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

    // ugly, i know
    AtColor color;
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

    bool success2;
    return AiTextureHandleAccess(sg, m_textureHandle, m_textureParams, &success2).rgb();
	// return color;
}

void TileTransformer::updateRepeatParam(AtPoint uvRepeat)
{
	m_uvRepeat = uvRepeat;
}

void TileTransformer::updateTexture(AtTextureHandle* textureHandle, 
						AtTextureParams* textureParams)
{
	m_textureHandle = textureHandle;
	m_textureParams = textureParams;
}


AtColor TileTransformer::testFunction()
{
	if (m_uvRepeat.x == 10) {
		return AiColor(0.0f, 0.0f, 1.0f);
	} else {
		return AiColor(0.0f, 1.0f, 0.0f); 
	}
	
}

#endif