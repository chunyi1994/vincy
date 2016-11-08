#ifndef HTTP_UTILS_H
#define HTTP_UTILS_H
#include <string>

#include <sys/stat.h>
namespace vincy {

static ssize_t getFileSize(const char *path)
{
    ssize_t fileSize = -1;
    struct stat statBuff;
    if(stat(path, &statBuff) >= 0)
    {
        fileSize = statBuff.st_size;
    }
    return fileSize;
}

std::string contentType(const std::string &path){
    size_t dotPos = path.find(".");
    if(dotPos == std::string::npos){
        return "text/html";
    }

    std::string ext = path.substr(dotPos, 4);
    if(ext == ".png")
    {
        return "image/png";
    }
    else if(ext == ".css")
    {
        return "text/css";
    }
    else if(ext == ".jpg")
    {
        return "image/jpeg";
    }
    else if(ext == ".gif")
    {
        return "image/gif";
    }
    else if(ext == ".htm")
    {
        return "text/html";
    }
    else
    {
        return "text/html";
    }
}

}

#endif // HTTP_UTILS_H

