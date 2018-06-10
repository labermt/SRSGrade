#include "stdafx.h"
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <numeric>
#include "boost/functional/hash.hpp"
#include "grade.h"

namespace
{
	// Cargo Culted From: The C++ Standard Libary by Nicolai M. Josuttis, p365

	template<typename T>
	void hash_val(std::size_t& seed, const T& val)
	{
		boost::hash_combine(seed, val);
	}

	template<typename T, typename... Types>
	void hash_val(std::size_t& seed, const T& val, const Types&... args)
	{
		boost::hash_combine(seed, val);
		hash_val(seed, args...);
	}

	template<typename... Types>
	std::size_t hash_val(const Types&... args)
	{
		std::size_t seed{ 0 };
		hash_val(seed, args...);
		return seed;
	}
}

int main()
{
	using namespace std::literals::string_literals;

	const auto name{ "CST136SRS00"s };
	const auto filename{ name + ".csv"s };

	std::ifstream ifstrm(filename);
	Ensures(ifstrm.good());

	auto srs_table{ std::vector<std::string>() };
	while (ifstrm.good() && !ifstrm.eof())
	{
		auto line{ std::string() };
		getline(ifstrm, line);
		Ensures(ifstrm.good() || ifstrm.eof());
		if (!line.empty())
		{
			srs_table.emplace_back(line);
		}
	}

	const auto hash =
		[]
		(const Grade::Key key)
		{
			auto result{ hash_val(key.getSrs(), key.getGrader(), key.getStudent()) };
			return result;
		};

	std::unordered_map<Grade::Key, const Grade, decltype(hash)> grade_sheet(32768, hash);

	for (const auto& srs_record : srs_table)
	{
		auto iss{ std::istringstream(srs_record) };

		Grade grade(name);

		iss >> grade;

		const auto key{ grade.getKey() };
		const auto value{ std::make_pair(key, grade) };

		const auto [insert_iter, success] { grade_sheet.insert(value) };

		if (!success)
		{
			const auto [existing_key, existing_grade] { *insert_iter };
			if (existing_grade.getTimestamp() < grade.getTimestamp())
			{
				grade_sheet.erase(insert_iter);
				assert(false); // TODO: Check this code works, then remove this line. 
				const auto replaced{ grade_sheet.insert(value).second };
				Ensures(replaced);
			}
		}
	}

	std::map<std::string, std::vector<unsigned>> score;

	for (const auto& grade: grade_sheet)
	{
		const auto student{ grade.first.getStudent() };
		auto& student_score{ score[student] };
		student_score.push_back(grade.second.getScore());
	}

	for (const auto& student_score: score)
	{
		const auto& [student_name, score_vector] {student_score};
		const auto sum{ std::accumulate(score_vector.cbegin(), score_vector.cend(), 0) };
		std::cout << student_name << ": " << sum / score_vector.size() << std::endl;
	}

	return 0;
}
