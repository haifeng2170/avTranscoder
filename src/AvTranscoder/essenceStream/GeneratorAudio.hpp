#ifndef _AV_TRANSCODER_ESSENCE_STREAM_GENERATOR_AUDIO_HPP_
#define _AV_TRANSCODER_ESSENCE_STREAM_GENERATOR_AUDIO_HPP_

#include "IInputEssence.hpp"

#include <AvTranscoder/common.hpp>
#include <AvTranscoder/codedStructures/AudioDesc.hpp>
#include <AvTranscoder/essenceStructures/Frame.hpp>

namespace avtranscoder
{

class AvExport GeneratorAudio : public IInputEssence
{
public:
	GeneratorAudio( );

	~GeneratorAudio( );

	// Stream properties
	void setAudioDesc( const AudioDesc& audioDesc );

	AudioDesc getAudioDesc() const;
	
	void setup() {}
	
	void setFrame( Frame& inputFrame );

	bool readNextFrame( Frame& frameBuffer );
	bool readNextFrame( Frame& frameBuffer, const size_t subStreamIndex );

private:
	AudioDesc      _audioDesc;
	AudioFrameDesc _frameDesc;
	
	Frame*         _inputFrame;
};

}

#endif