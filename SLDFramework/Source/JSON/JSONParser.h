
#ifndef SLDFRAMEWORK_JSONPARSER_H
#define SLDFRAMEWORK_JSONPARSER_H

#include "../Core/Base.h"

namespace SLD
{
	class JSONParser
	{
	public:
		class ImplJSONParser;
		JSONParser();
		~JSONParser();

	
	private:
		OwnedPtr<ImplJSONParser> m_pImplJSONParser;
	};
}


#endif
