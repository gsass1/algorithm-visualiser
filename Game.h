#pragma once

#include "IGame.h"
#include "Data.h"
#include <atomic>

class CButton;
class CListButton;
class CSlider;
class IFont;
struct IAlgorithm;

class CGame : public IGame
{
public:
			CGame();
			~CGame();

	void		Initialize();
	void		Quit();

	void		Update(float dtTime);
	void		Draw();

	const char *	GetGameName() { return "Algorithm Visualiser"; }

	void		OnPrint(const char *msg) {}

	void		StartSimulation();
	void		StopSimulation();

private:
	CButton *	btnStart;
	CButton *	btnQuit;
	CButton *	btnRandomize;
	CListButton *	btnListAlgorithms;
	CSlider *	sliderDataSize;
	CSlider *	sliderSleepTime;
	IFont		*font;
	CData		data;
	std::thread *	thread;
	IAlgorithm	*algorithm;
	std::string	currentAlgorithmName;
};
