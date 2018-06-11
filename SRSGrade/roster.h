#ifndef SRSGRADE_ROSTER_H
#define SRSGRADE_ROSTER_H

#include <vector>
#include <optional>

class Roster
{
private:
    class StudentRecord
    {
    private:
        const std::string id_;
        const std::string email_;
        const std::string name_;
        const std::string github_;

    public:
		StudentRecord(const std::string id, const std::string email, const std::string name, const std::string github);
		std::string getEmail() const;
		std::string getName() const;
	};

    std::vector<StudentRecord> roster_{};

public:
    explicit Roster(const std::string roster_filename_json);

    std::optional<const StudentRecord> find_name(const std::string student_name) const;
    bool is_email_good(const std::string student_email) const;
};

#endif // SRSGRADE_ROSTER_H

