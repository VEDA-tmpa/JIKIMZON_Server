#include "VideoClient.h"

#include "common/frame/GOP.h"

namespace cctv
{
    VideoClient::VideoClient(const std::string& host, int port, std::unique_ptr<cipher::ICiphable> cipherHandler)
        : BaseClient(host, port, std::move(cipherHandler)) 
		, mStorageManager(host)
	{
	}

    void VideoClient::handleData()
    {
		while (true)
		{
			frame::Frame frame = receiveFrame();

			frame::Header header = frame.GetHeader();
			if (static_cast<frame::GOP_START_FLAG>(header.GetGopStartFlag()) == frame::GOP_START_FLAG::TRUE)
			{
				std::vector<frame::Frame> frames;
				frames.reserve(header.GetGopSize());
				frames.push_back(frame);

				for (int i = 0; i < header.GetGopSize() - 1; ++i)
				{
					frames.push_back(receiveFrame());
				}

				frame::H264::GOP gop(frames);
				mStorageManager.SaveData(gop);
			}	
		}

		mbClosed = true;
	}
	
	frame::Frame VideoClient::receiveFrame()
	{
		// 1. Header 수신
		logger.Info("Header receive");
		std::vector<uint8_t> headerBuffer(sizeof(frame::HeaderStruct));
		int headerResult = receiveData(headerBuffer.data(), sizeof(frame::HeaderStruct));
		logger.Debug("Header receive size");
		if (headerResult <= 0)
		{
			logger.Error("Failed to receive header");
			throw std::runtime_error("Failed to receive header");
		}

		// 2. Header 역직렬화
		logger.Info("Header deserialize");
		frame::Header header;
		header.Deserialize(headerBuffer);
		logger.Debug("Header received. FrameId: " + std::to_string(header.GetFrameId()) +
					", BodySize: " + std::to_string(header.GetBodySize()));
		logger.Debug("Gop Start : " + std::to_string(static_cast<int>(header.GetGopStartFlag())));
		logger.Debug("Gop Size : " + std::to_string(header.GetGopSize()));

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
			logger.Error("Failed to receive body");
			throw std::runtime_error("Failed to receive body");
		}

		// // Body 복호화
		// std::vector<uint8_t> outDecrypted;
		// decryptBody(bodyBuffer, header.GetTimestamp(), outDecrypted);

		// 4. Body 역직렬화 (Body가 0일 경우 처리하지 않음)
		logger.Info("Body deserialize");
		frame::Body body;
		// if (bodyResult > 0)
		// {
		// 	body.Deserialize(outDecrypted);
		// }
		body.Deserialize(bodyBuffer);

		// 5. Frame 객체 생성
		logger.Info("Frame init");
		frame::Frame frame(header, body);

		return frame;
	}

	void VideoClient::decryptBody(const std::vector<uint8_t>& data, const std::string& timestamp, std::vector<uint8_t>& OUT decrypted)
	{
		std::vector<uint8_t> nonce(12, 0x00);
		std::copy(timestamp.end() - 12, timestamp.end(), nonce.begin());

		mCipherHandler->Decrypt(data, decrypted, nonce);
	}
}
