#include "configsection.h"


ConfigSection::ConfigSection(const char *file_, const char *section_)
{
    file__ = file_;
    section__ = section_;

}


ConfigSection::~ConfigSection()
{

}


void ConfigSection::update(char const *file_, char const *section_)
{
    file__ = file_;
    section__ = section_;
}


int ConfigSection::open_file()
{
        if (file__ != NULL)
        {
            file_ptr__ = fopen(file__, "r+");

            if (file_ptr__ == NULL)
            {
                return -1;
            }
        }
        else
        {
            return -1;
        }

        return 0;
}


int ConfigSection::copy_str(char *cur_str_, char *copy_str_, int pos_)
{
    int i = pos_;
    int j = 0;

    while ((cur_str_[i] == ' ') || (cur_str_[i] == '\t') || (cur_str_[i] == '=') || (cur_str_[i] == '['))
    {
        i++;
    }

    if ((cur_str_[i] == '\n') || (cur_str_[i] == '#'))
    {
        return -1;
    }

    for(j = 0; isStop(cur_str_[i]); j++)
    {
        copy_str_[j] = cur_str_[i];
        i++;
    }

    copy_str_[j] = '\0';

    return j;
}


int ConfigSection::find_section()
{
    char tmp_buf[2*MAXLENVALUE] = {0};
    char tmp_name[MAXLENSECNAME] = {0};
    int f = 0;

    if(open_file() == -1)
    {
        return -1;
    }

    if(section__ == NULL)
    {
        printf("section not selected");
        return -1;
    }

    while (fgets(tmp_buf, 256, file_ptr__) != NULL)
    {
        if ((strchr(tmp_buf, LEFTBRACKET) != NULL) && (strchr(tmp_buf, RIGHTBRACKET) != NULL))
        {
            f = get_namesec(tmp_buf, tmp_name);
            if (strcmp(section__, tmp_name) != 0)
            {
                continue;
            }
            else
            {
                return f;
            }
        }
    }
    return 0;
}


int ConfigSection::get_namesec(char *cur_str_, char *sec_name_str_)
{
    int j = 0;

    return copy_str(cur_str_, sec_name_str_, j);
}


int ConfigSection::get_namevalue(char *cur_str_, char *val_name_str_)
{
    int i = 0;
    if ((strchr(cur_str_, LEFTBRACKET) != NULL) && (strchr(cur_str_, RIGHTBRACKET) != NULL))
    {
        return -2;
    }
    return copy_str(cur_str_, val_name_str_, i);
}


int ConfigSection::get_value(char *cur_str_, char *val_str_, int pos_)
{
    int i = pos_;

    return copy_str(cur_str_, val_str_, i);
}


int ConfigSection::u(char const *name_, int default_)
{
    char tmp_buf[2*MAXLENVALUE] = {0};
    char tmp_value[MAXLENVALUE] = {0};
    char tmp_name_value[MAXLENVALUENAME] = {0};

    int f = 0;

    if (find_section() == -1)
    {
        return default_;
    }

    while (fgets(tmp_buf, 256, file_ptr__) != NULL)
    {

        f = get_namevalue(tmp_buf, tmp_name_value);

        if (f == -2)
        {
            return default_;
        }
        if (strcmp(tmp_name_value, name_) != 0)
        {
            continue;
        }
        else
        {
            f = get_value(tmp_buf, tmp_value, f);
            fclose(file_ptr__);
            return atoi(tmp_value);
        }
    }

    fclose(file_ptr__);
    return default_;
}


char const *ConfigSection::u(char const *name_, char const *default_, char *str_)
{
    char tmp_buf[2*MAXLENVALUE] = {0};
    char tmp_value[MAXLENVALUE] = {0};
    char tmp_name_value[MAXLENVALUENAME] = {0};

    int f = 0;

    if (find_section() == -1)
    {
        return default_;
    }

    while (fgets(tmp_buf, 256, file_ptr__) != NULL)
    {

        f = get_namevalue(tmp_buf, tmp_name_value);

        if (f == -2)
        {
            return default_;
        }

        if (strcmp(tmp_name_value, name_) != 0)
        {
            continue;
        }
        else
        {
            f = get_value(tmp_buf, tmp_value, f);
            fclose(file_ptr__);
            strcpy(str_, tmp_value);
            return str_;
        }
    }

    fclose(file_ptr__);

    return default_;
}


bool ConfigSection::isStop(char c)
{
    if ((c != ' ') && (c != '\t') && (c != '=') && (c != '\n') && (c != ']'))
    {
        return true;
    }
    else
    {
        return false;
    }
}
