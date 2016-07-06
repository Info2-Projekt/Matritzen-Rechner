/*
** Systemspezifische Dateifunktionen
*/

#ifndef __FILESPECIFICS_H
#define __FILESPECIFICS_H

#include "CFileInfo.h"

#include <vector>

namespace Codebook {


  bool getDir(const std::string &name, CFileInfo::filelist_type &files);
  CFileInfo getFileInfo(const std::string &name);
  bool createDirectory(const std::string &name);
  bool removeDirectory(const std::string &name);
  bool removeFile(const std::string &name);
  bool copyFile(const std::string &sname, const std::string &dname, bool dontOverwrite=true);


}; // namespace Codebook

#endif
