#include <iostream>
#include <string>
#include <sys/stat.h>
#include <windows.h>

#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES	((DWORD)-1)
#endif

/*******************************************/
BOOL IsDir(const std::string& path)
{
	DWORD Attr; 
 
	Attr = GetFileAttributes(path.c_str());
	if (Attr == INVALID_FILE_ATTRIBUTES)
		return FALSE;
 
	return (BOOL) (Attr & FILE_ATTRIBUTE_DIRECTORY);
}

/*******************************************/

void findFiles(std::string& fspath) {

  while ( * (fspath.rbegin()) == '/' || * (fspath.rbegin()) == '\\')
    fspath.pop_back();

  size_t i = 1;
  WIN32_FIND_DATA FindFileData;
  std::string destpath = fspath + std::string("\\*.*");

  std::cout << "destpath " << destpath << std::endl;

  HANDLE hFind = FindFirstFile(destpath.c_str(), & FindFileData);

  if (hFind != INVALID_HANDLE_VALUE) {
    do {
      std::string fullpath = std::string(fspath) + std::string("\\") + std::string(FindFileData.cFileName);
      if ( * (fullpath.rbegin()) == '.')
        continue;
      else
      if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        findFiles(fullpath);
      else {
        std::cout << "\t\t" << (FindFileData.cFileName) << std::endl;

      }
    }
    while (FindNextFile(hFind, & FindFileData));
  }
  FindClose(hFind);
}

/*******************************************/
int main(int argc, char ** argv) {

  if (argc <= 1) {
    std::cerr << "No path provided" << std::endl;
    return EXIT_FAILURE;
  }

  const char * path = argv[1];

  if (!IsDir(path)) {
    std::cerr << "Path doesn't exist" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "Provided path is: " << path << std::endl;

  std::string fspath = path;
  findFiles(fspath);

  return EXIT_SUCCESS;
}
