#include <cstdint>
#include <cstdio>
#include <exception>
#include <fstream>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <iterator>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using pair = std::pair<uint64_t, std::string>;

auto GetDynamicInfo(std::ifstream& DynamicInfoFile) {
  std::vector<pair> info;
  std::string line;

  while (!DynamicInfoFile.eof()) {
    std::getline(DynamicInfoFile, line);
    std::stringstream ss(line);
    pair tmp;
    ss >> tmp.first;
    tmp.second = ss.str();
    if (!tmp.second.empty()) {
      tmp.second = tmp.second.substr(tmp.second.find(" "));
      info.push_back(tmp);
    }
  }
  return info;
}

void LinkWithStaticInfo(std::ifstream& StaticInfoFile, std::ofstream& ResultFile, std::vector<pair>& info) {
  std::string line;
  std::string end_of_line = "\"];";
  while (!StaticInfoFile.eof()) {
    std::getline(StaticInfoFile, line);
    for (auto&& statInfo : info) {
      std::stringstream ss;
      ss << "Instruction0x" << std::hex << statInfo.first << " [";
      std::string instructionName(ss.str());
      if (line.find(instructionName) != line.npos) {
        auto end   = line.find_last_of("\"");
        line = line.substr(0, end);
        line = line + " | " + statInfo.second + end_of_line;
      }
    }
    ResultFile << line << std::endl;
  }
}

int main() try {
  std::ifstream StaticInfoFile("../NEWGRAPH.txt");
  std::ifstream DynamicInfoFile("../log.txt");
  std::ofstream ResultFile("../FinalFile.txt");

  if (StaticInfoFile.is_open() and DynamicInfoFile.is_open() and ResultFile.is_open()) {
    auto&& info = GetDynamicInfo(DynamicInfoFile);
    LinkWithStaticInfo(StaticInfoFile, ResultFile, info);
    }
  else {
    throw std::runtime_error("Cannot open file!");
  }
}

catch (std::exception& err) {
  std::cerr << err.what() << std::endl;
}