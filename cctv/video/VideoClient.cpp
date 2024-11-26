#include "VideoClient.h"

namespace cctv
{
    VideoClient::VideoClient(const std::string& host, int port, std::unique_ptr<cipher::ICiphable> cipherHandler)
        : BaseClient(host, port, std::move(cipherHandler)) 
	{
	}

    VideoClient::~VideoClient() 
	{
	}

    void VideoClient::handleData(const void* buffer, size_t size)
    {
		std::string filePath = std::string(PROJECT_ROOT) + "/storage/" + mHost + ".h264";
		logger.Info("filePath: " + filePath);

		FILE* file = fopen(filePath.c_str(), "ab");
		if (!file)
		{
			logger.Error("Failed to open output file");
			return;
		}


		receiveFrames(file, saveFrameToFile);

		
		fclose(file);
	}
	
	void VideoClient::receiveFrames(FILE* file, SaveHandler saveFrameHandler)
	{
		while (true)
		{
			// 1. Header 수신
			logger.Info("Header receive");
			std::vector<uint8_t> headerBuffer(sizeof(frame::HeaderStruct));
			int headerResult = receiveData(headerBuffer.data(), sizeof(frame::HeaderStruct));
			logger.Debug("Header receive size");
			if (headerResult <= 0)
			{
				return;
			}

			// 2. Header 역직렬화
			logger.Info("Header deserialize");
			frame::Header header;
			header.Deserialize(headerBuffer);
			logger.Debug("Header received. FrameId: " + std::to_string(header.GetFrameId()) +
						", BodySize: " + std::to_string(header.GetBodySize()));

			// 3. Body 수신
			logger.Info("Body receive");
			std::vector<uint8_t> bodyBuffer(header.GetBodySize());
			int bodyResult = 0;
			if (header.GetBodySize() > 0)
			{
				bodyResult = receiveData(bodyBuffer.data(), header.GetBodySize());
			}

			if (bodyResult < 0)
			{
				return;
			}

			// Body 복호화
			std::vector<uint8_t> outDecrypted;
			decryptBody(bodyBuffer, header.GetTimestamp(), outDecrypted);

			// 4. Body 역직렬화 (Body가 0일 경우 처리하지 않음)
			logger.Info("Body deserialize");
			frame::Body body;
			if (bodyResult > 0)
			{
				body.Deserialize(outDecrypted);
			}

			// 5. Frame 객체 생성
			logger.Info("Frame init");
			frame::Frame frame(header, body);
			std::vector<uint8_t> frameBuffer;
			frame.Serialize(frameBuffer);
			
			// 6. 저장
			saveFrameHandler(file, reinterpret_cast<const char*>(frameBuffer.data()), frameBuffer.size());
		}
	}

	void VideoClient::saveFrameToFile(FILE* file, const char* frameData, size_t frameSize)
	{
		fwrite(frameData, sizeof(char), frameSize, file);
		logger.Info("Frame saved to file");
	}

	void VideoClient::decryptBody(const std::vector<uint8_t>& data, const std::string& timestamp, std::vector<uint8_t>& OUT decrypted)
	{
		std::vector<uint8_t> nonce(12, 0x00);
		std::copy(timestamp.end() - 12, timestamp.end(), nonce.begin());

		mCipherHandler->Decrypt(data, decrypted, nonce);
	}
}
