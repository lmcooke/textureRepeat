import pymel.core as pm
import mtoa.utils as utils
import mtoa.ui.ae.utils as aeUtils
from mtoa.ui.ae.shaderTemplate import ShaderAETemplate



class AEfileRepeatTemplate(ShaderAETemplate):

	def setup(self):

        self.addSwatch()
        self.beginScrollLayout()
 

        self.addCustom('message', 'AEshaderTypeNew',
        'AEshaderTypeReplace')
 
        self.beginLayout("Color Section", collapse=False)
		self.addControl("constantColor", label="Color")


        self.addControl("fileName", label="File Name")
		self.addControl("flipTiles", label="Flip Tiles")
		self.addControl("offsetTiles", label="Offset Tiles")
		self.addControl("rotateTiles", label="Rotate Tiles")
        self.endLayout()
 
        pm.mel.AEdependNodeTemplate(self.nodeName)
        print 'template setup'
        self.addExtraControls()
        self.endScrollLayout()


