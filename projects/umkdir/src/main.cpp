#include <fstream>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

using namespace std;

void help(const char* pszExeName)
{
    cout << "usage:" << pszExeName << " folder_and_sub_folders" << endl;
    cout << "by uniqs. contact uniqs@163.com for detail." << endl;
}

int main(int argc, const char** argv)
{
    if (argc < 2)
    {
        help(argv[0]);
        return -1;
    }

    
    const auto root = fs::current_path();
    const auto dir = root / argv[1];
    fs::create_directories(dir);
}
