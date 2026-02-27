#pragma once
#include "framework.h"
#include "EnemyType.h"
#include <fstream>
#include <sstream>
#include <filesystem>

struct SpawnEvent
{
	float spawnTime = 0.f;									// 스폰 시간(초)
	EnemyType enemyType = EnemyType::NONE;	// 스폰할 적의 유형
	float x = 0.f;
	float y = 0.f;
	float speed = 0.f;											// 적의 이동 속도
	bool spawned = false;										// 이미 스폰되었는지 여부
};

struct Wave
{
	std::vector<SpawnEvent> spawnEvents;
};

class WaveLoader
{
private:
	static EnemyType StringToEnemyType(const std::string& str)
	{
		if (str == "MONSTER")				return EnemyType::MONSTER;
		else if (str == "MOTHERSHIP")	return EnemyType::MOTHERSHIP;
		else if (str == "DRAGON")			return EnemyType::DRAGON;
		else if (str == "GOONS")				return EnemyType::GOONS;
		else											return EnemyType::NONE;
	}

public:
	static std::vector<Wave> GetWaves()
	{
		std::vector<Wave> waves;

		std::string basePath = "GameAssets/wave";
		std::string extension = ".csv";
		
		// wave 파일 개수 구하기
		int fileCount = 0;
		for (const auto& entry : std::filesystem::directory_iterator("GameAssets"))
		{
			if (entry.is_regular_file() && entry.path().extension() == extension && entry.path().stem().string().find("wave") == 0)
			{
				++fileCount;
			}
		}

		// 반복문은 파일 개수만큼 반복
		for (int i = 5; i <= fileCount; ++i)
		{
			std::string path = basePath + std::to_string(i) + extension;
			LoadWavesFromFile(path, waves);
		}

		std::cout << "Wave loaded : " << waves.size() << std::endl;

		return waves;
	}

	static void LoadWavesFromFile(const std::string& path, std::vector<Wave>& waves)
	{
		Wave currentWave;

		std::ifstream file(path);
		if (!file.is_open())
		{
			std::cerr << "Failed to open wave file: " << path << std::endl;
			return;
		}

		std::string line;
		std::getline(file, line); // 헤더 라인 건너뛰기

		while (std::getline(file, line))
		{
			//std::cout << "CSV line: [" << line << "]" << std::endl;

			if (line.empty() || line == "\r")	// 빈 줄 = 다음 웨이브
			{
				if (!currentWave.spawnEvents.empty())
				{
					waves.push_back(currentWave);
					currentWave.spawnEvents.clear();
				}
				continue;
			}

			std::stringstream ss(line);
			std::string token;
			SpawnEvent event;

			std::getline(ss, token, ',');	event.spawnTime = std::stof(token);
			std::getline(ss, token, ',');	event.enemyType = StringToEnemyType(token);
			std::getline(ss, token, ',');	event.x = std::stof(token);
			std::getline(ss, token, ',');	event.y = std::stof(token);
			std::getline(ss, token, ',');	event.speed = std::stof(token);

			currentWave.spawnEvents.push_back(event);
		}

		// 마지막 웨이브 추가
		if (!currentWave.spawnEvents.empty())
			waves.push_back(currentWave);

		file.close();
	}
};