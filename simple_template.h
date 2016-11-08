#ifndef SIMPLETEMPLATE_H
#define SIMPLETEMPLATE_H
#include <fstream>
#include <string>
#include <map>
#include <vector>
//这个模板引擎性能很差,需要每次都解析一遍
namespace vincy {

typedef std::map<std::string, std::string> ForItemMap;
typedef std::vector<ForItemMap> ForLoopVector;
class SimpleTemplate
{
public:
    SimpleTemplate();
    ~SimpleTemplate();
    SimpleTemplate(const std::string& filename);
    void open(const std::string& filename);

    std::string toString();
    void setValue(const std::string& key, const std::string&value);
    void setForLoopValues(const std::string& key, const ForLoopVector &vec);

private:
    std::string toString(const std::string& content);
    void processLoop(std::string &content, const std::string &item, const std::string &items);

    std::ifstream ifs_;
    std::map<std::string, std::string> varValues_;
    std::map<std::string, ForLoopVector> forLoopValues_;

};

}
#endif // SIMPLETEMPLATE_H

//示例===========================================
// {% for item in titles %} <a href="{{ item.url }}"> {{ item.title }}</a> {% endfor %}
//    //<h1>我的名字是{{ myname }}</h1>
//    //<h1>我的密码是{{ mypassword }}</h1>
//==============================================


//std::string handleHey(HttpRequest& r)
//{
//    //hey.html的内容是:
//    //<h1>我的名字是{{ myname }}</h1>
//    //<h1>我的密码是{{ mypassword }}</h1>
//    SimpleTemplate t("hey.html");
//    t.setValue("myname", "gaochunyi");
//    t.setValue("mypassword", "abc123456");
//    return t.toString();
//}
