#ifndef KDTREE_UTIL_H
#define KDTREE_UTIL_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

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

    /**
     * Gzip compress/decompress code: https://gist.github.com/yfnick/6ba33efa7ba12e93b148
     *
     * @param data
     * @return
     */
    static string compress(const string& data) {
        namespace bio = boost::iostreams;

        stringstream compressed;
        stringstream origin(data);

        bio::filtering_streambuf<bio::input> out;
        out.push(bio::gzip_compressor(bio::gzip_params(bio::gzip::best_compression)));
        out.push(origin);
        bio::copy(out, compressed);

        return compressed.str();
    }

    static string decompress(const string& data) {
        namespace bio = boost::iostreams;

        stringstream compressed(data);
        stringstream decompressed;

        bio::filtering_streambuf<bio::input> out;
        out.push(bio::gzip_decompressor());
        out.push(compressed);
        bio::copy(out, decompressed);

        return decompressed.str();
    }

    static void writeBinaryFile(const string &filename, const string &data){
        ofstream file(filename, std::ios::binary);
        if (file.is_open()) {
            file.write(data.c_str(), data.length());
            file.close();
        } else {
            cout << "Unable to open file: " << filename << endl;
        }
    }

    static string readBinaryFile(const string &filename) {
        ifstream file(filename, std::ios::binary);
        return static_cast<stringstream const&>(stringstream() << file.rdbuf()).str();
    }

};

#endif //KDTREE_UTIL_H
