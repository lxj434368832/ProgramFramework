#include "ProtobufMsg.h"


void BaseHandler::Register( BaseHandler * h )
{
    s_handers[h->GetType()] = h;
}

void BaseHandler::Execute(unsigned uUserKey, const shared_ptr<pbmsg::Message> & msg, void* ptr)
{
    auto it = s_handers.find(msg->msg_type());

    if( it != s_handers.end ())
    {
        it->second->Process(uUserKey, msg, ptr);
    }
    else
    {
        //LOG(ERROR) << "消息 " << msg->type()<< " 没有对应的处理方法.\n";
    }
}

//对每个MSG 枚举的消息值，都会特化一个Process方法。
template<pbmsg::MSG Type>
void MessageHandler<pbmsg::Login_Request>::Process(unsigned uUserKey, const shared_ptr<pbmsg::Message> & msg, void* ptr)
{

}

//在全局空间创建对象，系统启动时，自动创建。如果需要在堆空间中分配，另行封装方法，并调用下面的代码，让编译器实例化类。
MessageHandler<pbmsg::Login_Request> MessageHandler<pbmsg::Login_Request>::thisHandler;



// 最后消息处理,非常的easy:
void HandleMsg()
{
	std::string msg;
	unsigned uUserKey = 1;
    shared_ptr<pbmsg::Message> recvMessage( new pbmsg::Message());
    bool parserOk = recvMessage->ParseFromArray((msg.c_str()+ 4), msg.size() - 4);
    if( parserOk )
    {
        BaseHandler::Execute (uUserKey,recvMessage, nullptr);
    }
}



//消息定义：

//package MsgPb;

//message Msg {
//  required string type = 1;  // Full type name of data.
//  required bytes data = 2;  // Serialized bytes fo concrete msg.
//}

//消息发送代码：
/*
void MsgSender::Send(const std::string & sDest, const MsgPb::Msg & msg)
{
    std::string s;
    bool bSuc = msg.SerializeToString(&s);
    BOOST_ASSERT(bSuc);
    Send(sDest, s);
}

void MsgSender::Send(const std::string & sDest,
    const google::protobuf::Message & msg) const
{
    MsgPb::Msg msgSend;
    msgSend.set_type(msg.GetTypeName());
    bool bSuc = msg.SerializeToString(msgSend.mutable_data());
    BOOST_ASSERT(bSuc);
    Send(sDest, msgSend);
}

//消息接收后分发：

void HandleOneMsg(const string & sFrom, const string & sMsg)
{
    MsgPb::Msg msg;
    bool bSuc = msg.ParseFromString(sMsg);
    if (!bSuc) return;
    MsgDispatcher::Dispatch(sFrom, msg);
}

void MsgDispatcher::Dispatch(const string & sFrom, const MsgPb::Msg & msg)
{
    HandlerMap::const_iterator itr = s_mapHandlers.find(msg.type());
    if (itr == s_mapHandler.end()) return;

    MsgPb::GoogleMsgPtr pMsg = MsgPb::ParseMsg(msg.type(), msg.data());

    if (pMsg)
        (*itr).second(sFrom, *pMsg);
}

消息解析：

typedef google::protobuf::Message GoogleMsg;
typedef boost::shared_ptr<GoogleMsg> GoogleMsgPtr;

// ParseMsg.cpp
#include "ParseMsg.h"

#include <google/protobuf/descriptor.h>
#include <googlt/protobuf/message.h>

namespace {

using MsgPb::GoogleMsgPtr;
GoogleMsgPtr GreateMsg(const string & sTypeName)
{
    using namespace google::protobuf;
    const Descriptor * pDescriptor = DescriptorPool::generated_pool();
        ->FindMessageTypeByName(sTypeName);
    if (NULL == pDescriptor)
        return GoogleMsgPtr();
    const Message * pPrototype = MessageFcatory::generated_factory();
        ->GetPrototype(pDescriptor);
    if (NULL == pPrototype)
        return GoogleMsgPtr();
    return GoogleMsgPtr(pPrototype->New());
}

}  // namespace

namespace MsgPb {

GoogleMsgPtr ParseMsg(const string & sTypeName, const string & sData)
{
    GoogleMsgPtr pMsg = CreateMsg(sTypeName);
    if (!pMsg) return GoogleMsgPtr();
    bool bSuc = pMsg->ParseFromString(sData);
    if (bSuc) return pMsg;
    return GoogleMsgPtr();
}

}  // namespce MsgPb

注册处理器：

void MsgDispatcher::Init()
{
    using namespace MsgPb;
    InsertHandler(LobbyRegisterMsg(), LobbyRegisterMsgHandler());
    InsertHandler(LoginMsg(), LoginMsgHandler());
    ...
}

void MsgDispatcher::InsertHandler(const MsgPb::GoogleMsg & msg,
                                    const MsgHandler & h)
{
    s_mapHandlers[msg.GetTypeName()] = h;
}


//定义一个结构体
struct SMsgCmd {
                int msgtype;					 //msg type
                int (*func)(const char *argv);   //handler * 函数指针
            };

            //定义一个结构体数组，并初始化
static SMsgCmd commands[] = {
                                    { 1,   Request1},
                                    { 2,   Request1},
                                   };

//在服务程序的消息入口处，遍历改数组即可
void handData()
{
    for (int i = 0; i < commands.size(); i++)
    {
        if (msgtype == commands[i].msgtype)
        {
            (*commands[i].func)(argv);
        }
    }
}
*/