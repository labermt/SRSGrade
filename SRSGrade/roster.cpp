#include "stdafx.h"
#include <algorithm>
#include <optional>
#include <gsl_assert.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "roster.h"


// Example Roster:
//
// {
//     "roster":
//     [
//         {
//             "id":     "Pikachu", 
//             "email":  "stuart.dent@oit.edu",
//             "name":   "Stu Dent", 
//             "github": "studentRepo"
//         },
//         {
//             "id":     "Charmander", 
//             "email":  "sandra.wich@oit.edu",
//             "name":   "Sand Wich", 
//             "github": "sandwichRepo"
//         }
//     ]
// }

// Schema:
// {
//     "$schema": "http://json-schema.org/draft-06/schema#",
//     "title": "Roster",
//     "description": "A list of students registered for a course",
//     "type": "object",
//      "properties":
//      {
//          "id": 
//          {
//              "description": "A private and unique identifier for the student",
//              "type": "string"
//          },
//          "email": 
//          {
//              "description": "The student's preferred email address",
//              "type": "string"
//          },
//          "name":
//          {
//              "description": "The student's preferred name",
//              "type": "string"
//          },
//          "github":
//          {
//              "description": "The student's github account name",
//              "type": "string"
//          }
//      },
//      "required": ["id", "email", "name", "github"]
// }


Roster::StudentRecord::StudentRecord(const std::string id, const std::string email, const std::string name, const std::string github):
	id_{ id }, email_{ email }, name_{ name }, github_{ github }
{
}

std::string Roster::StudentRecord::getEmail() const
{
	return email_;
}

std::string Roster::StudentRecord::getName() const
{
	return name_;
}

Roster::Roster(const std::string roster_filename_json)
{
	boost::property_tree::ptree root;

	boost::property_tree::read_json(roster_filename_json, root);

	for (const boost::property_tree::ptree::value_type& student_pair : root.get_child("roster"))
	{
		const auto& [key, student_entry]{student_pair};

		// TODO: Is there a way to get all values in one shot?
		const auto& id{ student_entry.get<std::string>("id") };
		const auto& email{ student_entry.get<std::string>("email") };
		const auto& name{ student_entry.get<std::string>("name") };
		const auto& github{ student_entry.get<std::string>("github") };

		const StudentRecord student_record(id, email, name, github);

		roster_.emplace_back(student_record);
	}
};

std::optional<const Roster::StudentRecord> Roster::find_name(const std::string student_name) const
{
    const auto match_lambda = [student_name](const StudentRecord& student_record){ return student_record.getName() == student_name; };
    const auto iter
    {
        std::find_if
        (
            roster_.begin(), 
            roster_.end(), 
            match_lambda
        )
    };

	Expects(iter != roster_.end());

	std::optional<const Roster::StudentRecord> result(*iter);

    return result;
}

bool Roster::is_email_good(const std::string student_email) const
{
    const auto match_lambda = [student_email](const StudentRecord& student_record){ return student_record.getEmail() == student_email; };
    const auto iter
    {
        std::find_if
        (
            roster_.begin(), 
            roster_.end(), 
            match_lambda
        )
    };

    const auto result{iter != roster_.end()};

    return result;
}
