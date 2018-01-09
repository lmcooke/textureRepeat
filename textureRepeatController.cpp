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

		return blurEdges(sg);

	} else if (m_quiltEdges) {

		return quiltEdges(sg);

	} else {
		// no edge improvement settings 
		return m_tileTransformer->calculateColor(sg);
	}


	
}

AtColor TextureRepeatController::blurEdges(AtShaderGlobals* sg)
{
	
	float origU = sg->u;
    float origV = sg->v;

    AtPoint2 fracUV = getUVposition(sg);

	float frac_u = fracUV.x;
	float frac_v = fracUV.y;

	AtColor toReturn = m_tileTransformer->calculateColor(sg);

	// u direction
	if (frac_u < m_blurRadius) {

		sg->u = sg->u - frac_u;

		AtColor leftMargin = m_tileTransformer->calculateColor(sg);

		sg->u = origU;

		// linear interpolation
		float ratio = frac_u/m_blurRadius;
		toReturn = (ratio * toReturn) + ((1.f - ratio) * leftMargin);

	}

	// v direction
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

AtColor TextureRepeatController::quiltEdges(AtShaderGlobals* sg)
{
	float origU = sg->u;
    float origV = sg->v;

    AtPoint2 fracUV = getUVposition(sg);

	// TODO : set resolution based on texture
	float stepSize = 0.01f;

	AtColor toReturn = AiColor(0.0f,0.0f,1.0f);

	// check to see if quilting is necessary
	// left edge
	if (fracUV.x < m_quiltWidth) {
		toReturn = AiColor(0.0f,1.0f,0.0f);

		// build matrix
		int width = static_cast<int>(stepSize / m_quiltWidth);
		int height = static_cast<int>(stepSize / 1.f);

		// TODO : initialize quilt
		// AtColor leftQuilt[][];
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {

				
			}
		}


	}
	if (fracUV.y < m_quiltWidth) {
		toReturn = AiColor(1.0f,0.0f,0.0f);
	}

	return toReturn;
}

AtPoint2 TextureRepeatController::getUVposition(AtShaderGlobals* sg)
{
	AtPoint2 uvPos = AtPoint2();

		// get UVs with repeating value
	float repeatingUV_u = sg->u * m_uvRepeat.x;
	float repeatingUV_v = sg->v * m_uvRepeat.y;

	// get tile
	float tile_u = static_cast<float>(floor(repeatingUV_u));
	float tile_v = static_cast<float>(floor(repeatingUV_v));

	// get current uv fraction value
	uvPos.x = repeatingUV_u - tile_u;
	uvPos.y = repeatingUV_v - tile_v;

	return uvPos;
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