#include "ICodec.hpp"

extern "C" {
#include <libavutil/mem.h>
}

#include <stdexcept>
#include <cassert>
#include <iostream>

namespace avtranscoder {

ICodec::ICodec( const ECodecType type, const std::string& codecName )
	: _avCodecContext( NULL )
	, _avCodec( NULL )
	, _type( type )
{
	setCodec( type, codecName );
	allocateContext();
	loadCodecOptions();
}

ICodec::ICodec( const ECodecType type, const AVCodecID codecId )
	: _avCodecContext( NULL )
	, _avCodec( NULL )
	, _type( type )
{
	setCodec( type, codecId );
	allocateContext();
	loadCodecOptions();
}

ICodec::~ICodec()
{
	avcodec_close( _avCodecContext );
	av_free( _avCodecContext );
	_avCodecContext = NULL;
}

std::string ICodec::getCodecName() const
{
	assert( _avCodec != NULL );
	return avcodec_descriptor_get( _avCodec->id )->name;
}

AVCodecID ICodec::getCodecId() const
{
	assert( _avCodec != NULL );
	return _avCodec->id;
}

int ICodec::getLatency()  const
{
	assert( _avCodecContext != NULL );
	return _avCodecContext->delay;
}

std::vector<Option> ICodec::getOptions()
{
	std::vector<Option> optionsArray;
	for( OptionMap::iterator it = _options.begin(); it != _options.end(); ++it )
	{
		optionsArray.push_back( it->second );
	}
	return optionsArray;
}

void ICodec::setCodec( const ECodecType type, const std::string& codecName )
{
	const AVCodecDescriptor* avCodecDescriptor = avcodec_descriptor_get_by_name( codecName.c_str() );
	if( ! avCodecDescriptor )
	{
		std::string msg( "unable to find codec " );
		msg += codecName;
		throw std::runtime_error( msg );
	}

	setCodec( type, avCodecDescriptor->id );
}

void ICodec::setCodec( const ECodecType type, const AVCodecID codecId )
{
	if( codecId == 0 )
	{
		std::cout << "Warning: Unsupported codec with id 0" << std::endl;
		return;
	}

	if( type == eCodecTypeEncoder )
	{
		_avCodec = avcodec_find_encoder( codecId );
		if( _avCodecContext )
			_avCodecContext->codec = _avCodec;
	}
	else if( type == eCodecTypeDecoder )
	{
		_avCodec = avcodec_find_decoder( codecId );
		if( _avCodecContext )
			_avCodecContext->codec = _avCodec;
	}
}


void ICodec::allocateContext()
{
	_avCodecContext = avcodec_alloc_context3( _avCodec );
	if( ! _avCodecContext )
	{
		throw std::runtime_error( "unable to allocate the codecContext and set its fields to default values" );
	}
}

void ICodec::loadCodecOptions()
{
	if( _type == eCodecTypeEncoder )
		loadOptions( _options, _avCodecContext, AV_OPT_FLAG_ENCODING_PARAM );
	else if( _type == eCodecTypeDecoder )
		loadOptions( _options, _avCodecContext, AV_OPT_FLAG_DECODING_PARAM );
}

}
