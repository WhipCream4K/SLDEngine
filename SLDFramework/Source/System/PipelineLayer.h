#pragma once

namespace SLD
{
	enum class PipelineLayer
	{
		OnPreAsync,
		OnAsync,
		OnPostAsync,
		OnFixedUpdate,
		OnFixedUpdateAsync,
		OnPrePhysicsStep,
		OnPostPhysicsStep,
		OnPreRender,
		OnRender,

		Count
	};
}
