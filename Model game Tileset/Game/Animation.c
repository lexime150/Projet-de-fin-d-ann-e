//#include "Animation.h"
//
//Animation CreateAnimation(sfSprite* _sprite, unsigned _frameCount, unsigned _frameRate, sfIntRect _firstFrame, sfBool _isLooping)
//{
//	Animation newAnimation = { 0 };
//
//	newAnimation.sprite = _sprite;
//	newAnimation.frameCount = _frameCount;
//	newAnimation.frameRate = _frameRate;
//	newAnimation.firstFrame = _firstFrame;
//	newAnimation.isLooping = _isLooping;
//
//	return newAnimation;
//}
//
//void UpdateAnimation(Animation* const _animation, float _dt)
//{
//	if (!_animation->isFinished)
//	{
//		// Update the frame
//		_animation->timer += _dt;
//		if (_animation->timer > 1.f / _animation->frameRate)
//		{
//			_animation->timer -= 1.f / _animation->frameRate;
//			_animation->frameNumber++;
//
//			// End of the animation
//			if (_animation->frameNumber == _animation->frameCount)
//			{
//				// Animation is looping
//				if (_animation->isLooping)
//				{
//					_animation->frameNumber = 0;
//				}
//				else
//				{
//					_animation->frameNumber--;
//					_animation->isFinished = sfTrue;
//				}
//			}
//		}
//
//		// Select a frame
//		sfIntRect selectedFrame = _animation->firstFrame;
//		selectedFrame.left += _animation->frameNumber * selectedFrame.width;
//		sfSprite_setTextureRect(_animation->sprite, selectedFrame);
//	}
//}
//
//void LoadAnimation(Animation* const _animation)
//{
//	_animation->frameNumber = 0;
//	_animation->timer = 0.f;
//	_animation->isFinished = sfFalse;
//}
//
//sfBool AnimationIsFinished(const Animation* const _animation)
//{
//	return _animation->isFinished;
//}