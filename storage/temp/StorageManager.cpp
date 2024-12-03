#include "StorageManager.h"

StorageManager::StorageManager(std::string ip)
	: mId(ip)
{
	// id 에 해당하는 자료구조가 있는가. 
		// 1. 없다면, 그 자료구조 자체를 생성해야하고
			fwrite() // 메타데이터영역에

		// 2. 있다면, 그 자료구조에서 Metadata 를 읽어온다(mStartPoint, mEndPoint)
			fread() // 메타데이터영역에서 
}


StorageManager::SaveFrame(Frame frame)
{
	// append 모드로
	fwrite(mId, frame) // 데이터를 넣었음

	// 그에 맞게, 메타데이터를 갱신함 (mEndPoint 가 갱신됨, 필요에 맞게, mStartPoint 갱신)
}

StorageManager::LoadNextFrame(OUT Frame frame)
{
	fread(mId, ) // mCurrentPoint 에서 그 frame 만큼 읽음

	// mCurrentPoint 는 읽은 frame 만큼 갱신됌
}