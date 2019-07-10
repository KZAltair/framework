#pragma once
#include "Colors.h"
#include "Graphics.h"

namespace SpriteEffect
{
	class AlphaBlend
	{
	public:
		void operator()(Color src, int xDest, int yDest, Graphics& gfx) const
		{
			const Color dst = gfx.GetPixel(xDest, yDest);
			// pre-extract alpha
			const int alpha = src.GetA();
			// complement of alpha
			const int cAlpha = 255 - alpha;
			// blend channels by linear interpolation using integer math
			// (basic idea: src * alpha + dst * (1.0 - alpha), where alpha is from 0 to 1
			// we divide by 256 because it can be done with bit shift
			// it gives us at most 0.4% error, but this is negligible
			const unsigned char r = (src.GetR() * alpha + dst.GetR() * cAlpha) / 256;
			const unsigned char g = (src.GetG() * alpha + dst.GetG() * cAlpha) / 256;
			const unsigned char b = (src.GetB() * alpha + dst.GetB() * cAlpha) / 256;
			// write the resulting interpolated color to the screen
			gfx.PutPixel(xDest, yDest, { r,g,b });
		}
	};
}