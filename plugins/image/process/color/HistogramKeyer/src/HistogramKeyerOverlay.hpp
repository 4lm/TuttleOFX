#ifndef _TUTTLE_PLUGIN_HISTOGRAMKEYEROVERLAY_HPP_
#define _TUTTLE_PLUGIN_HISTOGRAMKEYEROVERLAY_HPP_

#include "HistogramKeyerPlugin.hpp"
#include "RGBOverlay.hpp"
#include "HSLOverlay.hpp"

#include <tuttle/plugin/global.hpp>
#include <tuttle/plugin/interact/interact.hpp>
#include <tuttle/plugin/interact/InteractInfos.hpp>

#include <ofxsImageEffect.h>
#include <ofxsInteract.h>

namespace tuttle {
namespace plugin {
namespace histogramKeyer {

class HistogramKeyerOverlay : public OFX::OverlayInteract
{
	typedef double Scalar;
	typedef OfxRGBColourF Color;
	
public:	
	/*Class arguments*/
	HistogramKeyerPlugin* _plugin;			//plugin reference
	interact::InteractInfos _infos;				
	
	bool _penDown;							//is mouse under selection
	bool _keyDown;							//is key under selection
	OfxPointI _origin;						//begin of the current selection
	OfxPointI _end;							//end of the current selection
	
	bool _isFirstTime; //temporary
	
	/*temporary when Nuke doesn't work*/
	HSLOverlay _hslParam;
	RGBOverlay _rgbParam;

	/*Creator*/
	HistogramKeyerOverlay( OfxInteractHandle handle, OFX::ImageEffect* effect );
	~HistogramKeyerOverlay();
	
	bool draw( const OFX::DrawArgs& args );	//draw all of the component (main display)
	
	/*Mouse management*/
	bool penDown( const OFX::PenArgs& args );	//mouse/pen is clicking down
	bool penUp( const OFX::PenArgs& args );		//mouse/pen is releasing
	bool penMotion( const OFX::PenArgs& args );	//mouse/pen is under motion
	
	/*Keyboard management*/
	bool keyDown( const OFX::KeyArgs& args );	//Ctrl key is pressing down
	bool keyUp( const OFX::KeyArgs& args );		//Ctrl key is releasing 
	
	/*Selection help (display)*/
	void displaySelectedAreas( const OfxPointI imgSize );	//display the selected areas (texture)
	void displaySelectionZone();	//display the current selection zone (white square)
	
	/*Get overlay data*/
	OverlayData& getData();
};

class RGBParamOverlayDescriptor : public OFX::ParamInteractDescriptor
{
public:
	OFX::Interact* createInstance( OfxInteractHandle handle, OFX::ImageEffect* effect )
	{
		//return new RGBOverlay( handle, effect ); (when Nuke overlay works)
		return NULL; //temporary
	}
};

class HSLParamOverlayDescriptor : public OFX::ParamInteractDescriptor
{
public:
	OFX::Interact* createInstance( OfxInteractHandle handle, OFX::ImageEffect* effect )
	{
		//return new HSLOverlay( handle, effect ); (when Nuke overlay works)
		return NULL; //temporary
	}
};

class HistogramKeyerOverlayDescriptor : public OFX::EffectOverlayDescriptor
{
public:
	OFX::Interact* createInstance( OfxInteractHandle handle, OFX::ImageEffect* effect )
	{
		return new HistogramKeyerOverlay( handle, effect );
	}
};

}
}
}

#endif
