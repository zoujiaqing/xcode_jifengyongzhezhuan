#ifndef BASE_CONFIG_H_
#define BASE_CONFIG_H_
#include <string>

class CConfig
{   
public:
    CConfig(void){}
public:
    virtual void clear() = 0;
    void load();
protected:
    virtual void load_config(std::string& strFilePath) = 0;
    std::string m_strFilePath;
private:
    CConfig(const CConfig &cr);
    CConfig & operator=( const CConfig &cr);
};

#endif// base_config_h
