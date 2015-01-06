/* -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   vi:ai:tabstop=8:shiftwidth=4:softtabstop=4:expandtab
*/

/*
 * Author: Gabriel Burca <gburca dash binder at ebixio dot com>
 *
 * Sample code for using binders in Android from C++
 *
 * The Demo service provides 3 operations: push(), alert(), add(). See
 * the IDemo class documentation to see what they do.
 *
 * Both the server and client code are included below.
 *
 * To view the log output:
 *      adb logcat -v time tcpdumpservice:* *:S
 *
 * To run, create 2 adb shell sessions. In the first one run "binder" with no
 * arguments to start the service. In the second one run "binder N" where N is
 * an integer, to start a client that connects to the service and calls push(N),
 * alert(), and add(N, 5).
 */

#define LOG_TAG "tcpdumpservice"

/* For relevant code see:
    frameworks/base/{include,libs}/binder/{IInterface,Parcel}.h
    frameworks/base/include/utils/{Errors,RefBase}.h
 */

#include <stdlib.h>

#include "utils/RefBase.h"
#include "utils/Log.h"
#include "binder/TextOutput.h"
#define LOGD ALOGD

#include <binder/IInterface.h>
#include <binder/IBinder.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>
#include <string>
#include <iostream>

extern "C" {
    #include <tcpdumpapi.h>
}


using namespace android;


#define INFO(...) \
    do { \
        printf(__VA_ARGS__); \
        printf("\n"); \
        LOGD(__VA_ARGS__); \
    } while(0)

void assert_fail(const char *file, int line, const char *func, const char *expr) {
    INFO("assertion failed at file %s, line %d, function %s:",
            file, line, func);
    INFO("%s", expr);
    abort();
}

