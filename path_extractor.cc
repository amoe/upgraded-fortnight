#include <cctype>
#include "path_extractor.hh"
#include "utility.hh"

using std::string;
using std::vector;
using std::isspace;

// The path extractor also has a responsibility to resolve the paths into 
// absolute ones.

vector<string> PathExtractor::getRequestedFiles(
    string uploadSpec, string xlsxPath
) {
    vector<string> splitVersion =  splitSemicolons(uploadSpec);

    vector<string> result;
        
    for (const string& str : splitVersion) {
        if (str.empty() || isWhitespaceOnly(str))
            continue;

        result.push_back(resolvePath(xlsxPath, str));
    }

    return result;
}


bool PathExtractor::isWhitespaceOnly(string input) {
    for (int i = 0; i < input.size(); i++) {
        if (!isspace(input.at(i))) 
            return false;
    }

    return true;
}
