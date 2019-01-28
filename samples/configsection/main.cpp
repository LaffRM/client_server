#include <iostream>

#include "../../common/configsection/configsection.h"
using namespace std;

int main()
{
    char val[MAXLENVALUE];
    int t = 0;
    //char const *file_conf = "file:///home/roman/git_isp/infotek_git/trunk/samples/configsection/conf";
    char const *file_conf = "conf";
    
    ConfigSection a(file_conf, "local");
    t = a.u("timeout", 400);
    char const *tmp_buf1 = a.u("log", "failfailfailfailfailfailfailfailfailfailfailfailfailfailfail", val);

    printf("log: %s\n", tmp_buf1);
    printf("timeout: %d\n", t);
    a.update(file_conf, "global");

    t = a.u("port", 666);
    char const *tmp_buf2 = a.u("ip", "kroke", val);

    printf("ip: %s\n", tmp_buf2);
    printf("port: %d\n", t);
    return 0;
}
