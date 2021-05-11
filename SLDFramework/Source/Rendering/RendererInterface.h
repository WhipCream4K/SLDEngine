

#ifndef SLDFRAMEWORK_RENDERERINTERFACE_H
#define SLDFRAMEWORK_RENDERERINTERFACE_H

#include "../Core/Base.h"
#include "RenderTarget.h"

namespace SLD
{
	class InputComponent;
	class RendererInterface : public std::enable_shared_from_this<RendererInterface>
	{
	public:
		// TODO: Add key events for the users when they want handle something in the renderer via key events
		[[maybe_unused]] virtual bool Init(const RenderTarget& rtv) = 0;
		[[maybe_unused]] virtual bool ClearBackBuffer() = 0;
		[[maybe_unused]] virtual bool Present() = 0;
		virtual ~RendererInterface() = default;

		template<typename RendererType,
		typename = std::enable_if_t<std::is_base_of_v<RendererInterface, RendererType>, RendererType>>
		constexpr auto GetRenderInstance() -> WeakPtr<RendererType>;

		RendererInterface() = default;
		RendererInterface(const RendererInterface&) = delete;
		RendererInterface(RendererInterface&&) = delete;
		RendererInterface& operator=(const RendererInterface&) = delete;
		RendererInterface& operator=(RendererInterface&&) = delete;
	protected:
		WeakPtr<InputComponent> m_InputComponent;
	};

	template <typename RendererType, typename>
	constexpr WeakPtr<RendererType> RendererInterface::GetRenderInstance()
	{
		return std::static_pointer_cast<RendererType>(weak_from_this());
	}
}


#endif