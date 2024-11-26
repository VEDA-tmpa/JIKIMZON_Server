#ifndef BASE_CLIENT_H
#define BASE_CLIENT_H

#include <string>
#include <memory>

#include "common/log/Logger.h"
#include "common/cipher/ICiphable.h"

namespace tcp
{
    class BaseClient
    {
    public:
        BaseClient(const std::string& host, int port, std::unique_ptr<cipher::ICiphable> cipherHandler);
        virtual ~BaseClient();

        void Start();
        void Close();

    protected:
        virtual void handleData(const void* buffer, size_t size) = 0;
        
		int receiveData(void* buffer, size_t size);

        static logger::Logger logger;

        std::string mHost;
        int mPort;
        int mSocketFd;
        bool mbClosed;

        std::unique_ptr<cipher::ICiphable> mCipherHandler;

    private:
        void connectToServer();
    };
}
#endif /* BASE_CLIENT_H */