#define ASSERT(e) \
    do { \
        if (!(e)) \
            assert_fail(__FILE__, __LINE__, __func__, #e); \
    } while(0)


// Where to print the parcel contents: aout, alog, aerr. alog doesn't seem to work.
#define PLOG aout



// Interface (our AIDL) - Shared by server and client
class ITcpdumpInterface : public IInterface {
    public:
        // Sends a user-provided value to the service
        virtual std::string push(char *data) = 0;
        virtual std::string getDataPacketFromIPJ(char *ip) = 0;
        virtual std::string getDataPacketToIPJ(char *ip) = 0;
        virtual std::string getDataPacketFromIPAndPortJ(char *ip, char *port) = 0;
        virtual std::string getDataPacketFromPortJ(char *port) = 0;
        virtual std::string getDataPacketToPortJ(char *port) = 0;        
        virtual std::string getDataPacketFromIPToIPJ(char *ip, char* ipto) = 0;   
        virtual std::string getDataPacketFromMacJ(char *mac) = 0;   
        virtual std::string getDataPacketToMacJ(char *mac) = 0;   
        virtual std::string getDataPacketFromMacToMacJ(char *mac, char* macto) = 0;   
        virtual std::string getDataPacketWithProtocolJ(char *type) = 0;   

        DECLARE_META_INTERFACE(TcpdumpInterface);  // Expands to 5 lines below:
        //static const android::String16 descriptor;
        //static android::sp<IDemo> asInterface(const android::sp<android::IBinder>& obj);
        //virtual const android::String16& getInterfaceDescriptor() const;
        //IDemo();
        //virtual ~IDemo();
};

// Client
class TcpdumpClient : public BpInterface<ITcpdumpInterface> {
    public:
        TcpdumpClient(const sp<IBinder>& impl) : BpInterface<ITcpdumpInterface>(impl) {
            LOGD("TcpdumpClient::TcpdumpClient()");
        }

        virtual std::string push(char *push_data) {
            Parcel data, reply;
            data.writeInterfaceToken(ITcpdumpInterface::getInterfaceDescriptor());
            data.writeCString(push_data);
            // reply.writeInterfaceToken(ITcpdumpInterface::getInterfaceDescriptor());

            aout << "TcpdumpClient::push parcel to be sent:\n";
            data.print(PLOG); endl(PLOG);

            remote()->transact(1, data, &reply);

            aout << "TcpdumpClient::push parcel reply:\n";
            // const char* tmp = reply.readCString();
            // std::cout << "client char: " << tmp << std::endl;
            std::string result = std::string((const char*)reply.readCString());
            std::cout << "client string: " << result << std::endl;

            reply.print(PLOG); endl(PLOG);
            LOGD("TcpdumpClient::push(%s)", push_data);
            return result;
        }

        virtual std::string getDataPacketFromIPJ(char *ip) {
            Parcel data, reply;
            data.writeInterfaceToken(ITcpdumpInterface::getInterfaceDescriptor());
            data.writeCString(ip);

            // aout << "TcpdumpClient::push parcel to be sent:\n";
            // data.print(PLOG); endl(PLOG);

            remote()->transact(1, data, &reply);

            // aout << "TcpdumpClient::push parcel reply:\n";
            // const char* tmp = reply.readCString();
            std::string result = std::string((const char*)reply.readCString());
            // reply.print(PLOG); endl(PLOG);
            // LOGD("TcpdumpClient::push(%s)", IP);
            return result;
        }

        virtual std::string getDataPacketToIPJ(char *ip) {
            Parcel data, reply;
            data.writeInterfaceToken(ITcpdumpInterface::getInterfaceDescriptor());
            data.writeCString(ip);

            // aout << "TcpdumpClient::push parcel to be sent:\n";
            // data.print(PLOG); endl(PLOG);

            remote()->transact(2, data, &reply);

            // aout << "TcpdumpClient::push parcel reply:\n";
            // const char* tmp = reply.readCString();
            std::string result = std::string((const char*)reply.readCString());
            // reply.print(PLOG); endl(PLOG);
            // LOGD("TcpdumpClient::push(%s)", IP);
            return result;
        }

        virtual std::string getDataPacketFromIPAndPortJ(char *ip, char *port){
            Parcel data, reply;
            data.writeInterfaceToken(ITcpdumpInterface::getInterfaceDescriptor());
            data.writeCString(ip);
            data.writeCString(port);

            // aout << "TcpdumpClient::push parcel to be sent:\n";
            // data.print(PLOG); endl(PLOG);

            remote()->transact(3, data, &reply);

            // aout << "TcpdumpClient::push parcel reply:\n";
            // const char* tmp = reply.readCString();
            std::string result = std::string((const char*)reply.readCString());
            // reply.print(PLOG); endl(PLOG);
            // LOGD("TcpdumpClient::push(%s)", IP);
            return result;
        }

        virtual std::string getDataPacketFromPortJ(char *port) {
            Parcel data, reply;
            data.writeInterfaceToken(ITcpdumpInterface::getInterfaceDescriptor());
            data.writeCString(port);

            // aout << "TcpdumpClient::push parcel to be sent:\n";
            // data.print(PLOG); endl(PLOG);

            remote()->transact(4, data, &reply);

            // aout << "TcpdumpClient::push parcel reply:\n";
            // const char* tmp = reply.readCString();
            std::string result = std::string((const char*)reply.readCString());
            // reply.print(PLOG); endl(PLOG);
            // LOGD("TcpdumpClient::push(%s)", IP);
            return result;
        }

        virtual std::string getDataPacketToPortJ(char *port) {
            Parcel data, reply;
            data.writeInterfaceToken(ITcpdumpInterface::getInterfaceDescriptor());
            data.writeCString(port);

            // aout << "TcpdumpClient::push parcel to be sent:\n";
            // data.print(PLOG); endl(PLOG);

            remote()->transact(5, data, &reply);

            // aout << "TcpdumpClient::push parcel reply:\n";
            // const char* tmp = reply.readCString();
            std::string result = std::string((const char*)reply.readCString());
            // reply.print(PLOG); endl(PLOG);
            // LOGD("TcpdumpClient::push(%s)", IP);
            return result;
        }

        virtual std::string getDataPacketFromIPToIPJ(char *ip, char *ipto){
            Parcel data, reply;
            data.writeInterfaceToken(ITcpdumpInterface::getInterfaceDescriptor());
            data.writeCString(ip);
            data.writeCString(ipto);

            // aout << "TcpdumpClient::push parcel to be sent:\n";
            // data.print(PLOG); endl(PLOG);

            remote()->transact(6, data, &reply);

            // aout << "TcpdumpClient::push parcel reply:\n";
            // const char* tmp = reply.readCString();
            std::string result = std::string((const char*)reply.readCString());
            // reply.print(PLOG); endl(PLOG);
            // LOGD("TcpdumpClient::push(%s)", IP);
            return result;
        }

        virtual std::string getDataPacketFromMacJ(char *mac){
            Parcel data, reply;
            data.writeInterfaceToken(ITcpdumpInterface::getInterfaceDescriptor());
            data.writeCString(mac);

            // aout << "TcpdumpClient::push parcel to be sent:\n";
            // data.print(PLOG); endl(PLOG);

            remote()->transact(7, data, &reply);

            // aout << "TcpdumpClient::push parcel reply:\n";
            // const char* tmp = reply.readCString();
            std::string result = std::string((const char*)reply.readCString());
            // reply.print(PLOG); endl(PLOG);
            // LOGD("TcpdumpClient::push(%s)", IP);
            return result;
        }

        virtual std::string getDataPacketToMacJ(char *mac){
            Parcel data, reply;
            data.writeInterfaceToken(ITcpdumpInterface::getInterfaceDescriptor());
            data.writeCString(mac);

            // aout << "TcpdumpClient::push parcel to be sent:\n";
            // data.print(PLOG); endl(PLOG);

            remote()->transact(8, data, &reply);

            // aout << "TcpdumpClient::push parcel reply:\n";
            // const char* tmp = reply.readCString();
            std::string result = std::string((const char*)reply.readCString());
            // reply.print(PLOG); endl(PLOG);
            // LOGD("TcpdumpClient::push(%s)", IP);
            return result;
        }

        virtual std::string getDataPacketFromMacToMacJ(char *mac, char* macto){
            Parcel data, reply;
            data.writeInterfaceToken(ITcpdumpInterface::getInterfaceDescriptor());
            data.writeCString(mac);
            data.writeCString(macto);

            // aout << "TcpdumpClient::push parcel to be sent:\n";
            // data.print(PLOG); endl(PLOG);

            remote()->transact(9, data, &reply);

            // aout << "TcpdumpClient::push parcel reply:\n";
            // const char* tmp = reply.readCString();
            std::string result = std::string((const char*)reply.readCString());
            // reply.print(PLOG); endl(PLOG);
            // LOGD("TcpdumpClient::push(%s)", IP);
            return result;
        }

        virtual std::string getDataPacketWithProtocolJ(char *type){
            Parcel data, reply;
            data.writeInterfaceToken(ITcpdumpInterface::getInterfaceDescriptor());
            data.writeCString(type);

            // aout << "TcpdumpClient::push parcel to be sent:\n";
            // data.print(PLOG); endl(PLOG);

            remote()->transact(10, data, &reply);

            // aout << "TcpdumpClient::push parcel reply:\n";
            // const char* tmp = reply.readCString();
            std::string result = std::string((const char*)reply.readCString());
            // reply.print(PLOG); endl(PLOG);
            // LOGD("TcpdumpClient::push(%s)", IP);
            return result;
        }
};

    //IMPLEMENT_META_INTERFACE(Demo, "Demo");
    // Macro above expands to code below. Doing it by hand so we can log ctor and destructor calls.
    const android::String16 ITcpdumpInterface::descriptor("Tcpdump");
    const android::String16& ITcpdumpInterface::getInterfaceDescriptor() const {
        return ITcpdumpInterface::descriptor;
    }
    android::sp<ITcpdumpInterface> ITcpdumpInterface::asInterface(const android::sp<android::IBinder>& obj) {
        android::sp<ITcpdumpInterface> intr;
        if (obj != NULL) {
            intr = static_cast<ITcpdumpInterface*>(obj->queryLocalInterface(ITcpdumpInterface::descriptor).get());
            if (intr == NULL) {
                intr = new TcpdumpClient(obj);
            }
        }
        return intr;
    }
    ITcpdumpInterface::ITcpdumpInterface() { LOGD("ITcpdumpInterface::ITcpdumpInterface()"); }
    ITcpdumpInterface::~ITcpdumpInterface() { LOGD("ITcpdumpInterface::~ITcpdumpInterface()"); }
    // End of macro expansion

// Server
class TcpdumpService : public BnInterface<ITcpdumpInterface> {
    virtual status_t onTransact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags = 0){
        static char packet[409600];
        char* ip;
        char* port;
        char* mac;
        char* ipto;
        char* macto;
        char* type;

        LOGD("TcpdumpService::onTransact(%i) %i", code, flags);
        data.checkInterface(this);
        data.print(PLOG); endl(PLOG);

        // const char* tmp = data.readCString();
        // std::cout << "server char: " << tmp << std::endl;
        // std::string result = std::string(tmp);
        // std::cout << "server string: " << result << std::endl;
        // // aout << inData;
        // // LOGD("TcpdumpService::onTransact got %s", inData);
        // // push(inData);
        // ASSERT(reply != 0);
        // // reply->writeInterfaceToken(ITcpdumpInterface::getInterfaceDescriptor());
        // reply->writeCString("test\nline1");
        // reply->print(PLOG); endl(PLOG);
        switch(code){
            case 1 :
                ip = (char*)data.readCString();                
                if (!getDataPacketFromIP(ip, packet)){
                    LOGD("getDataPacketFromIP error");
                }
                ASSERT(reply != 0);
                reply->writeCString(packet);
                reply->print(PLOG); endl(PLOG);
                break;
            
            case 2 :
                ip = (char*)data.readCString();                
                if (!getDataPacketToIP(ip, packet)){
                    LOGD("getDataPacketToIP error");
                }
                ASSERT(reply != 0);
                reply->writeCString(packet);
                reply->print(PLOG); endl(PLOG);
                break;

            case 3 :
                ip = (char*)data.readCString();                
                port = (char*)data.readCString();
                if (!getDataPacketFromIPAndPort(ip, port, packet)){
                    LOGD("getDataPacketFromIPAndPort error");
                }
                ASSERT(reply != 0);
                reply->writeCString(packet);
                reply->print(PLOG); endl(PLOG);
                break;

            case 4 :              
                port = (char*)data.readCString();
                if (!getDataPacketFromPort(port, packet)){
                    LOGD("getDataPacketFromPort error");
                }
                ASSERT(reply != 0);
                reply->writeCString(packet);
                reply->print(PLOG); endl(PLOG);
                break;

            case 5 :              
                port = (char*)data.readCString();
                if (!getDataPacketToPort(port, packet)){
                    LOGD("getDataPacketToPort error");
                }
                ASSERT(reply != 0);
                reply->writeCString(packet);
                reply->print(PLOG); endl(PLOG);
                break;

            case 6 :              
                ip = (char*)data.readCString();
                ipto = (char*)data.readCString();
                if (!getDataPacketFromIPToIP(ip, ipto, packet)){
                    LOGD("getDataPacketFromIPToIP error");
                }
                ASSERT(reply != 0);
                reply->writeCString(packet);
                reply->print(PLOG); endl(PLOG);
                break;

            case 7 :              
                mac = (char*)data.readCString();
                if (!getDataPacketFromMac(mac, packet)){
                    LOGD("getDataPacketFromMac error");
                }
                ASSERT(reply != 0);
                reply->writeCString(packet);
                reply->print(PLOG); endl(PLOG);
                break;

            case 8 :              
                mac = (char*)data.readCString();
                if (!getDataPacketToMac(mac, packet)){
                    LOGD("getDataPacketToMac error");
                }
                ASSERT(reply != 0);
                reply->writeCString(packet);
                reply->print(PLOG); endl(PLOG);
                break;

            case 9 :              
                mac = (char*)data.readCString();
                macto = (char*)data.readCString();
                if (!getDataPacketFromMacToMac(mac, macto, packet)){
                    LOGD("getDataPacketFromMacToMac error");
                }
                ASSERT(reply != 0);
                reply->writeCString(packet);
                reply->print(PLOG); endl(PLOG);
                break;

            case 10 :              
                type = (char*)data.readCString();
                if (!getDataPacketWithProtocol (type, packet)){
                    LOGD("getDataPacketWithProtocol error");
                }
                ASSERT(reply != 0);
                reply->writeCString(packet);
                reply->print(PLOG); endl(PLOG);
                break;
        }

        return NO_ERROR;
    }
    virtual std::string push(char *data) {
        INFO("Demo::push(%s)", data);
        return data;
    }
    virtual std::string getDataPacketFromIPJ(char *ip){
        return "";
    }
    virtual std::string getDataPacketToIPJ(char *ip){
        return "";
    }
    virtual std::string getDataPacketFromIPAndPortJ(char *ip, char *port){
        return "";
    };
    virtual std::string getDataPacketFromPortJ(char *port){
        return "";
    }
    virtual std::string getDataPacketToPortJ(char *port){
        return "";
    }
    virtual std::string getDataPacketFromIPToIPJ(char *ip, char *ipto){
        return "";
    }
    virtual std::string getDataPacketFromMacJ(char *mac){
        return "";
    }
    virtual std::string getDataPacketToMacJ(char *mac){
        return "";
    }
    virtual std::string getDataPacketFromMacToMacJ(char *mac, char *macto){
        return "";
    }
    virtual std::string getDataPacketWithProtocolJ(char *type){
        return "";
    }

};



