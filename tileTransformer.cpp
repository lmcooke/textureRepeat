#ifndef TILETRANSFORMER_CPP
#define TILETRANSFORMER_CPP

#include <math.h>
#include <cassert>

#include "tileTransformer.h"




// TODO : use shared_ptr to initialize member vars
TileTransformer::TileTransformer() :
	m_uvRepeat(),
	m_textureHandle(),
	m_textureParams(),
    m_flipTiles(),
    m_rotateTiles(),
    m_offsetTiles()
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
    float tile_u = static_cast<float>(floor(repeatingUV_u));
    float tile_v = static_cast<float>(floor(repeatingUV_v));

    // current uv fraction value
    float frac_u = repeatingUV_u - tile_u;
    float frac_v = repeatingUV_v - tile_v;


    // transform sg to get seed color at start of frac tile
    sg->u = tile_u / m_uvRepeat.x;
    sg->v = tile_v / m_uvRepeat.y;

    bool success1;
    AtColor seedColor = AiTextureHandleAccess(sg, m_textureHandle, 
    											m_textureParams, 
    											&success1).rgb();
    float greyScaleVal = (seedColor.r * 0.3f) + 
    						(seedColor.g * 0.59f) +
    						(seedColor.b * 0.11f);

    // secondary seed for offset (won't work too well with b+w images...)
    float greyScaleValR = (seedColor.r * 0.8f) + 
                        (seedColor.g * 0.12f) +
                        (seedColor.b * 0.08f);


    AtPoint2 frac_uv = AtPoint2();
    frac_uv.x = frac_u;
    frac_uv.y = frac_v;

    if (m_flipTiles) {

        frac_uv = flipUVpoint(frac_uv, greyScaleVal);
    }

    if (m_rotateTiles) {

        frac_uv = rotateUVpoint(frac_uv, greyScaleVal);
    }

    if (m_offsetTiles) {

        frac_uv = offsetUVpoint(frac_uv, greyScaleVal, greyScaleValR);
    }

    sg->u = frac_uv.x;
    sg->v = frac_uv.y;

    // TODO : blur edges?

    bool success2;
    // return AiTextureHandleAccess(sg, m_textureHandle, 
    //                             m_textureParams, 
    //                             &success2).rgb();

    AtColor sampledTexture = AiTextureHandleAccess(sg, m_textureHandle,
                                            m_textureParams,
                                            &success2).rgb();


    return sampledTexture;


}

// given seed value and current uv point, will transform uv point
AtPoint2 TileTransformer::flipUVpoint(AtPoint2 frac_uv, float seedVal)
{

    float randVal = getRandFromIncrement(8.f, seedVal);

    // ugly, i know
    AtColor color = AiColor(0.0f, 0.0f, 0.0f);
    if (randVal == 1) {
        color = AiColor(1.0f, 0.0f, 0.0f);
        // A1

    } else if (randVal == 2){
        color = AiColor(0.0f, 0.0f, 1.0f);

        // A2
        float tempU = frac_uv.x;
        frac_uv.x = frac_uv.y;
        frac_uv.y = tempU;

        frac_uv.y = 1.f - frac_uv.y;

    } else if (randVal == 3) {
        color = AiColor(0.0f, 1.0f, 0.0f);

        // B1
        frac_uv.x = 1.f - frac_uv.x;

    } else if (randVal == 4) {
        color = AiColor(1.0f, 1.0f, 0.0f);

        // B2
        float tempU = frac_uv.x;
        frac_uv.x = frac_uv.y;
        frac_uv.y = tempU;

        frac_uv.x = 1.f - frac_uv.x;
        frac_uv.y = 1.f - frac_uv.y;


    } else if (randVal == 5) {
        color = AiColor(0.0f, 1.0f, 1.0f);

        // C1
        frac_uv.y = 1.f - frac_uv.y;

    } else if (randVal == 6) {
        color = AiColor(1.0f, 0.0f, 1.0f);

        // C2
        float tempU = frac_uv.x;
        frac_uv.x = frac_uv.y;
        frac_uv.y = tempU;


    } else if (randVal == 7) {
        color = AiColor(1.0f, 1.0f, 1.0f);

        // D1
        float tempU = frac_uv.x;
        frac_uv.x = frac_uv.y;
        frac_uv.y = tempU;

        frac_uv.x = 1.f - frac_uv.x;

    } else {
        color = AiColor(0.0f, 0.0f, 0.0f);

        // D2
        frac_uv.x = 1.f - frac_uv.x;
        frac_uv.y = 1.f - frac_uv.y;
    }

    AtPoint2 toReturn = AtPoint2();
    toReturn.x = frac_uv.x;
    toReturn.y = frac_uv.y;

    return toReturn;

}

AtPoint2 TileTransformer::rotateUVpoint(AtPoint2 frac_uv, float seedVal)
{
    // TODO : define this at top of class
    float piF = 3.1415;

    // get rand angle 
    float angleVariation = 23.f; // this will be better if prime
    float randVal = getRandFromIncrement(angleVariation, seedVal);
    float randAngle = (2.f * piF) / randVal;

    // shift pt to centered in center of tile
    float centeredU = frac_uv.x + .5f;
    float centeredV = frac_uv.y + .5;

    // get distance to center
    float h = sqrt(pow(centeredU, 2) + pow(centeredV, 2));

    // get current angle
    float origAngle = acos(centeredU/h);

    // add random angle based on seed
    float newAngle = origAngle + randAngle;

    // get new coords in centered position!
    float newUc = cos(newAngle) * h;
    float newVc = sin(newAngle) * h;


    // shift back to orig position
    AtPoint2 toReturn = AtPoint2();
    toReturn.x = newUc - .5f;
    toReturn.y = newVc - .5f;
    return toReturn;
}

AtPoint2 TileTransformer::offsetUVpoint(AtPoint2 frac_uv, float seedVal1, float seedVal2)
{
    // get rand
    float randIncr = 15.f;
    float randX = getRandFromIncrement(randIncr, seedVal1);
    float randY = getRandFromIncrement(randIncr, seedVal2);

    AtPoint2 toReturn = AtPoint2();
    toReturn.x = frac_uv.x + (randX / randIncr);
    toReturn.y = frac_uv.y + (randY / randIncr);
    return toReturn;
}


float TileTransformer::getRandFromIncrement(float incrNum, float seed)
{
	float incr = 1.0f/incrNum;

	int incrNumInt = static_cast<int>(incrNum);
	int randVal = incrNum;
	for (int i = 1; i < (incrNumInt + 1); i++) {
		float iFloat = static_cast<float>(i);
		if (seed < (iFloat / incrNum)) {
			randVal = i;
			break;
		}
	}

	return randVal;
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

void TileTransformer::updateSettings(bool flipTiles, bool rotateTiles,
                                        bool offsetTiles)
{
    m_flipTiles = flipTiles;
    m_rotateTiles = rotateTiles;
    m_offsetTiles = offsetTiles;
}

void TileTransformer::update(AtPoint uvRepeat, 
                            AtTextureHandle* textureHandle,
                            AtTextureParams* textureParams,
                            bool flipTiles, bool rotateTiles,
                            bool offsetTiles)
{
    updateRepeatParam(uvRepeat);
    updateTexture(textureHandle, textureParams);
    updateSettings(flipTiles, rotateTiles, offsetTiles);
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