#include <iostream>
#include "parser.h"

int main(int argc, char **argv)
{
    while (true)
    {
        char input[100];
        std::cin >> input;
        std::string src(input);
        Parser parser(src);
        float result = parser.CalculateRpn();
        std::cout << "result : " << result << std::endl;
    }
    return 0;
}