#include <fstream>
#include <filesystem>
#include <functional>
#include <string>
#include <iostream>

void help(int argc, const char** argv, const std::string&& error) {
    std::cout << "error:" << error << std::endl;
}

int file_replace_str_trim_line(std::ifstream& ifs, std::ofstream& ofs, std::function<std::string(const std::string& line)> trim_line) {
    std::string line;
    while (std::getline(ifs, line)) {
        std::string line_new = trim_line(line);
        ofs << line_new << std::endl;
    }
    return 0;
}

int main(int argc, const char** argv) {
    int ret = -1;
    do {
        if (argc <= 2) break;
        std::string mode = argv[1], file = argv[2];

        if (!std::filesystem::exists(file)) break;
        std::string file_bk = file + ".bk";
        std::filesystem::copy_file(file, file_bk, std::filesystem::copy_options::update_existing);
        std::ifstream ifs(file_bk);
        std::ofstream ofs(file);

        if (mode == "full_line") {
            if (argc <= 4) return -1;
            std::string line_from = argv[3], line_to = argv[4];
            ret = file_replace_str_trim_line(ifs, ofs, [&line_from, &line_to](const std::string& line) {
                if (line_from == line) return line_to;
                return line;
            });
            break;
        }
        if (mode == "line_inner_between") {
            if (argc <= 5) return -1;
            std::string str_before = argv[3], str_after = argv[4], str_new = argv[5];
            ret = file_replace_str_trim_line(ifs, ofs, [&str_before, &str_after, &str_new](const std::string& line) {
                auto pos_before = line.find(str_before);
                if (pos_before == std::string::npos) return line;
                auto pos_after = line.find(str_after);
                if (pos_after == std::string::npos) return line;
                if (pos_before > pos_after) return line;
                std::string ret = line.substr(0, pos_before + str_before.size());
                ret += str_new;
                ret += line.substr(pos_after);
                return ret;
            });
            break;
        }

    } while (false);

    if (ret != 0) help(argc, argv, "mode not supported");

    return ret;
}
