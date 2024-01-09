#pragma once
#ifndef _FILES_HPP_
#define _FILES_HPP_

#include <memory>

struct FileCloser {
    void operator()(FILE *p) {
        fclose(p);
    }
};

using file_ptr = std::unique_ptr<FILE, FileCloser>;

#endif //_FILES_HPP_