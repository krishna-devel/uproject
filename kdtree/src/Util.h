#ifndef KDTREE_UTIL_H
#define KDTREE_UTIL_H

#include <string>
#include <vector>

using namespace std;

class Util {
public:
    /*
     * http://stackoverflow.com/a/3418285
     *
     */
    static void replaceAll(const string &from, const string &to, string &str) {
        if(from.empty()){
            return;
        }
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    }

    /**
     *
     * http://programmingknowledgeblog.blogspot.com/2013/04/the-most-elegant-way-to-split-string-in.html
     */
    static vector<string> split(const string &s, const string &delim, const bool keep_empty = false) {
        vector<string> result;
        if (delim.empty()) {
            result.push_back(s);
            return result;
        }
        string::const_iterator substart = s.begin(), subend;
        while (true) {
            subend = search(substart, s.end(), delim.begin(), delim.end());
            string temp(substart, subend);
            if (keep_empty || !temp.empty()) {
                result.push_back(temp);
            }
            if (subend == s.end()) {
                break;
            }
            substart = subend + delim.size();
        }
        return result;
    }

    static string convertMapToString(
        const map<string, string> &dict,
        const string &kvSeparator,
        const string &itemSeparator
    ) {
        string stringToReturn;
        for(pair<string, string> p: dict) {
            string s = p.first + kvSeparator + p.second;
            stringToReturn += s + itemSeparator;
        }
        return stringToReturn;
    }

    static map<string, string> convertStringToMap(
        const string &inputString,
        const string &kvSeparator,
        const string &itemSeparator
    ) {
        map<string, string> mapToReturn;
        for (string item: split(inputString, itemSeparator)){
            vector<string> kv = split(item, kvSeparator);
            mapToReturn[kv[0]] = kv[1];
        }
        return mapToReturn;
    }

    static string convertVectorToString(const vector<string> &data, const string &itemSeparator) {
        string stringToReturn;
        for(string s: data) {
            stringToReturn += s + itemSeparator;
        }
        return stringToReturn;
    }

    static vector<string> convertStringToVector(const string &inputString, const string &itemSeparator) {
        return split(inputString, itemSeparator);
    }
};

#endif //KDTREE_UTIL_H
