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

	AtColor baseColor = m_tileTransformer->calculateColor(sg);

	// check to see if current coord is within rad of edge
	float colorNum = 1.f;
	float totalR = baseColor.r;
	float totalG = baseColor.g;
	float totalB = baseColor.b;

	AtColor toReturn = baseColor;

	// left
	if (frac_u < m_blurRadius) {
		colorNum += 1.f;

		sg->u = sg->u - frac_u;

		AtColor leftMargin = m_tileTransformer->calculateColor(sg);

		sg->u = origU;

		totalR += leftMargin.r;
		totalG += leftMargin.g;
		totalB += leftMargin.b;

		// linear interpolation
		float ratio = frac_u/m_blurRadius;
		toReturn = (ratio * toReturn) + ((1.f - ratio) * leftMargin);

		// toReturn = leftMargin;
	}

	// right
	if (frac_u > (1.f - m_blurRadius)) {
		colorNum += 1.f;

		sg->u = sg->u + (1.f - frac_u);

		AtColor rightMargin = m_tileTransformer->calculateColor(sg);

		sg->u = origU;

		totalR += rightMargin.r;
		totalG += rightMargin.g;
		totalB += rightMargin.b;

		// linear interpolation
		float ratio = (1.f - frac_u)/m_blurRadius;
		toReturn = (ratio * rightMargin) + ((1.f - ratio) * toReturn);

		// toReturn = rightMargin;
		// toReturn = AiColor(0.0f, 0.0f, 1.0f);
	}

	// top
	if (frac_v < m_blurRadius) {
		colorNum += 1.f;

		sg->v = sg->v - frac_v;

		AtColor topMargin = m_tileTransformer->calculateColor(sg);

		sg->v = origV;

		totalR += topMargin.r;
		totalG += topMargin.g;
		totalB += topMargin.b;
		
		// toReturn = topMargin;
	}

	if (frac_v > (1.f - m_blurRadius)) {
		colorNum += 1.f;

		sg->v = sg->v + (1.f - frac_v);

		AtColor bottomMargin = m_tileTransformer->calculateColor(sg);

		sg->v = origV;

		totalR += bottomMargin.r;
		totalG += bottomMargin.g;
		totalB += bottomMargin.b;
		
		// toReturn = bottomMargin;
	}

	// average colors
	AtColor finalColor = AiColor(totalR / colorNum,
								totalG / colorNum,
								totalB / colorNum);


	// TODO : clamp?


	return toReturn;
	// return finalColor;
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