 

#include <iostream>
#include "tinyxml2.h"
#include <regex>
#include <vector>
#include <fstream>

using namespace tinyxml2;
using std::cout;
using std::endl;
using std::string;
using std::regex;
using std::vector;
using std::ofstream;

struct RssItem
{
    string title;
    string link;
    string description;
    string content;
};

class RssReader
{
public:
    RssReader(const string& filename);
    void parseRss();//解析
    void dump(const string& filename);//输出
private:
    vector<RssItem> _rss;
};

RssReader::RssReader(const string& filename) {

    XMLDocument doc;
    XMLError ret = doc.LoadFile(filename.data());

    if (!ret)
    {
        cout << "success" << endl;
    }
    else {
        cout << "fail" << endl;
    }

    XMLElement* ele = doc.FirstChildElement("rss")->FirstChildElement("channel")->FirstChildElement("item");
    if (ele)
    {
        while (ele)
        {
            string title = ele->FirstChildElement("title")->GetText();
            string link = ele->FirstChildElement("link")->GetText();
            string description = ele->FirstChildElement("description")->GetText();
            string content = ele->FirstChildElement("content:encoded")->GetText();

            // 正则表达式用于匹配 HTML 标签
            std::regex tag_regex("<[^>]*>");
            // 使用 std::regex_replace 替换所有匹配到的 HTML 标签为空字符串
            description = std::regex_replace(description, tag_regex, "");
            content = std::regex_replace(content, tag_regex, "");

            RssItem rss;
            rss.title = title;
            rss.link = link;
            rss.description = description;
            rss.content = content;

            
            _rss.push_back(rss);

            /*cout << rss.title << endl;
            cout << rss.link << endl;
            cout << "=============================" << endl;*/


            ele = ele->NextSiblingElement("item");
        }

    }

}

void RssReader::dump(const string& filename) {
    ofstream ofs(filename);
    int index = 0;
    for (auto& ret : _rss)
    {
        ofs << "<doc><docid>" << ++index
            << "</docid><title>" << ret.title
            << "</title><link>" << ret.link
            << "</link><description>" << ret.description
            << "</description><content>" << ret.content
            << "</content></doc>" << endl;       
    }

    ofs.close();
}

void test0() {
    
    RssReader r("C:/Users/14310/Desktop/coolshell.xml");
   
    r.dump("C:/Users/14310/Desktop/2.txt");
}

int main()
{
    //SetConsoleOutputCP(CP_UTF8);
    test0();
}
