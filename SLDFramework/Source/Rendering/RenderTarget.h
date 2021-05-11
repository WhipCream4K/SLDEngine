
#ifndef SLDFRAMEWORK_RENDERTARGET_H
#define SLDFRAMEWORK_RENDERTARGET_H

namespace SLD
{
	struct RenderTarget
	{
		RenderTarget(uint32_t rWidth, uint32_t rHeight, const std::any& rtv)
			: width(rWidth)
			, height(rHeight)
			, handle(rtv)
		{
		}

		RenderTarget() = default;

		uint32_t width{};
		uint32_t height{};
		std::any handle{};
	};
}


#endif
