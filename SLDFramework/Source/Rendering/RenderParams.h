#ifndef SLDFRAMEWORK_RENDERPARAMS_H
#define SLDFRAMEWORK_RENDERPARAMS_H

#include <cstdint>
#include "../Miscellaneous/SFMLPrerequisite.h"

#ifdef HAS_DX12

enum class Dx12RenderElement
{
	SetPipelineStateObject,
	SetGraphicsRootSignature,
	SetGraphicsRootDescriptorTable,
	SetGraphiscRoot32BitConstants,
	SetPrimitiveTopology,
	SetVertexBuffer,
	SetIndexBuffer,
	SetViewport,
	SetScissorRects,
	DrawIndexedInstanced
};

#endif

#ifdef HAS_SFML

enum class SFMLRenderElement
{
	RenderShapes,
	RenderSprite,
	RenderTexture,
	RenderStates,
	RenderText
};

#endif

namespace SLD
{
	using RenderIdentifier = uint16_t;
}

#endif