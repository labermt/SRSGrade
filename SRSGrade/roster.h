#ifndef SRSGRADE_ROSTER_H
#define SRSGRADE_ROSTER_H

#include <vector>

class Roster
{
private:
    class Student
    {
    private:
        const std::string id_;
        const std::string email_;
        const std::string name_;
        const std::string github_;
    };

    std::vector<Student> roster_;

public:
    explicit Roster(const std::string roster_filename_json);

    std::optional<Student> find_name(const std::string name) const;
    bool is_email_good(const std::string email) const;
};

#endif // SRSGRADE_ROSTER_H

