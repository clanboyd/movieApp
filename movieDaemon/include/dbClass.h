#ifndef DBCLASS_H_
#define DBCLASS_H_

#include <stdio.h>
#include <iostream>
#include <vector>
#include "sqlite3.h"

const std::string DB_NAME="/home/yoda/movieApp/movieDaemon/data/movie.db";

const std::string DB_SELECT_STR="SELECT ";
const std::string DB_INSERT_STR="INSERT ";
const std::string DB_DELETE_STR="DELETE ";
const std::string DB_UPDATE_STR="UPDATE ";
const std::string DB_FROM_STR=" FROM ";
const std::string DB_WHERE_STR=" WHERE ";
const std::string DB_INTO_STR=" INTO ";
const std::string DB_VALUES_STR=" VALUES ";
const std::string DB_AND_STR=" AND ";
const std::string DB_OPEN_BRAC_STR="(";
const std::string DB_CLOSE_BRAC_STR=")";
const std::string DB_ALL_STR="*";
const std::string DB_EQUAL_STR="=";
const std::string DB_SPACE_STR=" ";
const std::string DB_QUOTE_STR="\'";
const std::string DB_TERM_STR=";";
const std::string DB_COMMA_STR=",";

const std::string DB_MOVIES_STR="Movies";
const std::string DB_MOVIES_TITLE_STR="title";
const std::string DB_MOVIES_FORMAT_STR="format";
const std::string DB_MOVIES_GENRE_STR="genre";
const std::string DB_MOVIES_KEY_STR="key";

const std::string DB_PEOPLE_STR="people";
const std::string DB_PEOPLE_NAME_STR="name";
const std::string DB_PEOPLE_NAME_KEY_STR="name_key";

const std::string DB_CHECKOUT_STR="checkout";
const std::string DB_CHECKOUT_NAME_KEY_STR="name_key";
const std::string DB_CHECKOUT_MOVIE_KEY_STR="movie_key";

class dbClass 
{
    public:

	dbClass(){};
	~dbClass(){};
    int DbInit();
    int OpenDb();
    void CloseDb();
    void ListDb();
    void SetResultSingle(const std::string &rResult){mSingleRes=rResult;};
    std::string GetResultSingle(){return mSingleRes;};
    void ReSetResultSingle(){mSingleRes="INIT";};
    
    void SetResultMulti(const std::string &rResult){mMultiRes.push_back(rResult);};
    std::vector<std::string> GetResultMulti(){return mMultiRes;};
    void SetResultCheckInOut(const bool &rResult){mCheckInOutRes=rResult;};
    bool GetResultCheckInOut(){return mCheckInOutRes;};

    std::string QueryMovieByKey(const std::string &rKey);
    std::string QueryPersonByKey(const std::string &rKey);
    std::string CheckOutMovie(const std::string &rMovieKey, const std::string &rPersonKey);
    std::string CheckInMovie(const std::string &rMovieKey, const std::string &rPersonKey);

    // CALLBACKS
    static int CallbackSingle(void *pObj, int argc, char **argv, char **azColName);
    static int CallbackMulti(void *pObj, int argc, char **argv, char **azColName);

    struct CALL_RESULT
    {
        std::string movieID;
        std::string personID;
        bool result;
    };
    
    private:

	sqlite3 *sqlDriver;
    std::string mQuery;
    std::string mSingleRes;
    bool mCheckInOutRes;
    std::vector<std::string> mMultiRes;
};

#endif /* DBCLASS_H_ */
