#ifndef SLDFRAMEWORK_LLINPUTS_H
#define SLDFRAMEWORK_LLINPUTS_H

#include "SFMLInputs.h"
#include <variant>

#include "InputParams.h"

namespace SLD
{
	using LLInputs = std::variant<SFMLInputs>;
	
	struct LLInputRead
	{
		InputParams::ReadOut operator()(SFMLInputs& llRead) const
		{
			return llRead.ReadInputs();
		}
	};
	
}

#endif