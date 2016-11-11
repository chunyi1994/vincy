#include <algorithm>
#include <assert.h>
#include "simple_template.h"
#include "utils.h"
using std::vector;
using std::string;

namespace vincy {

SimpleTemplate::SimpleTemplate() :
    ifs_(),
    varValues_(),
    forLoopValues_()
{

}


SimpleTemplate::SimpleTemplate(const std::string &filename) :
    ifs_("templates/" + filename),
    varValues_(),
    forLoopValues_()
{

}

bool SimpleTemplate::operator!() const
{
   return ! operator bool();
}

vincy::SimpleTemplate::operator bool() const
{
    if(ifs_){
        return true;
    }else{
        return false;
    }
}


SimpleTemplate::~SimpleTemplate()
{
    if(ifs_)
    {
        ifs_.close();
    }
}

void SimpleTemplate::open(const std::string &filename)
{
    ifs_.open("templates/" + filename);
}

std::string SimpleTemplate::toString()
{
    if(ifs_)
    {
        std::string ret;
        std::string line;
        while(std::getline(ifs_, line))
        {
            ret.append(line);
        }
        return toString(ret);
    }
    return std::string("");
}

void SimpleTemplate::setValue(const std::string &key, const std::string &value)
{
    varValues_[key] = value;
}

void SimpleTemplate::setForLoopValues(const std::string &key, const ForLoopVector &vec)
{
    forLoopValues_[key] = vec;
}

enum ParseState{
    STATE_FIND_LEFT,
    STATE_FIND_LEFT_BRACE,
    STATE_FIND_RIGHT_PERCENT,
    STATE_FIND_LEFT_PERCENT,
    STATE_FIND_RIGHT_BRACE,
    STATE_END
};

std::string SimpleTemplate::toString(const std::string& rawContent)
{
    std::vector<std::string> lines;
    size_t pos = 0;
    size_t left = 0;
    ParseState state = STATE_FIND_LEFT;
    while(pos < rawContent.length())
    {
        switch(state)
        {
        case STATE_FIND_LEFT:
        {
            left = pos;
            pos = rawContent.find('{', left);
            if(pos == std::string::npos){
                pos = rawContent.length();
                state = STATE_END;
            }
            else
            {
                if(rawContent[pos + 1] == '%')
                {
                    state = STATE_FIND_LEFT_PERCENT;
                }
                else if(rawContent[pos + 1] == '{')
                {
                    state = STATE_FIND_RIGHT_BRACE;
                }

            }
            std::string str = rawContent.substr(left, pos - left);
            lines.push_back(str);
            break;
        }

        case STATE_FIND_LEFT_PERCENT:
        {
            left = pos + 2;
            pos = rawContent.find('%', left);
            assert(pos != std::string::npos);
            std::string str = rawContent.substr(left, pos - left);
            size_t posLeft = str.find("for");
            size_t posRight = str.find("in", posLeft);
            assert(posLeft != std::string::npos && posRight != std::string::npos);
            std::string item = str.substr(posLeft + 3, posRight - posLeft - 3);
            std::string items = str.substr(posRight + 2, str.length() - posRight - 2);
            trim(item);
            trim(items);
            pos +=2;
            left = pos;
            pos = rawContent.find("{%", left);
            assert(pos != std::string::npos);
            std::string loopStr = rawContent.substr(left, pos - left);
            trim(loopStr);
            processLoop(loopStr, item, items);
            lines.push_back(loopStr);
            state = STATE_FIND_RIGHT_PERCENT;
            break;
        }

        case STATE_FIND_RIGHT_PERCENT:
        {
            pos = rawContent.find("%}", pos);
            assert(pos != std::string::npos);
            pos += 2;
            state = STATE_FIND_LEFT;
            break;
        }

        case STATE_FIND_RIGHT_BRACE:
        {
            left = pos + 2;
            pos = rawContent.find('}',left);
            assert(pos != std::string::npos);
            std::string str = rawContent.substr(left, pos - left);
            trim(str);
            lines.push_back(varValues_[str]);
            pos += 2;
            state = STATE_FIND_LEFT;
            break;
        }

        case STATE_END:
            pos = std::string::npos;
            break;
        }//switch
    }//while

    std::string ret;
    std::for_each(lines.begin(), lines.end(), [&](const std::string& str) { ret += str; } );
    return ret;
}

void SimpleTemplate::processLoop(std::string &content, const std::string &item, const std::string &items)
{
    std::vector<std::string> lines;
    for(auto& itemMap : forLoopValues_[items]){
        size_t pos = 0;
        size_t left = 0;
        ParseState state = STATE_FIND_LEFT;
        while(pos < content.length())
        {
            switch(state)
            {
            case STATE_FIND_LEFT:
            {
                left = pos;
                pos = content.find('{', left);
                if(pos == std::string::npos)
                {
                    pos = content.length();
                    state = STATE_END;
                }
                else
                {
                    state = STATE_FIND_RIGHT_BRACE;
                }
                std::string str = content.substr(left, pos - left);
                lines.push_back(str);
                break;
            }

            case STATE_FIND_RIGHT_BRACE:
            {
                left = pos + 2;
                pos = content.find('}',left);
                assert(pos != std::string::npos);
                std::string itemName = content.substr(left, pos - left);
                trim(itemName);
                if(beginWith(itemName, item + "."))
                {
                    std::string child = itemName.substr(item.length() + 1, itemName.length() - item.length() - 1);
                    lines.push_back(itemMap[child]);
                }
                else
                {
                    lines.push_back(varValues_[itemName]);
                }
                pos += 2;
                state = STATE_FIND_LEFT;
                break;
            }

            case STATE_END:
                pos = std::string::npos;
                break;
            }//switch
        }//while
    }
    content = "";
    std::for_each(lines.begin(), lines.end(), [&](const string& str){ content += str; });

}

}

