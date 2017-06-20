#include "BubbleSort.h"
#include "Button.h"
#include "Game.h"
#include "InsertionSort.h"
#include "ListButton.h"
#include "QuickSort.h"
#include "RadixSort.h"
#include "Slider.h"
#include "IResourceManager.h"
#include "IGraphics.h"
#include "IInput.h"
#include "ICommon.h"
#include "StringUtils.h"

std::atomic<int> sleepTime(10);
static std::atomic<bool> threadFinished(false);

CGame game_local;

static IAlgorithm *CreateAlgorithmFromName(const std::string &name)
{
	if(name == "Bubble Sort")
		return new CBubbleSort();
	if(name == "Quick Sort")
		return new CQuickSort();
	if(name == "Insertion Sort")
		return new CInsertionSort();
	if(name == "Radix Sort")
		return new CRadixSort();
	return nullptr;
}

static void ThreadEntry(IAlgorithm *algorithm, CData *data)
{
	threadFinished = false;
	algorithm->Run(data);
	threadFinished = true;
}

CGame::CGame()
	: data(200, 1000)
	, thread(nullptr)
	, algorithm(nullptr)
	, btnStart(nullptr)
	, btnQuit(nullptr)
	, btnRandomize(nullptr)
	, btnListAlgorithms(nullptr)
	, sliderDataSize(nullptr)
{
}

CGame::~CGame()
{
}

void CGame::StartSimulation()
{
	StopSimulation();
	data.Reset();
	algorithm = CreateAlgorithmFromName(currentAlgorithmName);

	thread = new std::thread(ThreadEntry, algorithm, &data);
	btnStart->SetText("Stop");
}

void CGame::StopSimulation()
{
	if (algorithm) {
		algorithm->SetHalt();
	}

	if (thread) {
		thread->join();
		delete thread;
		thread = nullptr;
	}

	delete algorithm;
	algorithm = nullptr;

	threadFinished = false;

	btnStart->SetText("Start");
}

void CGame::Initialize()
{
	font = resMgr->GetFont("data/res/font/ui.fnt");

	/* Construct UI */
	CVector2f screenSize = graphics->GetSize();
	static const float offsetX = 10.0f;
	static const float offsetY = 50.0f;
	float x = offsetX;

	btnStart = new CButton(CVector2f(x, screenSize.y - offsetY), "Start");
	btnStart->Initialize();
	x += (float)btnStart->GetRect().width + offsetX;
	btnQuit = new CButton(CVector2f(x, screenSize.y - offsetY), "Quit");
	btnQuit->Initialize();
	x += (float)btnQuit->GetRect().width + offsetX;
	btnRandomize = new CButton(CVector2f(x, screenSize.y - offsetY), "Randomize");
	btnRandomize->Initialize();
	x += (float)btnRandomize->GetRect().width + offsetX;
	btnListAlgorithms = new CListButton(CVector2f(x, screenSize.y - offsetY), "Algorithm", {"Bubble Sort", "Quick Sort", "Insertion Sort", "Radix Sort"});
	btnListAlgorithms->Initialize();

	sliderDataSize = new CSlider(0, 1000, 200, CVector2f(offsetX, screenSize.y - 2.0f * offsetY), "Data Count");
	sliderDataSize->Initialize();
	sliderSleepTime = new CSlider(1, 100, 10, CVector2f(offsetX, screenSize.y - 3.0f * offsetY),  "Time Delay");
	sliderSleepTime->Initialize();
}

void CGame::Quit()
{
	StopSimulation();
}

void CGame::Update(float dtTime)
{
	btnStart->Update(dtTime);
	btnRandomize->Update(dtTime);
	btnQuit->Update(dtTime);
	btnListAlgorithms->Update(dtTime);
	sliderDataSize->Update(dtTime);
	sliderSleepTime->Update(dtTime);

	sleepTime = sliderSleepTime->GetValue();

	if(threadFinished) {
		StopSimulation();
	}

	if(btnQuit->Clicked())
		common->Quit();

	if (input->KeyPressed(NSKey::NSK_RETURN, true) || btnStart->Clicked())
		if (algorithm)
			StopSimulation();
		else
			StartSimulation();

	if (input->KeyPressed(NSKey::NSK_p, true) || btnRandomize->Clicked()) {
		if(algorithm)
			StopSimulation();
		data.Randomize();
	}

	if(btnListAlgorithms->GetSelectedItem() != currentAlgorithmName) {
		if(algorithm)
			StopSimulation();

		currentAlgorithmName = btnListAlgorithms->GetSelectedItem();
	}

	if(sliderDataSize->GetValue() != data.GetSize())
		data.ChangeSize(sliderDataSize->GetValue());

	data.Update(dtTime);
}

void CGame::Draw()
{
	int *rawData = data.GetRawData();
	int *timeData = data.GetTimeData();
	float sliceWidth = (float)graphics->GetWidth() / (float)data.GetSize();
	for (int i = 0; i < data.GetSize(); i++) {
		int t = 255 - (int)((float)std::abs(timeData[i]) / (float)interval * 255.0f);
		CColor color;
		if(timeData[i] < 0)
			color = data.IsDirty(i) ? CColor(0, 255, 0) : CColor(255, t, t);
		else
			color = data.IsDirty(i) ? CColor(0, 255, 0) : CColor(t, t, 255);
		graphics->DrawRect(CRect(CVector2f((float)i * sliceWidth, 0), (int)sliceWidth, ((float)rawData[i] / (float)data.GetMax()) * graphics->GetHeight()), color);
	}

	btnStart->Draw();
	btnRandomize->Draw();
	btnQuit->Draw();
	btnListAlgorithms->Draw();
	sliderDataSize->Draw();
	sliderSleepTime->Draw();

	std::string str = StrUtl::FormatString("Read: %d, Write: %d", data.GetAccessCount(), data.GetWriteCount());
	graphics->DrawText(font, CVector2f(460.0f, graphics->GetHeight() - 50.0f), CColor::white, str);

	str = StrUtl::FormatString("Algorithm: %s", currentAlgorithmName.c_str());
	graphics->DrawText(font, CVector2f(460.0f, graphics->GetHeight() - 80.0f), CColor::white, str);
}

/* Compile with -DSTANDALONE=ON */
#define GAMEAPI
extern "C" GAMEAPI void GameDLLExchange(EngineGlobals *eg, GameGlobals *gg) {
#ifndef STANDALONE
	::config = eg->config;
	::common = eg->common;
	::fileSystem = eg->filesystem;
	::game = &game_local;
	::graphics = eg->graphics;
	::input = eg->input;
	::resMgr = eg->resMgr;
	::soundSystem = eg->soundSys;
#endif
	gg->game = &game_local;
}
