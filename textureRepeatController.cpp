#ifndef TEXTUREREPEATCONTROLLER_CPP
#define TEXTUREREPEATCONTROLLER_CPP

#include "textureRepeatController.h"

TextureRepeatController::TextureRepeatController() :
	m_tileTransformer(),
	m_uvRepeat(),
	m_textureHandle(),
	m_textureParams(),
	m_flipTiles(),
	m_rotateTiles(),
	m_offsetTiles(),
	m_blurEdges(),
	m_blurRadius(),
	m_quiltEdges(),
	m_quiltWidth()
{

	m_tileTransformer = new TileTransformer();
}

TextureRepeatController::TextureRepeatController(AtPoint uvRepeat,
							AtTextureHandle* textureHandle,
							AtTextureParams* textureParams,
							bool flipTiles,
							bool rotateTiles,
							bool offsetTiles,
							bool blurEdges,
							float blurRadius,
							bool quiltEdges,
							float quiltWidth)
{
	m_uvRepeat = uvRepeat;
	m_textureHandle = textureHandle;
	m_textureParams = textureParams;
	m_flipTiles = flipTiles;
	m_rotateTiles = rotateTiles;
	m_offsetTiles = offsetTiles;
	m_blurEdges = blurEdges,
	m_blurRadius = blurRadius;
	m_quiltEdges = quiltEdges;
	m_quiltWidth = quiltWidth;


	m_tileTransformer = new TileTransformer(m_uvRepeat,
										m_textureHandle,
										m_textureParams,
										m_flipTiles,
										m_rotateTiles,
										m_offsetTiles);

	m_tileTransformer->testFunction();

}

TextureRepeatController::~TextureRepeatController()
{
	delete m_tileTransformer;
}


AtColor TextureRepeatController::calculateColor(AtShaderGlobals* sg)
{
	
	if (m_blurEdges) {
		// TODO

		return blurEdges(sg);
		// return AiColor(1.0f, 0.0f, 0.0f);

	} else if (m_quiltEdges) {
		// TODO
		return AiColor(0.0f, 1.0f, 0.0f);

	} else {
		// no edge improvement settings 
		return m_tileTransformer->calculateColor(sg);
	}


	
}

AtColor TextureRepeatController::blurEdges(AtShaderGlobals* sg)
{
	
	float origU = sg->u;
    float origV = sg->v;

	// get UVs with repeating value
	float repeatingUV_u = sg->u * m_uvRepeat.x;
	float repeatingUV_v = sg->v * m_uvRepeat.y;

	// get tile
	float tile_u = static_cast<float>(floor(repeatingUV_u));
	float tile_v = static_cast<float>(floor(repeatingUV_v));

	// get current uv fraction value
	float frac_u = repeatingUV_u - tile_u;
	float frac_v = repeatingUV_v - tile_v;

	AtColor toReturn = m_tileTransformer->calculateColor(sg);

	// left
	if (frac_u < m_blurRadius) {

		sg->u = sg->u - frac_u;

		AtColor leftMargin = m_tileTransformer->calculateColor(sg);

		sg->u = origU;

		// linear interpolation
		float ratio = frac_u/m_blurRadius;
		toReturn = (ratio * toReturn) + ((1.f - ratio) * leftMargin);

	}

	// top
	if (frac_v < m_blurRadius) {

		sg->v = sg->v - frac_v;

		AtColor topMargin = m_tileTransformer->calculateColor(sg);

		sg->v = origV;
		
		// linear interpolation
		float ratio = frac_v/m_blurRadius;
		toReturn = (ratio * toReturn) + ((1.f - ratio) * topMargin);

	}

	return toReturn;
}


void TextureRepeatController::update(AtPoint uvRepeat,
			AtTextureHandle* textureHandle,
			AtTextureParams* textureParams,
			bool flipTiles,
			bool rotateTiles,
			bool offsetTiles,
			bool blurEdges,
			float blurRadius,
			bool quiltEdges,
			float quiltWidth)
{
	m_uvRepeat = uvRepeat;
	m_textureHandle = textureHandle;
	m_textureParams = textureParams;
	m_flipTiles = flipTiles;
	m_rotateTiles = rotateTiles;
	m_offsetTiles = offsetTiles;
	m_blurEdges = blurEdges,
	m_blurRadius = blurRadius;
	m_quiltEdges = quiltEdges;
	m_quiltWidth = quiltWidth;

	m_tileTransformer->update(m_uvRepeat,
							m_textureHandle,
							m_textureParams,
							m_flipTiles,
							m_rotateTiles,
							m_offsetTiles);
}


AtColor TextureRepeatController::testFunction()
{
	return m_tileTransformer->testFunction();
}




#endif