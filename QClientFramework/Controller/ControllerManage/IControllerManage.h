#pragma once

class IMainClient;
class ModelManage;

class IControllerManage
{
public:
    IControllerManage(IMainClient *_main = nullptr):m_main(_main){}
    virtual ~IControllerManage() { m_main = nullptr; }

    virtual bool Start() = 0;
    virtual void Stop() = 0;

protected:

    IMainClient *m_main = nullptr;
};

