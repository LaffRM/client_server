#include "log.h"

using namespace std;

log::log(unsigned int n_, bool c_, char const *f_, char const *fp_)
{
    file__ = f_;
    file_prefix__ = fp_;
    max_size__ = n_;
    close__ = c_;
    name_create();

    file_ptr__ = fopen(file_name__, "a+");

    if(!file_ptr__)
    {
        printf("ERROR! File wasn't opened!");
        return;
    }

    if (file_length() + 149 >= max_size__)
    {
        fclose(file_ptr__);
        file_clear();
        if(close__)
        {
            file_ptr__ = fopen(file_name__, "a+");

            if(!file_ptr__)
            {
                printf("ERROR! File wasn't opened!");
                return;
            }
        }
    }

    if(close__)
    {
        fclose(file_ptr__);
    }
}

log::~log()
{
    if(!close__)
    {
        fclose(file_ptr__);
    }

}

void log::write(const char *msg_)
{
    char buffer_log[256]={0};
    char buffer_msg[128]={0};
    size_t msg_len = 0;

    if (close__)
    {
        file_ptr__ = fopen(file_name__, "a+");

        if(!file_ptr__)
        {
            printf("ERROR! File wasn't opened!");
            return;
        }
    }

    msg_len = strlen(msg_);

    if (msg_len > 128)
    {
        cut_msg(msg_, buffer_msg);
    }
    else
    {
        memcpy(buffer_msg, msg_, msg_len);
    }

    log_create(buffer_log, buffer_msg);

    msg_len = strlen(buffer_log);

    if (msg_len+file_length() > max_size__)
    {
        fclose(file_ptr__);
        file_rename();
        file_ptr__ = fopen(file_name__, "a+");

        if(!file_ptr__)
        {
            printf("ERROR! File wasn't opened!");
            return;
        }
        fseek(file_ptr__, 0, SEEK_END);
        fprintf(file_ptr__, "%s", buffer_log);
    }
    else
    {
        fseek(file_ptr__, 0, SEEK_END);
        fprintf(file_ptr__, "%s", buffer_log);
    }

    if (close__)
    {
        fclose(file_ptr__);
    }
}

size_t log::file_length()
{
    int f_l = 0;

    fseek(file_ptr__, 0, SEEK_END);

    f_l = ftell(file_ptr__);

    return f_l;
}


char* log::get_time(char* buf)
{
    struct tm *ptr;
    time_t tm;
    tm = time(NULL);
    ptr = localtime(&tm);

    strftime(buf, 80, "%g%m%d %H%M%S", ptr);

    return buf;
}


char* log::get_usec(char *buf)
{
    unsigned int usec;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    usec = tv.tv_usec;

    snprintf (buf, 80, "%d", usec);

    return buf;
}




void log::name_create()
{
    if(file_prefix__ != NULL)
    {
        strcpy(file_name__, file_prefix__);
    }

    strcat(file_name__, file__);
}

void log::log_create(char* buf_log, char const *buf_msg)
{
    char buffer_time[80];
    char buffer_usec[80];
    char buf_rn[2] = {'\r', '\n'};
    char buf_cpy[2] = {'\0'};
    unsigned int i = 0;

    i = strlen(buf_msg);

    if ( i >= 2)
    {

        buf_cpy[0] = buf_msg[i - 2];
        buf_cpy[1] = buf_msg[i - 1];

        if (memcmp(buf_cpy, buf_rn, 2) == 0)
            sprintf(buf_log, "%s %s %s", get_time(buffer_time), get_usec(buffer_usec), buf_msg);
        else
            sprintf(buf_log, "%s %s %s\r\n", get_time(buffer_time), get_usec(buffer_usec), buf_msg);

    }
    else
        sprintf(buf_log, "%s %s %s\r\n", get_time(buffer_time), get_usec(buffer_usec), buf_msg);

}

void log::file_clear()
{
    file_ptr__ = fopen(file_name__, "w");

    if(!file_ptr__)
    {
        printf("ERROR! File wasn't opened!");
        return;
    }

    fclose(file_ptr__);
}

void log::file_rename()
{
    if (file_name__[strlen(file_name__)-1] == '~')
    {
        file_name__[strlen(file_name__)-1] = '\0';
    }
    else
    {
        strncat(file_name__, "~", 1);
    }
    file_clear();
}


void log::cut_msg(const char *buf, char *buf_msg)
{
    strncpy(buf_msg, buf, 124);
    strcat(buf_msg, "...");
}

void log::writeb(void const* buf_, size_t len_, char const *info_)
{
    char buf_msg[256]={0};
    char buffer_log[256]={0};;
    size_t msg_len = 0;


    if (close__)
    {

        file_ptr__ = fopen(file_name__, "a+");

        if(!file_ptr__)
        {
            printf("ERROR! File wasn't opened!");
            return;
        }
    }



    if(file_ptr__)
    {
        fclose(file_ptr__);
    }

    file_ptr__ = fopen(file_name__, "ab+");

    if(!file_ptr__)
    {
        printf("ERROR! File wasn't opened!");
        return;
    }

    logb_msg(buf_, len_, info_, buf_msg);

    blog_create(buffer_log, buf_msg);

    msg_len = strlen(buffer_log);

    if (msg_len+file_length() > max_size__)
    {
        fclose(file_ptr__);
        file_rename();
        file_ptr__ = fopen(file_name__, "a+");

        if(!file_ptr__)
        {
            printf("ERROR! File wasn't opened!");
            return;
        }
        fseek(file_ptr__, 0, SEEK_END);
        fprintf(file_ptr__, "%s", buffer_log);
    }
    else
    {
        fseek(file_ptr__, 0, SEEK_END);
        fprintf(file_ptr__, "%s", buffer_log);
    }

    if (close__)
    {
        fclose(file_ptr__);
    }

}


void log::logb_msg(void const* buf_, size_t len_, char const *info_, char *buf_msg_)
{

    unsigned char const *p = (unsigned char const *)buf_;

    if(strlen(info_) <= 128)
    {
        strcpy(buf_msg_, info_);
    }
    else
    {
        char tmp_buf[128];
        cut_msg(info_, tmp_buf);
        strcpy(buf_msg_, tmp_buf);
        return;
    }

    for(unsigned int i = 0; (i < len_) && (strlen(buf_msg_)) <= 126; i++)
    {
        char tmp_c[4];
        sprintf(tmp_c, " %02x", p[i]);
        strcat(buf_msg_, tmp_c);
    }

    if (strlen(buf_msg_) > 128)
    {
        char tmp_buf[256];
        memcpy(tmp_buf, buf_msg_, 256);
        cut_msg(buf_msg_, tmp_buf);
        memcpy(buf_msg_, tmp_buf, 256);
    }

}

void log::blog_create(char* buf_log, char const *buf_msg)
{
    char buffer_time[80];
    char buffer_usec[80];

    sprintf(buf_log, "%s %s %s\r\n", get_time(buffer_time), get_usec(buffer_usec), buf_msg);

}
