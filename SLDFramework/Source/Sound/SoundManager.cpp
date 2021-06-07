#include "SoundManager.h"
#include <fmod.hpp>
#include <fmod_errors.h>


class SLD::SoundManager::ImplSoundManager
{
public:

	ImplSoundManager();

	int PlayStream(
		const std::string& filePath,
		float volume,
		float pitch,
		bool shouldLoop) const;

	int PlayStream(
		const char* filePath,
		float volume,
		float pitch,
		bool shouldLoop) const;

	bool IsPlaying(int channelId) const;

private:

	RefPtr<FMOD::System> m_FMODSystem{};
	uint32_t m_ChannelsCnt{ 18 };
};

SLD::SoundManager::ImplSoundManager::ImplSoundManager()
{
	FMOD::System* system{};

	auto result{ FMOD::System_Create(&system) };
	if (result != FMOD_OK)
		throw std::runtime_error(FMOD_ErrorString(result));

	result = system->init(int(m_ChannelsCnt), FMOD_INIT_NORMAL, nullptr);

	if (result != FMOD_OK)
		throw std::runtime_error(FMOD_ErrorString(result));

	m_FMODSystem = RefPtr<FMOD::System>{ system,[](FMOD::System* ptr)
	{
		ptr->release();
		ptr = nullptr;
	} };
}

int SLD::SoundManager::ImplSoundManager::PlayStream(const std::string& filePath, float volume, float pitch,
	bool shouldLoop) const
{
	int channelId{};
	FMOD::Sound* sound{};
	FMOD::Channel* channel{};
	int loopCount{ shouldLoop ? -1 : 0 };

	auto result{ m_FMODSystem->createStream(filePath.c_str(), FMOD_LOOP_OFF | FMOD_2D | FMOD_CREATESTREAM,
	nullptr,&sound) };

	if (result == FMOD_OK)
	{
		m_FMODSystem->playSound(sound, nullptr, true, &channel);
		channel->setLoopCount(loopCount);
		channel->setVolume(volume);
		channel->setPitch(pitch);
		channel->getIndex(&channelId);
		channel->setPaused(false);
	}

	return channelId;
}

int SLD::SoundManager::ImplSoundManager::PlayStream(const char* filePath, float volume, float pitch,
	bool shouldLoop) const
{
	int channelId{};
	FMOD::Sound* sound{};
	FMOD::Channel* channel{};
	int loopCount{ shouldLoop ? -1 : 0 };

	auto result{ m_FMODSystem->createStream(filePath, FMOD_LOOP_OFF | FMOD_2D | FMOD_CREATESTREAM,
	nullptr,&sound) };

	if (result == FMOD_OK)
	{
		m_FMODSystem->playSound(sound, nullptr, true, &channel);
		channel->setLoopCount(loopCount);
		channel->setVolume(volume);
		channel->setPitch(pitch);
		channel->getIndex(&channelId);
		channel->setPaused(false);
	}

	return channelId;
}

bool SLD::SoundManager::ImplSoundManager::IsPlaying(int channelId) const
{
	FMOD::Channel* channel{};
	auto result = m_FMODSystem->getChannel(channelId, &channel);

	if (result != FMOD_OK)
		throw std::runtime_error(FMOD_ErrorString(result));
	
	bool isLooping{};
	channel->isPlaying(&isLooping);
	return isLooping;
}

SLD::SoundManager::~SoundManager() = default;

[[maybe_unused]] int SLD::SoundManager::PlayStream(const std::string & filePath, float volume, float pitch, bool shouldLoop) const
{
	return m_pSoundManager->PlayStream(filePath, volume, pitch, shouldLoop);
}

[[maybe_unused]] int SLD::SoundManager::PlayStream(const char* filePath, float volume, float pitch, bool shouldLoop) const
{
	return m_pSoundManager->PlayStream(filePath, volume, pitch, shouldLoop);
}


bool SLD::SoundManager::IsPlaying(int channelId) const
{
	return m_pSoundManager->IsPlaying(channelId);
}

SLD::SoundManager::SoundManager()
	: m_pSoundManager(std::make_unique<ImplSoundManager>())
{
}
