#include "stdafx.h"
#include <algorithm>
#include <optional>
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
//             "name" :  "Stu Dent", 
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


Roster::Roster(const std::string roster_filename_json)
{
    boost::property_tree::ptree root;

    boost::property_tree::read_json(roster_filename_json, root);

    for (const boost::property_tree::ptree::value_type& student: root.get_child("roster"))
    {
        roster_.emplace_back(student.second.get_value<Student>());
    }
};

std::optional<Student> Roster::find_name(const std::string name) const
{
    const auto match_lambda{ [name](const Student& student){ return student.getName() == name; } };
    const auto iter
    {
        std::find_if
        (
            roster_.begin(), 
            roster_.end(), 
            match_lambda
        )
    };

    auto std::optional<Student> result;
    if (iter != roster.end())
    {
        result = *iter;
    }

    return result;
}

bool Roster::is_email_good(const std::string email)
{
    const auto match_lambda{ [email](const Student& student){ return student.getEmail() == email; } };
    const auto iter
    {
        std::find_if
        (
            roster_.begin(), 
            roster_.end(), 
            match_lambda
        )
    };

    const auto result{iter != roster.end()};

    return result;
}

