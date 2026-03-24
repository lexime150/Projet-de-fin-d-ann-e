#ifndef ANIMATION_H
#define ANIMATION_H

#include "Common.h"


typedef struct Animation
{
	sfSprite* sprite;
	unsigned frameSpeed;
	unsigned frameCount;
	unsigned currentFrame;
	float timer;
	sfIntRect firstFrame;
	sfBool isPlaying;
	sfBool isLooping;

}Animation;


Animation CreateAnimation(sfSprite* _sprite, unsigned _frameCount, unsigned _frameSpeed, sfBool _isPlaying, sfBool _isLooping, sfIntRect _firstFrame);
void UpdateAnimation(Animation* _animation, float _dt);


#endif 