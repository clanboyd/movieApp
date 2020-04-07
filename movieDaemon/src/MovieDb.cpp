#include <fstream>
#include "MovieDb.h"
#include "Movies.h"
#include <typeinfo>
#include <vector>

void MovieDb::ImportFile()
{
	ParseFileMovie("movieDaemon/data/Movies.csv",mMovieVector);
	ParseFilePeople("movieDaemon/data/People.csv",mPeopleVector);
}

void MovieDb::ParseFileMovie(std::string fileNameIn, std::vector<Media*> &inVector)
{   
    std::ifstream myReadFile;
    myReadFile.open(fileNameIn.c_str());
    std::string tempString;
    std::string title;
    std::string media;
    std::string genre;
    std::string rfidMovie;
    std::string rfidPerson;

    if (myReadFile.is_open()) 
	{
        while( !myReadFile.eof() )
		{
            getline(myReadFile, tempString);
            if( myReadFile ){
				size_t found = tempString.find(",");
				if ( found != std::string::npos )
				{
					title=tempString.substr(0, found);
					tempString.erase(0,found+1);
					found = tempString.find(",");
					if ( found != std::string::npos )
					{
						media=tempString.substr(0, found);
						tempString.erase(0,found+1);
						found = tempString.find(",");
						if ( found != std::string::npos )
						{
							genre=tempString.substr(0, found);
							tempString.erase(0,found+1);
						    found = tempString.find(",");
    						if ( found != std::string::npos )
    						{
    							rfidMovie=tempString.substr(0, found);
    							tempString.erase(0,found+1);
    							rfidPerson=tempString;
    						}
						}
					}
				}
                if(media=="BluRay")
				{
                    Media *myblu = new BluRay(title,rfidMovie,rfidPerson);
                    inVector.push_back(myblu);
                }
                else if(media=="BluRay4k")
				{
                    Media *myblu4k = new BluRay4k(title,rfidMovie,rfidPerson);
                    inVector.push_back(myblu4k);
                }
                else
				{
                    Media *mydvd = new DVD(title,rfidMovie,rfidPerson);
                    inVector.push_back(mydvd);
                
                }
            }
        }
    }
    myReadFile.close();
}

std::string MovieDb::FindTitleById(const std::string &rInId)
{
	std::string title=UNK_MOVIE;
	for ( std::vector<Media*>::iterator it= mMovieVector.begin();it!= mMovieVector.end(); ++it)
	{
		if ( (*it)->GetMovieRfId() == rInId )
		{
			title = (*it)->GetTitle();
			break;
		}
	}
	return title;

}

void MovieDb::ParseFilePeople(std::string fileNameIn, std::vector<People*> &inVector)
{   
    std::ifstream myReadFile;
    myReadFile.open(fileNameIn.c_str());
    std::string tempString;
    std::string name;
    std::string rfidPerson;
    std::string rfidMovie;

    if (myReadFile.is_open()) 
	{
        while( !myReadFile.eof() )
		{
            getline(myReadFile, tempString);
            if( myReadFile ){
				size_t found = tempString.find(",");
				if ( found != std::string::npos )
				{
					name=tempString.substr(0, found);
					tempString.erase(0,found+1);
					found = tempString.find(",");
					if ( found != std::string::npos )
					{
						rfidPerson=tempString.substr(0, found);
						tempString.erase(0,found+1);
    					rfidMovie=tempString;
					}
				}
                People *myPeep = new People(name,rfidPerson,rfidMovie);
                inVector.push_back(myPeep);
            }
        }
    }
    myReadFile.close();
}

std::string MovieDb::FindPersonById(const std::string &rInId)
{
	std::string name=UNK_PERSON;
	for ( std::vector<People*>::iterator it= mPeopleVector.begin();it!= mPeopleVector.end(); ++it)
	{
		if ( (*it)->GetCardId() == rInId )
		{
			name = (*it)->GetName();
			break;
		}
	}
	return name;

}

std::string MovieDb::CheckOutMovie(const std::string &rInId)
{
    std::vector<std::string> moviedbVector;
    std::vector<std::string> persondbVector;
    // Update Movie db with persons id
    bool movieFound=false;
    bool personFound=false;
    std::fstream myMFile;
    std::fstream myPFile;
    string tmpString;
    string movieid = rInId.substr(0,12);
    string personid = rInId.substr(12,12);
    myMFile.open("movieDaemon/data/Movies.csv", ios::in);
	size_t found;
    
    while (!myMFile.eof()) 
    {
        getline(myMFile, tmpString);
		found = tmpString.find(movieid);
		if ( found != std::string::npos )
		{
            // find final comma
    		found = tmpString.find_last_of(",");
    		if ( found != std::string::npos )
            {
                tmpString.replace(found+1,personid.length(),personid);
                moviedbVector.push_back(tmpString);
                movieFound=true;
            }
        }
        else
        {
            moviedbVector.push_back(tmpString);
        }
    }
    myMFile.close();
    if ( true == movieFound )
    {
        myPFile.open("movieDaemon/data/People.csv", ios::in | ios::out | std::ofstream::app);
        while (!myPFile.eof()) 
        {
            getline(myPFile, tmpString);
    		found = tmpString.find(personid);
    		if ( found != std::string::npos )
    		{
                // find final comma
    		    found = tmpString.find_last_of(",");
    		    if ( found != std::string::npos )
                {
                    tmpString.replace(found+1,movieid.length(),movieid);
                    persondbVector.push_back(tmpString);
                    personFound=true;
                }
            }
            else
            {
                persondbVector.push_back(tmpString);
            }
        }
    }
    myPFile.close();

    if ( true == movieFound && true == personFound )
    {
        myMFile.open("movieDaemon/data/Movies.csv", ios::out);
        for ( std::vector<std::string>::iterator it= moviedbVector.begin();it!= moviedbVector.end(); ++it)
        {
            myMFile << (*it) << "\n";
        }
        myMFile.close();
        myPFile.open("movieDaemon/data/People.csv", ios::out);
        for ( std::vector<std::string>::iterator it= persondbVector.begin();it!= persondbVector.end(); ++it)
        {
            myPFile << (*it) << "\n";
        }
        myPFile.close();
	    return IO_SUCCESS;
    }
	return IO_ERROR;

}
