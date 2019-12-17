#ifndef THREAD_H
#define THREAD_H

#include "Server.h"
#include <string>

class Thread
{
    protected:
        int sock;
    public:
        Thread(int);
        virtual ~Thread();

        virtual void operator()() = 0;
        virtual void sendMsg(char*);
        virtual bool recvMsg(int, char*);

        int getSock();
        void setSock(int);
};

class Bed: public Thread
{
    private:
        char data[64];
    public:
        Bed(int);
        ~Bed();

        //  void sendMsg(char*);
        void operator()();
        void setData(char*);
        char* getData();
     //   void setSock(int);
};


class Gui: public Thread
{
    private:
        //  int sock;
        Bed *bed;
    public:
        Gui(int);
        ~Gui(); // <--- niet vergeten de aangemaakte pointers te deleten

        //    void sendMsg(char*);
        void setBed(Bed*);
        void operator()();
      //  void setSock(int);
      //  int getSock();
};

#endif
