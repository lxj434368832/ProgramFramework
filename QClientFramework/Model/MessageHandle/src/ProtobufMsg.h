#pragma once

#include <memory>
#include <map>
#include "Message.pb.h"

using std::shared_ptr;

class BaseHandler
{

public:
    BaseHandler(pbmsg::MSG type):type_(type){
        Register (this);
    }
    virtual ~BaseHandler(){}
    pbmsg::MSG GetType() const { return type_; }
    //���崦��������������ʵ��.
    virtual void Process(unsigned uUserKey, const shared_ptr<pbmsg::Message> & msg, void* ptr) = 0;

    //ע����Ϣ������
    static void Register( BaseHandler *);

    //ִ��ָ������Ϣ,��ѯ������������Process��
    static void Execute(unsigned uUserKey, const shared_ptr<pbmsg::Message> & msg, void* ptr);

private:
    pbmsg::MSG type_;

private:
    static std::map<pbmsg::MSG , BaseHandler*> s_handers;
};

// ÿ����Ϣ��ʵ��Process��һ���ػ��汾
template< pbmsg::MSG Type>
class MessageHandler : public BaseHandler
{
public:
MessageHandler(void):BaseHandler(Type){}
    ~MessageHandler(void){}

    void Process(unsigned uUserKey, const shared_ptr<pbmsg::Message> & msg, void* ptr);

private:
    static MessageHandler thisHandler;
};
