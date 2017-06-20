#include "Data.h"
#include <algorithm>
#include <string.h>

CData::CData(int size, int max)
	: accessCount(0)
	, writeCount(0)
	, max(max)
	, size(size)
	, dirty(-1)
{
	data = new int[size];
	timeData = new int[size]();
	Randomize();
}

CData::~CData()
{
	delete [] data;
	delete [] timeData;
}

void CData::Update(float dtTime)
{
	for (int i = 0; i < size; i++) {
		if (timeData[i] > 0) {
			timeData[i] -= 16;
			if (timeData[i] < 0)
				timeData[i] = 0;
		} else if (timeData[i] < 0) {
			timeData[i] += 16;
			if (timeData[i] > 0)
				timeData[i] = 0;
		}
	}
}

void CData::Set(int pos, int value)
{
	std::lock_guard<std::mutex> lock(mutex);
	data[pos] = value;
	dirty = pos;
	writeCount++;
	timeData[pos] = -interval;
}

int CData::Get(int pos)
{
	std::lock_guard<std::mutex> lock(mutex);
	dirty = pos;
	accessCount++;
	timeData[pos] = interval;
	return data[pos];
}

void CData::Swap(int i, int j)
{
	std::lock_guard<std::mutex> lock(mutex);
	static int tmp;
	tmp = data[i];
	data[i] = data[j];
	data[j] = tmp;
	dirty = j;
	accessCount += 2;
	writeCount += 2;
	timeData[i] = -interval;
	timeData[j] = -interval;
}

void CData::Randomize()
{
	if(size > 0) {
		for (int i = 0; i < size; i++)
			data[i] = (int)((float)(i) / (float)size * (float)max);
		std::random_shuffle(&data[0], &data[size - 1]);
	}
}

void CData::Reset()
{
	accessCount = 0;
	writeCount = 0;
}


void CData::ChangeSize(int newSize)
{
	std::lock_guard<std::mutex> lock(mutex);
	int *newData = new int[newSize];
	memcpy(newData, data, std::min(size, newSize) * sizeof(int));
	int *newTimeData = new int[newSize]();
	memcpy(newTimeData, timeData, std::min(size, newSize) * sizeof(int));

	for(int i = 0; i < std::min(size, newSize); i++)
		newData[i] = (int)((float)newData[i] * (float)size/(float)newSize);

	if(newSize > size) {
		for(int i = size; i < newSize; i++) {
			newData[i] = (int)((float)(i-size) / (float)(newSize-size) * (float)max);
		}
		std::random_shuffle(&newData[size], &newData[newSize - 1]);
	}
	delete [] data;
	delete [] timeData;
	data = newData;
	timeData = newTimeData;
	size = newSize;
}
