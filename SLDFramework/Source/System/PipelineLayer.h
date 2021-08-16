#pragma once

namespace SLD
{
	enum class PipelineLayer
	{
		OnPreAsync,
		OnAsync,
		OnPostAsync,
		OnFixedUpdate,
		OnPostPhysicsStep,
		OnPreRender,
		OnRender,

		Count
	};
}
