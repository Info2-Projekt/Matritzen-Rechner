
#ifndef __CFILE_H
#define __CFILE_H

#include <fstream>
#include <string>

namespace Codebook {

class CFile {
	private:
    std::fstream *m_file;
    std::string m_filename;
    std::streampos m_size;

	public:
		class file_error {
			public:
        const std::string name;
        file_error(const std::string &n) :name(n) {};
		};

    CFile();
    explicit CFile(const std::string &name, bool create=true, bool erase=false);
		~CFile();

    bool open(const std::string &name, bool create=true, bool erase=false);

    void readBlock(void *addr, std::streampos size, std::streampos pos);
    void writeBlock(const void *addr, std::streampos size, std::streampos pos);
    void readBlock(void *addr, std::streampos size);
    void writeBlock(const void *addr, std::streampos size);
    void expand(std::streampos size);
    unsigned long size() const {
      return(m_size);
    }
    std::string getFileName(void) const{
      return(m_filename);
    }
};


}; // namespace Codebook

#endif
