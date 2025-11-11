#include <iostream>
#include <vector>
#include <sstream>

std::vector<std::string> split_spaces(const std::string &str)
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string word;

    while (ss >> word) // skips extra spaces automatically
        result.push_back(word);

    return result;
}

int main(int ac, char **av)
{
    if (ac < 2)
    {
        std::cerr << "Usage: ./a.out \"your string here\"\n";
        return 1;
    }

    std::string input = av[1];
    std::vector<std::string> cmds = split_spaces(input);

    for (int i = 0; i < (int)cmds.size(); i++)
    {
        std::cout << cmds[i];
        if (i < (int)cmds.size() - 1)
            std::cout << " ";
    }
    std::cout << "\n";
}

