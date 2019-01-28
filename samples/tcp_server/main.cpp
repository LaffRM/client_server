#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "../../common/eth/tcp_server.h"
#include <string>
#include <map>

#define DEFAULT_TEST_SERVER_TCP_PORT 5972
#define DEFAULT_TEST_SERVER_TCP_IP "127.0.0.1"
#define CONST_CFG_NAME "server.cfg"


bool work=true;


class TestServer: private eth::TCPServer {

public:

  TestServer(char const *sv_ip_, unsigned short sv_port_, char const *cfg_file_name_): TCPServer(cfg_file_name_){
      char val[MAXLENVALUE];

      TCPServer::cfg_file__->update(cfg_file_name_, "global");

      srv_ip__ = TCPServer::cfg_file__->u("ip", sv_ip_, val);
      srv_port__ = TCPServer::cfg_file__->u("port", (int)sv_port_);
  };

  ~TestServer(){};

  // Инициализация объекта
  bool init(){
    return tcp_server__init(srv_ip__.c_str(), srv_port__);
  }

  // Добавить сокеты "на прослушку" (для коннекшенов по умолчанию ожидается только чтение)
  void socket_add(fd_set &rd_, fd_set &wr_, int &n_) {
    tcp_server__socket_add(rd_, wr_, n_);
  }

  // Проверить события на сокете
  void socket_check (fd_set const &rd_, fd_set const &wr_){
    tcp_server__socket_check(rd_, wr_);
  }

  void send(TCPServer::Id id_, unsigned char const *v_, size_t &send_){
    tcp_server__send(id_, v_, send_);
  }
private:

  std::string    srv_ip__;
  unsigned short srv_port__;
  char const *cfg_file_name___;

  // Получение данных
  virtual void tcp_server__recv(TCPServer::Id id_, unsigned char const *buf_, size_t len_){

    std::string buf((char const *)buf_, len_);

    printf("tcp_server__recv(%zd):%s", len_, buf.c_str());


    //Для теста отправим то что пришло от клиета обратно...

    send(id_, buf_, len_);


  };

  // Оповещение о наличии соединения
  virtual void tcp_server__connected(TCPServer::Id id_, char const *cl_ip_, unsigned short cl_port_){

    printf("tcp_server__connected(%zd) %s:%d\n", id_, cl_ip_, cl_port_);

    char buf[MAX_TCP_SERVER_BUF_SIZE] = {'\0'};

    std::string str = "Test TCP server:\n";
    size_t try_send = str.length();
    send(id_, (unsigned char const *)str.c_str(), try_send);

    sprintf(buf, "Client %lu connected", id_);

    log_file->write(buf);
    log_file_b->writeb("", 0, buf);
  };

  // Оповещение о разрыве соединения
  virtual void tcp_server__disconnected(Id id_){
    char buf[MAX_TCP_SERVER_BUF_SIZE] = {'\0'};

    printf("tcp_server__disconnected(%zd)\n", id_);

    sprintf(buf, "client %lu disconnected", id_);

    log_file->write(buf);
    log_file_b->writeb("", 0, buf);
  };

};

int main(int argc, char *argv[]){

  char a[MAXLENVALUE];

  if (argc > 1)
    strcpy(a, argv[1]);
  else
    strcpy(a, CONST_CFG_NAME);


  TestServer tcp_serv(DEFAULT_TEST_SERVER_TCP_IP, DEFAULT_TEST_SERVER_TCP_PORT, a);

  if (tcp_serv.init()==false) {
    printf("TestServer init error\n");
    return 1;
  }

  printf("Start TCP TestServer\n");

  fd_set  fds_rd, fds_wr;
  timeval tv;
  int     n;
  int     retcode = 0;



  while (work) {

    FD_ZERO(&fds_rd);
    FD_ZERO(&fds_wr);
    n=0;

    tcp_serv.socket_add(fds_rd, fds_wr, n);

    tv.tv_sec=1;
    tv.tv_usec=0;
    switch (select(n, &fds_rd, &fds_wr, NULL, &tv)) {

      case -1:
        if(work==false) break;
        if (errno==EINTR) {
          if (work==true) {
            printf("\nWARNING: main(): select(): %s\n", strerror(errno));
          }
          retcode=1;
        }
        printf("ERROR: main(): select(): %s\n", strerror(errno));
        work=false;
        break;

      case 0:
        break;

      default:

        tcp_serv.socket_check(fds_rd, fds_wr);


        break;

    }

  }


  return retcode;
}
