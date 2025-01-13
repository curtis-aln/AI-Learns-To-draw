#pragma once


struct VideoSettings
{
	inline static const std::string save_file = "video.mp4";
	inline static const size_t video_length_seconds = 5;
	inline static const size_t frames_per_second = 3;
	inline static const size_t total_frames = video_length_seconds * frames_per_second;
};


struct MutationSettings
{
	inline static constexpr float mutation_rate = 0.3f;
	inline static constexpr float mutation_range = 0.1f;
	inline static constexpr float creation_rate = 0.05f;
	inline static constexpr float destruction_rate = 0.07f;

	inline static const sf::Uint8 col_range = 20;
	inline static constexpr float pos_range = 80.f;
};


struct EvolutionSettings
{
	inline static constexpr int generations = 80;
	inline static constexpr int population_size = 120;
};


struct CanvasSettings
{
	inline static constexpr int init_triangles = 25;
};