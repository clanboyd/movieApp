#ifndef MOVIEDB_H_
#define MOVIEDB_H_

#include <iostream>
#include <string>
#include <vector>
#include "Movies.h"
#include "People.h"

class MovieDb 
{
public:
	MovieDb(){ImportFile();};
	~MovieDb(){};
	std::string FindTitleById(const std::string &rInId);
    std::string FindPersonById(const std::string &rInId);
    std::string CheckOutMovie(const std::string &rInId);
private:
	void ImportFile();
	void ParseFileMovie(std::string fileNameIn, std::vector<Media*> &inVector);
	void ParseFilePeople(std::string fileNameIn, std::vector<People*> &inVector);
	std::vector<Media*> mMovieVector;
	std::vector<People*> mPeopleVector;

};

#endif /* MOVIEDB_H_ */
