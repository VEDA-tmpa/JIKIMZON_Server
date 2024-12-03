

#define OUT

class StorageManager
{
public:
	StorageManager(std::string ip);

	SaveFrame(Frame frame);
	LoadNextFrame(OUT Frame frame); 

private:
	std::string mId;

	uint16_t mStartPoint;
	uint16_t mEndPoint;
	uint16_t mCurrentPoint;
};
