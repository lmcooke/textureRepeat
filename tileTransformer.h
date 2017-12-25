#ifndef TILETRANSFORMER_H
#define TILETRANSFORMER_H

#include <ai.h>

class TileTransformer
{

public:
	TileTransformer();
	TileTransformer(AtPoint uvRepeat);
	~TileTransformer();

	AtColor calculateColor(AtShaderGlobals* sg);
	AtPoint2 flipUVpoint(AtPoint2 frac_uv, float seedVal);
	AtPoint2 rotateUVpoint(AtPoint2 frac_uv, float seedVal);
	AtPoint2 offsetUVpoint(AtPoint2 frac_uv, float seedVal1, float seedVal2);
	
	float getRandFromIncrement(float incrNum, float seed);

	void updateRepeatParam(AtPoint uvRepeat);
	void updateTexture(AtTextureHandle* textureHandle, 
						AtTextureParams* textureParams);
	void updateSettings(bool flipTiles, bool rotateTiles,
						bool offsetTiles);

	void update(AtPoint uvRepeat, AtTextureHandle* textureHandle,
				AtTextureParams* textureParams,
				bool flipTiles, bool rotateTiles, bool offsetTiles);

	AtColor testFunction();


	AtPoint m_uvRepeat;
	AtTextureHandle* m_textureHandle;
	AtTextureParams* m_textureParams;
	bool m_flipTiles;
	bool m_rotateTiles;
	bool m_offsetTiles;
	bool m_useBlur;

private:

};


#endif