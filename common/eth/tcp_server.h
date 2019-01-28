#ifndef __ETH_TCP_SERVER__H__
#define __ETH_TCP_SERVER__H__

#include "../../common/configsection/configsection.h"
#include "../../common/log/log.h"


#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>

#include <string>
#include <map>


#define MAX_TCP_SERVER_BUF_SIZE 1024
#define MAX_LOG_SIZE 65535
#define DEFAULT_LOG_NAME "log.log"
#define DEFAULT_BLOG_NAME "binary_log.log"

namespace eth {

class TCPServer {

public:
  typedef unsigned long Id; //!< Идентификатор клиента (автоикримент при добавлении)

  log *log_file;              //!< Указатель на объект для записи в лог
  log *log_file_b;            //!< Указатель на объект для записи в лог в hex виде
  ConfigSection *cfg_file__;    //!< Указатель на объект для чтения параметров из config файла

private:

  int socket__; //!< сокет сервера
  Id id__;
  //!Структура для хранения информации о клиенте
  struct SocketInfo {
    int socket;          //!< Сокет клиента
    std::string ip;      //!< IP клиента
    unsigned short port; //!< Порт клиента

    unsigned char send_buf[MAX_TCP_SERVER_BUF_SIZE]; //!< Буфер для отправки
    size_t   send_len;            //!< Размер буфера для отправки
    SocketInfo(int socket_, std::string const &ip_, unsigned short port_):socket(socket_), ip(ip_), port(port_), send_len(0) {}
  };


  std::map<Id, SocketInfo> sockets__; //!< Список сокетов клиентов

public:

  TCPServer(char const *cfg_file_name_):socket__(-1){

    char val_log[MAXLENVALUE] = {"\0"};
    char val_binary_log[MAXLENVALUE] = {"\0"};

    cfg_file__ = new ConfigSection(cfg_file_name_, "local");

    const char *log_name = cfg_file__->u("log", DEFAULT_LOG_NAME, val_log);
    const char *log_name_b = cfg_file__->u("binary_log", DEFAULT_BLOG_NAME, val_binary_log);

    log_file = new log(MAX_LOG_SIZE, true, log_name);

    log_file_b = new log(MAX_LOG_SIZE, true, log_name_b);

    id__ = 0;
  }


  ~TCPServer(){

      for(std::map<Id, SocketInfo>::iterator tmp_it, it = sockets__.begin(); it != sockets__.end(); it=tmp_it)
      {
          tmp_it = it;
          ++tmp_it;

          close((it->second).socket);
          sockets__.erase(it);
      }

      delete cfg_file__;
      delete log_file;
      delete log_file_b;
      if( socket__!=-1 ) close(socket__);

  }

  // Инициализация серверного сокета
  bool tcp_server__init(char const *sv_host_, unsigned short sv_port_);// {

    //1. Открытие сокета socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)

    //2. Проверка успешности открытия

    //3. Привязваем сокет к локальной конечной точке ip:port, bind(...)

    //return true;
  //}

  // Проверить события на сокете (осуществляется функцией FD_ISSET)
  void tcp_server__socket_check (fd_set const &rd_, fd_set const &wr_);//{
    //1. Проверить чтение на своем сокете, возможно кто то хочет подключиться

    //1.1 Если действительно кто то подключается тогда делаем accept(...)

    //1.2 Добавляем клиента в список клиентов sockets__

    //1.3 Оповещение о соединении абстрактный метод tcp_server__connected()

    //2. Проверяем клиентские сокеты на чтение, может что то прислали

    //2.1 Если есть событие чтения то пытаемся прочитать read(...)

    //2.2 Если прочитали 0 байт или с ошибкой, закрываем сокет и удаляем из списка

    //2.2.1 Оповещение о разрыве соединения абстрактный метод tcp_server__disconnected()

    //2.3 Если чтение успешное то вызываем абстрактный метод tcp_server__recv(), и передаем в него что смогли прочитать

    //3.1 Проверяем возможность записи на клиентский сокет, отпправляем данные если что то уже есть в буфере
  //}

  // Добавить сокеты "на прослушку" (осуществляется функцией FD_SET)
  void tcp_server__socket_add(fd_set &rd_, fd_set &wr_, int &n_);// {
    //1. Добавляем на чтение свой сокет, чтобы смогли подключится клиенты

    //2. Для всех клиентов выставляем так же, что готовы читать данные

    //3. Добавляем сокеты клиентов на запись, вдруг что то будем отправлять

    //4. Следим чтобы значение как сокет сервера так и клиентов не привышало значение дискриптора в n_,
    //   Делаем n_=sock+1, если это произошло (нужно для селекта в основном потоке)

  //}

  // Отправка даннных клиенту
  void tcp_server__send(TCPServer::Id id_, unsigned char const *v_, size_t &send_);//{
    //1. Ищем клиента в списке по id_

    //2. Добавляем данные на отправку для клиента в его буфер
  //}

  // Оповещение о получении данных
  virtual void tcp_server__recv(Id, unsigned char const *, size_t) = 0;

  // Оповещение о наличии соединения
  virtual void tcp_server__connected(Id, char const *cl_ip_, unsigned short cl_port_) = 0;

  // Оповещение о разрыве соединения
  virtual void tcp_server__disconnected(Id) = 0;


};  // TCPServer

} // eth

#endif // __ETH_TCP_SERVER__H__
