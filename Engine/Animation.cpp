#include "Animation.h"

Animation::Animation(int x, int y, int width, int height, int count, 
	const Surface & sprite, float holdTime)
	:
	sprite(sprite),
	holdTime(holdTime)
{
	for (int i = 0; i < count; i++)
	{
		frames.emplace_back(x + i * width, x + (i + 1)*width, y, y + height);
	}
}

void Animation::Draw(const Vei2 & pos, Graphics & gfx) const
{
	gfx.DrawSprite(pos.x, pos.y, frames[iCurFrame], sprite, SpriteEffect::AlphaBlend{});
}

void Animation::Draw(const Vei2 & pos, Graphics & gfx, const RectI & clip) const
{
	gfx.DrawSprite(pos.x, pos.y, frames[iCurFrame], clip, sprite, SpriteEffect::AlphaBlend{});
}

void Animation::Update(float dt)
{
	curFrameTime += dt;
	while(curFrameTime > holdTime)
	{
		Advance();
		curFrameTime -= holdTime;
	}
}

void Animation::Advance()
{
	if (++iCurFrame >= frames.size())
	{
		iCurFrame = 0;
	}
}
