#pragma once

#include <Message.pb.h>
#include <memory>
#include <map>
#include "Client.h"

using std::shared_ptr;

class BaseHandler
{

public:
    BaseHandler(pbmsg::MSG type):type_(type){
        Register (this);
    }
    virtual ~BaseHandler(){}
    pbmsg::MSG GetType() const { return type_; }
    //具体处理方法，由派生类实现.
    virtual void Process( const shared_ptr<pbmsg::Message> & msg, const shared_ptr<Client> & client) = 0;

    //注册消息处理方法
    static void Register( BaseHandler *);

    //执行指定的消息,查询处理方法，调用Process。
    static void Execute( const shared_ptr<pbmsg::Message> & msg, const shared_ptr<Client> & client);

private:
    pbmsg::MSG type_;

private:
    static std::map<pbmsg::MSG , BaseHandler *> handers;

};

// 每个消息都实现Process的一个特化版本
template< pbmsg::MSG Type>
class MessageHandler : public BaseHandler
{
public:
MessageHandler(void):BaseHandler(Type){}
    ~MessageHandler(void){}

    void Process( const shared_ptr<pbmsg::Message> & msg, const shared_ptr<Client> & client);

private:
    static MessageHandler thisHandler;

};
