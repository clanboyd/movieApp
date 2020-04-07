#ifndef PEOPLE_H_
#define PEOPLE_H_

#include <string>
#include <vector>
#include "Definitions.h"

using namespace std;

class People
{
public:
	People(std::string inName, std::string inPersonId, std::string inMovieId);
	~People();
	string GetName();
	string GetCardId();
	std::vector<std::string> GetCheckedOut();

protected:
	std::string mName;
	std::string mRfId;
	std::vector<std::string> mCheckedOut;
};

#endif /* PEOPLE_H_ */
