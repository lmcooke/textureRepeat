#ifndef TEXTUREREPEATCONTROLLER_H
#define TEXTUREREPEATCONTROLLER_H

#include <ai.h>

class TextureRepeatController
{

public:

	TextureRepeatController();
	TextureRepeatController(AtPoint uvRepeat,
							AtTextureHandle* textureHandle,
							AtTextureParams* textureParams,
							bool flipTiles,
							bool rotateTiles,
							bool offsetTiles,
							bool blurEdges,
							float blurRadius,
							bool quiltEdges,
							float quiltWidth);
	~TextureRepeatController();

	void update(AtPoint uvRepeat,
				AtTextureHandle* textureHandle,
				AtTextureParams* textureParams,
				bool flipTiles,
				bool rotateTiles,
				bool offsetTiles,
				bool blurEdges,
				float blurRadius,
				bool quiltEdges,
				float quiltWidth);

	AtColor calculateColor(AtShaderGlobals* sg);

	AtColor blurEdges(AtShaderGlobals* sg);

	AtColor testFunction();


	// member variables
	TileTransformer *m_tileTransformer;
	AtPoint m_uvRepeat;
	AtTextureHandle* m_textureHandle;
	AtTextureParams* m_textureParams;
	bool m_flipTiles;
	bool m_rotateTiles;
	bool m_offsetTiles;
	bool m_blurEdges;
	float m_blurRadius;
	bool m_quiltEdges;
	float m_quiltWidth;

private:

};

#endif