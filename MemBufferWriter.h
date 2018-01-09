//MIT License
//
//Copyright(c) 2018 LIGA(廖礼佳)
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
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