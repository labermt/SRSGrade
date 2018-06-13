#include "stdafx.h"
#include <limits>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <numeric>
#include <gsl.h>
#include "boost/functional/hash.hpp"
#include "roster.h"
#include "grader_score.h"
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

int cout_score(const Roster& roster)
{
	using namespace std::literals::string_literals;

	const auto name{ "CST136SRS00"s };
	const auto filename{ name + ".csv"s };

	std::ifstream ifstrm(filename);

	Expects(ifstrm.good());
	ifstrm.ignore(std:: numeric_limits<std::streamsize>::max(), '\n');

	auto srs_table{ std::vector<std::string>() };
	while (ifstrm.good() && !std::cin.eof())
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
			const auto result{ hash_val(key.getGraderEmail(), key.getStudentName()) };
			return result;
		};

	std::unordered_map<Grade::Key, const Grade, decltype(hash)> grade_sheet(32768, hash);

	for (const auto& srs_record : srs_table)
	{
		auto iss{ std::istringstream(srs_record) };

		Grade grade(roster);

		iss >> grade;

		const auto key{ grade.getKey() };
		const auto found_iter{ grade_sheet.find(key) };

		const auto value{ std::make_pair(key, grade) };

		// CRUD
		if (found_iter == grade_sheet.end()) // create
		{
			const auto success{ grade_sheet.insert(value).second };
			Ensures(success);
		}
		else // update
		{
			const auto [existing_key, existing_grade] { *found_iter }; // read
			if (existing_grade.getTimestamp() < grade.getTimestamp())
			{
				grade_sheet.erase(found_iter); // delete
				assert(false); // TODO: Check this code works, then remove this line. 
				const auto success{ grade_sheet.insert(value).second };
				Ensures(success);
			}
		}
	}

	std::map<std::string, GraderScore> score;

	for (const auto& grade_record : grade_sheet)
	{
		const auto& [key, grade] { grade_record };

		const auto grader_email_key{ key.getGraderEmail() };
		Ensures(!grader_email_key.empty());

		const auto student_name_key{ key.getStudentName() };
		const auto optional_student_record{ roster.find_name(student_name_key) };
		Expects(optional_student_record.has_value());
		const auto student_record{ optional_student_record.value() };
		const auto student_email{ student_record.getEmail() };

		const auto score_value{ grade.getScore() };
		auto& grader_score{ score[student_name_key] };
        
        if (grader_email_key.compare("labermt@gmail.com"s) == 0)
        {
            Expects(grader_score.getInstructor() == 0);
            grader_score.setInstructor(score_value);
        }
        else if (grader_email_key.compare(student_email) == 0)
        {
            Expects(grader_score.getSelf() == 0);
            grader_score.setSelf(score_value);            
        }
        else
        {
    		grader_score.addPeer(grade.getScore());
        }
	}

    const char* delimiter{ "" };

	std::cout << "{" "\n"
		"\t" "\"grade\": [" "\n";
	for (const auto& student_score: score)
	{
        static const std::string quote{ R"(")" };

		const auto& [student_name, grader_score]{ student_score };
        const auto instructor_score{ grader_score.getInstructor() };
        const auto self_score{ grader_score.getSelf() };
        const auto& peer_score{ grader_score.getPeer() };

        const auto peer_size{ peer_score.size() };
		double peer_mean_score{ 0.0 };
		if (peer_size != 0)
		{
			const auto quarter_size{ peer_size / 4 };
			const auto quarter_begin{ peer_score.cbegin() + quarter_size };
			const auto quarter_end{ peer_score.cend() - quarter_size };

			peer_mean_score = 1.0 * std::accumulate(quarter_begin, quarter_end, 0) / std::distance(quarter_begin, quarter_end);
		}
        
		std::cout << delimiter <<
            "\t" "{" << "\n" << 
                "\t\t" << quote << student_name << quote << ": " << "\n" <<
                "\t\t" << "{" << "\n" <<
                "\t\t\t" << quote << "instructor" << quote << ": " << instructor_score << ", " << "\n" <<
                "\t\t\t" << quote << "self"       << quote << ": " << self_score       << ", " << "\n" <<
                "\t\t\t" << quote << "peer"       << quote << ": " << peer_mean_score  <<         "\n" <<
                "\t\t" << "}" << "\n" <<
			"\t" "}";

        delimiter = ", \n"; 
	}
	std::cout << "\n" "\t" "]" "\n"
		"}" << std::endl;

	return 0;
}

void help()
{
    std::cout <<
        "syntax: SRSGrade [roster_filename.json] [<CST000SRS00.csv] \n"
        "example: $ SRSGrade cst000roster.json <CST000SRS00.csv \n" <<
        std::endl;
}

int main(int argc, char* argv[])
{
	auto result = 1;
    if (argc!=2)
    {
        help();
    }
    else
    {
        const Roster roster(argv[1]);
        result = cout_score(roster);
    }
    return result;
}
