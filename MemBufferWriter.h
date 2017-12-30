#include <fstream>
#include <thread>
#include <minwindef.h>
class MemBufferWriter
{
public:
	MemBufferWriter(std::wstring filename, __int64 cachesize)
	{
		file.open(filename, std::fstream::binary | std::fstream::out);
		cacheSize = cachesize;
		cacheData[0] = new BYTE[cacheSize];
		cacheData[1] = new BYTE[cacheSize];
	};
	void Write(BYTE* data, __int64 _Count)
	{
		if (cacheSize < (ture_cacheSize + _Count))
		{
			if (thread.joinable())
			{
				thread.join();
			}
			thread = (std::thread(&std::fstream::write, &file, (const char*) cacheData[index], ture_cacheSize));
			ture_cacheSize = 0;
			if (index==0)
			{
				index = 1;
			}
			else
			{
				index = 0;
			}
		}

		memcpy(cacheData[index] + ture_cacheSize, data, _Count);
		ture_cacheSize += _Count;

	};
	void Close()
	{
		if (ture_cacheSize>0)
		{
			if (thread.joinable())
			{
				thread.join();
			}
			thread = (std::thread(&std::fstream::write, &file, (const char*) cacheData[index], ture_cacheSize));
		}
		if (thread.joinable())
		{
			thread.join();
		}
		if (file.is_open())
		{
			file.flush();
			file.close();
		}
		delete cacheData[0];
		delete cacheData[1];
	};
private:
	std::fstream file;
	std::thread  thread;
	BYTE* cacheData[2];
	__int64 cacheSize = 0;
	__int64 ture_cacheSize = 0;
	int index=0;
};