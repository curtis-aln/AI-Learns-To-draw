#pragma once


struct VideoSettings
{
	inline static const std::string save_file = "video.mp4";
	inline static const size_t video_length_seconds = 5;
	inline static const size_t frames_per_second = 50;
	inline static const size_t total_frames = video_length_seconds * frames_per_second;
};


struct MutationSettings
{
	inline static constexpr float mutation_rate = 0.08f;
	inline static constexpr float mutation_range = 1.f;
	inline static constexpr float creation_rate = 0.08f;
	inline static constexpr float destruction_rate = 0.07f;
};