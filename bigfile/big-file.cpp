#include <iostream>
#include <algorithm>
#include "app-exception.hpp"
#include "string-utils.hpp"
#include "file-generator.hpp"
#include <cstring>
#include <algorithm>
#include <vector>
extern char *__progname;

bool stdin_load(std::vector<std::string> &vec)
{
    bool ret = false;

    while (std::cin)
    {
        std::string input;
        std::getline(std::cin, input);
        input += '\n';
        vec.push_back(input);
        if (!ret)
            ret = true;
    }

    return ret;
}

int main(int agrc, const char **argv)
{

    try
    {
        if (agrc < 3)
        {
            std::cout << "Usage : " << __progname << " "
                      << "<filename> <size>" << std::endl;
            return 0;
        }

        std::string file = argv[1];

        unsigned long size = StringUtils::toLongVal(argv[2]);

        std::vector<std::string> vec;

        stdin_load(vec);

        FileGenerator generator(file, size, [vec](unsigned char *buff, unsigned long size) {
            int i = 0;
            unsigned long total = 0;
            
            if (buff == NULL || size == 0 || vec.size() == 0 ) return;

            unsigned char *p = buff;

            memset(buff, 0, size);

            while (total < size)
            {
                snprintf((char *)p, size - total, "%s", vec[i].c_str());
                total += vec[i].length();
                p += vec[i].length();
                if (++i == vec.size())
                    i = 0;
            }
        },
                                [](unsigned long curr, unsigned long total) {
                                    char s[1024];
                                    sprintf(s, "Progress Bar [%lu / %lu]\r", curr, total);
                                    std::cout << s;
                                });

        generator.generate();

        std::cout << std::endl;
    }
    catch (std::exception &e)
    {
        std::cout << "[" << __progname << "]"
                  << "[ERROR] " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
