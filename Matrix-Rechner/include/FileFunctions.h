/*
** Allgemeine Dateifunktionen
*/

#ifndef __FILEFUNCTIONS_H
#define __FILEFUNCTIONS_H

#include "FileSpecifics.h"
#include "CFileFilter.h"
#include "CFileArray.h"
#include <vector>
#include <deque>

#include <fstream>

namespace Codebook {


bool fileExists(const std::string &name);
bool createPath(const std::string &name);
bool moveFile(const std::string &sname, const std::string &dname, bool dontOverwrite=true);
bool copyDirectory(const std::string &sname, const std::string &dname, bool copyMainDir=true, bool dontOverwrite=true);
bool removeDirectory(const std::string &name, bool removeMainDir);
bool moveDirectory(const std::string &sname, const std::string &dname, bool moveMainDir=true, bool dontOverwrite=true);
void sortFileList(CFileInfo::filelist_type &fl);
bool getCompleteDir(const std::string &name, CFileInfo &fi);
std::string fileInfoToHtml(const CFileInfo &fi, bool onlyDirs, int imgsize, int fontsize);
std::string dirToHtml(const std::string &name);
std::string dirToHtml(const CFileInfo::filelist_type &filelist, const std::string &dirname);
std::string searchResultsToHtml(const std::string &name, const CFileFilter &ff, const CFileInfo::filelist_type &fl);
void findFiles(const std::string &name, const CFileFilter &filefilter, CFileInfo::filelist_type &filelist);
void findFiles(const CFileInfo &structure, const CFileFilter &filefilter, CFileInfo::filelist_type &filelist);
std::streampos findFirstInFile(const std::string &name, const std::string &search, std::streampos p=0);
std::streampos findFirstInFile(CFileArray &file, const std::string &search, std::streampos p=0);
std::streampos findFirstInFileCI(const std::string &name, const std::string &search, std::streampos p=0);
std::streampos findFirstInFileCI(CFileArray &file, const std::string &search, std::streampos p=0);
std::streampos findFirstInFile(const std::string &name, const std::vector<char> &search, std::streampos p=0);
std::streampos findFirstInFile(CFileArray &file, const std::vector<char> &search, std::streampos p=0);
std::streampos findFirstInFile(const std::string &name, const std::deque<char> &search, std::streampos p=0);
std::streampos findFirstInFile(CFileArray &file, const std::deque<char> &search, std::streampos p=0);
std::streampos compareFiles(const std::string &name1,const std::string &name2);
std::streampos findFirstPatternInFile(const std::string &name, const std::string &search, bool casesensitivity=true, std::streampos p=0);
std::streampos findFirstPatternInFile(CFileArray &file, const std::string &search, bool casesensitivity=true, std::streampos p=0);

}; // namespace Codebook

#endif
