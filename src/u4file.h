/*
 * u4file.h
 */

#ifndef U4FILE_H
#define U4FILE_H

#include <string>
#include <vector>
#include <list>

#ifdef putc
#undef putc
#endif

/**
 * An abstract interface for file access.
 */
class U4FILE {
public:
    virtual ~U4FILE() {}

    virtual void close() = 0;
    virtual int seek(long offset, int whence) = 0;
    virtual long tell() = 0;
    virtual size_t read(void *ptr, size_t size, size_t nmemb) = 0;
    virtual int getc() = 0;
    virtual int putc(int c) = 0;
    virtual long length() = 0;

    int getshort();
};

/** A replacement class to manage path searching. Very open-concept */
#define u4Path (*U4PATH::getInstance())
class U4PATH {
public:
    U4PATH() : defaultsHaveBeenInitd(false){}
    void initDefaultPaths();

    static U4PATH * getInstance();

    std::list<std::string> u4ForDOSPaths;
    std::list<std::string> u4ZipPaths;

private:
    bool defaultsHaveBeenInitd;
};

bool u4fsetup();
void u4fcleanup();
bool u4isUpgradeAvailable();
bool u4isUpgradeInstalled();
U4FILE *u4fopen(const std::string &fname);
U4FILE *u4fopen_upgrade(const std::string &fname);
U4FILE *u4fopen_stdio(const char* fname);
void u4fclose(U4FILE *f);
int u4fseek(U4FILE *f, long offset, int whence);
long u4ftell(U4FILE *f);
size_t u4fread(void *ptr, size_t size, size_t nmemb, U4FILE *f);
int u4fgetc(U4FILE *f);
int u4fgetshort(U4FILE *f);
int u4fputc(int c, U4FILE *f);
long u4flength(U4FILE *f);
std::vector<std::string> u4read_stringtable(U4FILE *f, long offset, int nstrings);

std::string u4find_path(const char* fname,
                        const std::list<std::string>* subPaths = NULL);
std::string u4find_music(const std::string &fname);
std::string u4find_sound(const std::string &fname);
std::string u4find_conf(const std::string &fname);
std::string u4find_graphics(const std::string &fname);

#endif
