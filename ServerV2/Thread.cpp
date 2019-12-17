#include "Thread.h"
#include "Server.h"

using namespace std;

Thread::Thread(int n): sock(n)
{}

Thread::~Thread()
{}

void Thread::sendMsg(char* data)
{
    if (send(sock, data, strlen(data), 0) < 0) {
        cout << "Error sending" << endl;
    }
}

bool Thread::recvMsg(int sock, char* data)
{
    return (recv(sock, data, 255, 0) > 0);
}

int Thread::getSock()
{
    return sock;
}

void Thread::setSock(int x)
{
    this->sock = x;
}

//-----------------------//
Bed::Bed(int n): Thread(n)
{
    memset(data, 0, sizeof(data));
    strcpy(data, "leeg");
}

Bed::~Bed() {}

void Bed::operator()()
{
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    char test[64];
    strcpy(test, "test");
    setData(test);

    while(recvMsg(sock, buffer)) {

        if(buffer[0] == 'd')
        {
            strcpy(buffer, "ssss");
            setData(buffer);
        }
        else if(buffer[0] == 'f')
        {
            strcpy(buffer, "aaaa");
            setData(buffer);
        }

        sendMsg(buffer);

        memset(buffer, 0, sizeof(buffer));
    }
    close(sock);
    cout << "Connection closed on socket " << sock << endl;
}

void Bed::setData(char* s)
{
    memset(data, 0, sizeof(data));
    strcpy(data, s);
}

char* Bed::getData()
{
    return data;
}

/*void Bed::sendMsg(char* data)
  {
  if (send(sock, data, strlen(data), 0) < 0) {
  cout << "Error sending" << endl;
  }
  }*/

/*void Bed::setSock(int x)
  {
  this->sock = x;
  }*/


//-----------------------//
Gui::Gui(int n): Thread(n)
{}

Gui::~Gui() {}

void Gui::operator()()
{
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    while(recvMsg(sock, buffer)) {

        if (buffer[0] == 'b') { 
            strcpy(buffer, "doe iets");
            bed->sendMsg(buffer);
        }

        else if (buffer[0] == 'd') {
            strcpy(buffer, bed->getData());
            sendMsg(buffer);
        }

        else {
            strcpy(buffer, "echo");

            sendMsg(buffer);
        }

        memset(buffer, 0, sizeof(buffer));
    }
    close(sock);
    cout << "Connection closed on socket " << sock << endl;

    // ~Gui();
}

/*void Gui::sendMsg(char* data)
  {
  if (send(sock, data, strlen(data), 0) < 0) {
  cout << "Error sending" << endl;
  }
  }*/

void Gui::setBed(Bed* b)
{
    bed = b;
}

/*void Gui::setSock(int x)
  {
  this->sock = x;
  }

  int Gui::getSock()
  {
  return sock;
  }*/

