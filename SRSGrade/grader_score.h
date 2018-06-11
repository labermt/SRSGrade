#ifndef SRSGRADE_GRADER_SCORE_H
#define SRSGRADE_GRADER_SCORE_H

#include <vector>

class GraderScore
{
private:
    unsigned instructor_{ 0 };
    unsigned self_{ 0 };
    std::vector<unsigned> peer_;

public:
    unsigned getInstructor() const;
    unsigned getSelf() const;
    std::vector<unsigned> getPeer() const;

    void setInstructor(const unsigned value);
    void setSelf(const unsigned value);            
	void addPeer(const unsigned value);
};

#endif // SRSGRADE_GRADER_SCORE_H

