#ifndef CHEESYVISIONSERVER_H_
#define CHEESYVISIONSERVER_H_

#include "../Process/AsyncProcess.h"
#include "networktables2/stream/SocketServerStreamProvider.h"

class CheesyVisionServer: private AsyncProcess
{
    static CheesyVisionServer *_instance;
    
    
    CheesyVisionServer(int port = 1180);
    int _listenPort;
    
    int _leftCount;
    int _rightCount;
    int _totalCount;
    bool _curLeftStatus;
    bool _curRightStatus;
    bool _counting;
    
    double _lastHeartbeatTime;
    bool _listening;
public:
    static CheesyVisionServer *GetInstance();
    static void Initialize();
    bool HasClientConnection();
    void SetPort(int port){_listenPort = port;}
    virtual void Tick();
    void StartListening(){_listening = true; _instance->Start();}
    void StopListening(){_listening = false; _instance->Abort();}
    
    void Reset();
    void UpdateCounts(bool left, bool right);
    void StartSamplingCounts(){_counting = true;}
    void StopSamplingCounts(){_counting = false;}

    bool GetLeftStatus(){return _curLeftStatus;}
    bool GetRightStatus(){return _curRightStatus;}
    int GetLeftCount(){return _leftCount;}
    int GetRightCount(){return _rightCount;}
    int GetTotalCount(){return _totalCount;}
    ~CheesyVisionServer();
};


#endif //CHEESYVISIONSERVER_H_
