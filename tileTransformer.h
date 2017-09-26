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

	void updateRepeatParam(AtPoint uvRepeat);
	void updateTexture(AtTextureHandle* textureHandle, 
						AtTextureParams* textureParams);

	AtColor testFunction();


	AtPoint m_uvRepeat;
	AtTextureHandle* m_textureHandle;
	AtTextureParams* m_textureParams;

private:

};


#endif