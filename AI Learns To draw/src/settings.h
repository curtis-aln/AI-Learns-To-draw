#pragma once


struct VideoSettings
{
	inline static const std::string save_file = "video.mp4";
	inline static const size_t video_length_seconds = 20;
	inline static const size_t frames_per_second = 250;
	inline static const size_t total_frames = video_length_seconds * frames_per_second;
};


struct MutationSettings
{
	inline static constexpr float mutation_rate    = 0.1f;
	inline static constexpr float mutation_range   = 0.3f;
	inline static constexpr float creation_rate    = 0.1f;
	inline static constexpr float destruction_rate = 0.1f;

	inline static const sf::Uint8 col_range = 100;
	inline static constexpr float pos_range = 80.f;
};


struct EvolutionSettings
{
	inline static constexpr int generations = VideoSettings::total_frames;
	inline static constexpr int population_size = 60;
};


struct CanvasSettings
{
	inline static constexpr int init_triangles = 0;

	// todo
	inline static constexpr bool enable_transparency = true;
	inline static constexpr bool multi_colored_triangles = true;
};