#ifndef __CUTSCENEPLAYER_H__
#define __CUTSCENEPLAYER_H__

#include "Module.h"
#include "Point.h"
#include <queue>
#include "SDL/include/SDL_audio.h"

#include "SDL/include/SDL.h"
#include "SDL/include/SDL_thread.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libswresample/swresample.h>
}
struct SDL_Texture;

class CutscenePlayer : public Module
{
public:

	CutscenePlayer(bool enabled = true);

	virtual ~CutscenePlayer();

	bool Awake(pugi::xml_node& conf);

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	bool OpenCodecContext(int* index);
	bool OpenVideoCodecContext(int index);
	bool OpenAudioCodecContext(int index);
	bool ConvertPixels(int videoIndex, int audioIndex);
	bool AllocImage(AVFrame* dstFrame);
	void RenderCutscene();
	void SelectCharacter();
	void ProcessAudio();

public:
	int streamIndex = -1;
	AVFormatContext* formatContext;
	AVCodecContext* videoCodecContext;
	AVCodecContext* audioCodecContext;

	SDL_AudioDeviceID audioDevice;
	int audioStreamIndex;

	SDL_Texture* renderTexture;

	SDL_Rect renderRect;
	bool running;

	SDL_Rect rect1 = { 300,100,250,500 };
	SDL_Rect rect2 = { 600,100,250,500 };
	iPoint position1 = { 300,500 };
	iPoint position2 = { 600,500 };
	bool isHover1 = false;
	bool isHover2 = false;

	std::queue<AVPacket> audioBuffer;
};

#endif // __CUTSCENEPLAYER_H__