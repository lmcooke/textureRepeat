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

AtColor TextureRepeatController::calculateColor(AtShaderGlobals* sg)
{
	return m_tileTransformer->calculateColor(sg);
}

AtColor TextureRepeatController::testFunction()
{
	return m_tileTransformer->testFunction();
	// return AiColor(1.0f, 0.0f, 0.0f);
}




#endif