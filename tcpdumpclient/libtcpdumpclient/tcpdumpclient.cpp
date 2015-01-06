#include <jni.h>  

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



#define DEBUG_TAG "Sample_LIBPCAP_DEBUGGING"  



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
            // std::cout << "client string: " << result << std::endl;

            reply.print(PLOG); endl(PLOG);
            LOGD("TcpdumpClient::push(%s)", push_data);
            return result;
        }

        virtual std::string getDataPacketFromIPJ(char *ip) {
            LOGD(ip);
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



std::string send(const char* cmd) {
    std::string result = "resultjnifuck\nfuck\nx x x xx ";
    sp<ITcpdumpInterface> demo = getDemoServ();
    std::string str = demo->getDataPacketFromIPJ("10.0.2.15");
    return str;
}

void jstring2char(JNIEnv* env, jstring jstring, char* string) {
    jboolean isCopy;
    const char* res = env->GetStringUTFChars(jstring, &isCopy); 
    if (isCopy == JNI_TRUE) { 
        (env)->ReleaseStringUTFChars(jstring, res); 
    }
    strcpy(string, res);
}


extern "C" JNIEXPORT jstring JNICALL
Java_com_example_tcpdumpclient_MainActivity_query
(JNIEnv* env, jobject javaThis , jstring jcode, jstring jarg0, jstring jarg1 ) {
    // Convert jstring to char
    char code[256];
    char arg0[256];
    char arg1[256];
    jstring2char(env, jcode, code);
    jstring2char(env, jarg0, arg0);
    jstring2char(env, jarg1, arg1);
    sp<ITcpdumpInterface> demo = getDemoServ();
    std::string result;
    if (strcmp(code, "1") == 0){
        result = demo -> getDataPacketFromIPJ(arg0);
    } else 
    if (strcmp(code, "2") == 0){
        result = demo -> getDataPacketToIPJ(arg0);
    } else 
    if (strcmp(code, "3") == 0){
        result = demo -> getDataPacketFromIPAndPortJ(arg0, arg1);
    } else 
    if (strcmp(code, "4") == 0){
        result = demo -> getDataPacketFromPortJ(arg0);
    } else 
    if (strcmp(code, "5") == 0){
        result = demo -> getDataPacketToPortJ(arg0);
    } else 
    if (strcmp(code, "6") == 0){
        result = demo -> getDataPacketFromIPToIPJ(arg0, arg1);
    } else 
    if (strcmp(code, "7") == 0){
        result = demo -> getDataPacketFromMacJ(arg0);
    } else 
    if (strcmp(code, "8") == 0){
        result = demo -> getDataPacketToMacJ(arg0);
    } else 
    if (strcmp(code, "9") == 0){
        result = demo -> getDataPacketFromMacToMacJ(arg0, arg1);
    } else 
    if (strcmp(code, "10") == 0){
        result = demo -> getDataPacketWithProtocolJ(arg0);
    }

    return (env)->NewStringUTF((const char* )result.c_str());
}