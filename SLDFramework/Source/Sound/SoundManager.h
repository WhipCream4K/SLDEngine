
#ifndef SLDFRAMEWORK_SOUNDMANAGER_H
#define SLDFRAMEWORK_SOUNDMANAGER_H

#include "../Core/Base.h"
#include "../Core/Singleton.h"

namespace SLD
{
	class SoundManager final : public Singleton<SoundManager>
	{

		friend class Singleton<SoundManager>;

	public:

		class ImplSoundManager;

		SoundManager();
		~SoundManager() override;

		// returns channel id
		[[maybe_unused]] int PlayStream(
			const std::string& filePath,
			float volume = 1.0f,
			float pitch = 1.0f,
			bool shouldLoop = false) const;

		// returns channel id
		[[maybe_unused]] int PlayStream(
			const char* filePath,
			float volume = 1.0f,
			float pitch = 1.0f,
			bool shouldLoop = false) const;

		bool IsPlaying(int channelId) const;

	private:

		OwnedPtr<ImplSoundManager> m_pSoundManager;
	};
}


#endif