// Helper function to get a hold of the "Demo" service.
sp<ITcpdumpInterface> getDemoServ() {
    sp<IServiceManager> sm = defaultServiceManager();
    ASSERT(sm != 0);
    sp<IBinder> binder = sm->getService(String16("Tcpdump"));
    // TODO: If the "Demo" service is not running, getService times out and binder == 0.
    ASSERT(binder != 0);
    sp<ITcpdumpInterface> demo = interface_cast<ITcpdumpInterface>(binder);
    ASSERT(demo != 0);
    return demo;
}


int main(int argc, char **argv) {
    if (argc == 1) {
        LOGD("We're the service");

        defaultServiceManager()->addService(String16("Tcpdump"), new TcpdumpService());
        android::ProcessState::self()->startThreadPool();
        LOGD("Demo service is now ready");
        IPCThreadState::self()->joinThreadPool();
        LOGD("Demo service thread joined");
    } else if (argc == 2) {
        INFO("We're the client: %s", argv[1]);

        char *v = argv[1];

        sp<ITcpdumpInterface> demo = getDemoServ();
        std::cout << demo->getDataPacketFromIPJ("10.0.2.15") << std::endl;
        std::cout << demo->getDataPacketToIPJ("10.0.2.15") << std::endl;
        std::cout << demo->getDataPacketFromIPAndPortJ("10.0.2.1", "80") << std::endl;
        std::cout << demo->getDataPacketFromPortJ("80") << std::endl;
        std::cout << demo->getDataPacketFromIPToIPJ("10.0.2.15", "10.0.2.1") << std::endl;
        std::cout << demo->getDataPacketFromMacJ("52:54:00:12:34:56") << std::endl;
        std::cout << demo->getDataPacketToMacJ("52:54:00:12:34:56") << std::endl;
        std::cout << demo->getDataPacketFromMacToMacJ("52:54:00:12:34:56", "52:54:00:12:34:56") << std::endl;
        std::cout << demo->getDataPacketWithProtocolJ("icmp") << std::endl;
    }
    return 0;
}

/*
    Single-threaded service, single-threaded client.
 */
