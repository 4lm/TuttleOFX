#ifndef _ofxsProcessing_h_
#define _ofxsProcessing_h_

/*
 * OFX Support Library, a library that skins the OFX plug-in API with C++ classes.
 * Copyright (C) 2005 The Open Effects Association Ltd
 * Author Bruno Nicoletti bruno@thefoundry.co.uk
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * Neither the name The Open Effects Association Ltd, nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The Open Effects Association Ltd
 * 1 Wardour St
 * London W1D 6PA
 * England
 *
 *
 *
 */

#include "ofxsImageEffect.h"
#include "ofxsMultiThread.h"

/** @file This file contains a useful base class that can be used to process images
 *
 * The code below is not so much a skin on the base OFX classes, but code used in implementing
 * specific image processing algorithms. As such it does not sit in the support include lib, but in
 * its own include directory.
 */

namespace OFX {

////////////////////////////////////////////////////////////////////////////////
// base class to process images with
class ImageProcessor : public OFX::MultiThread::Processor
{
protected:
	OFX::ImageEffect& _effect;      /**< @brief effect to render with */
	OFX::Image* _dstImg;       /**< @brief image to process into */
	OfxRectI _renderWindow; /**< @brief render window to use */

public:
	/** @brief ctor */
	ImageProcessor( OFX::ImageEffect& effect )
		: _effect( effect ),
		_dstImg( 0 )
	{
		_renderWindow.x1 = _renderWindow.y1 = _renderWindow.x2 = _renderWindow.y2 = 0;
	}

	/** @brief set the destination image */
	void setDstImg( OFX::Image* v ) { _dstImg = v; }

	/** @brief reset the render window */
	void setRenderWindow( OfxRectI rect ) { _renderWindow = rect; }

	/** @brief overridden from OFX::MultiThread::Processor. This function is called once on each SMP thread by the base class */
	void multiThreadFunction( unsigned int threadId, unsigned int nThreads )
	{
		// slice the y range into the number of threads it has
		unsigned int dy = _renderWindow.y2 - _renderWindow.y1;

		unsigned int y1 = _renderWindow.y1 + threadId * dy / nThreads;

		int step        = ( threadId + 1 ) * dy / nThreads;
		unsigned int y2 = _renderWindow.y1 + ( step < dy ? step : dy );

		OfxRectI win = _renderWindow;

		win.y1 = y1; win.y2 = y2;

		// and render that thread on each
		multiThreadProcessImages( win );
	}

	/** @brief called before any MP is done */
	virtual void preProcess( void ) {}

	/** @brief this is called by multiThreadFunction to actually process images, override in derived classes */
	virtual void multiThreadProcessImages( OfxRectI window ) = 0;

	/** @brief called before any MP is done */
	virtual void postProcess( void ) {}

	/** @brief called to process everything */
	virtual void process( void )
	{
		// is it OK ?
		if( !_dstImg || ( _renderWindow.x2 - _renderWindow.x1 == 0 && _renderWindow.y2 - _renderWindow.y1 ) )
			return;

		// call the pre MP pass
		preProcess();

		// call the base multi threading code, should put a pre & post thread calls in too
		multiThread();

		// call the post MP pass
		postProcess();
	}

};

};
#endif
