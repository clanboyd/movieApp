#ifndef MOVIEDB_H_
#define MOVIEDB_H_

#include <iostream>
#include <string>
#include <vector>
#include "Movies.h"

class MovieDb 
{
public:
	MovieDb(){ImportFile();};
	~MovieDb(){};
	std::string FindTitleById(const std::string &rInId);
private:
	void ImportFile();
	void ParseFile(std::string fileNameIn, std::vector<Media*> &inVector);
	std::vector<Media*> mMovieVector;

};

#endif /* MOVIEDB_H_ */
