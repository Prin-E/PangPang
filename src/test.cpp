#include "Common.h"

#if defined(WIN32)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "GameEngine.h"
#include "State.h"
#include "Event.h"
#include "Map.h"
#include "Tank.h"
#include "Line.h"
#include "Player.h"
#include "LocalPlayer.h"
#include "Stage.h"
#include "Resource.h"
#include "AIPlayer.h"

using namespace PangPang;

class TestMenu : public State
{
public:
	TestMenu(StateManager* ptr)
		: State(ptr), totalTime(0.0f), renderer(GetStateData().renderer)
		, uiManager(renderer), bar1(new UI::Bar()), bar2(new UI::Bar()), bar3(new UI::Bar()), angleView(new UI::AngleView()), imageView(new UI::ImageView())
		, timerLabel(new UI::Label())
	{
		// Bar
		bar1->SetValue(0.0f);
		bar1->SetPosition(Point(260, 70));
		bar1->SetSize(Size(420, 20));
		bar1->SetColor(1.0f, 0.40f, 0.30f);

		bar2->SetValue(0.0f);
		bar2->SetPosition(Point(260, 40));
		bar2->SetSize(Size(420, 20));
		bar2->SetColor(0.20f, 1.0f, 0.40f);

		bar3->SetValue(60.0f);
		bar3->SetPosition(Point(260, 10));
		bar3->SetSize(Size(420, 20));
		bar3->SetColor(0.8f, 0.8f, 0.20f);

		// Angle View
		angleView->SetPosition(Point(10, 10));
		angleView->SetSize(Size(130, 130));

		// Image View
		imageView->SetImageAtPath("dir/StatusBar.tga");
		imageView->SetSize(Size(800, 150));

		// Label
		timerLabel->SetPosition(Point(700, 24));
		timerLabel->SetSize(Size(100, 100));
		timerLabel->SetTextSize(80);
		timerLabel->SetTextColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
		timerLabel->SetText(L"00");
		timerLabel->SetTextAlignment(UI::TextAlignmentMiddle);
		
		// UI Manager
		uiManager.AddUI(bar1, 1);
		uiManager.AddUI(bar2, 1);
		uiManager.AddUI(bar3, 1);
		uiManager.AddUI(imageView, 0);
		uiManager.AddUI(angleView, 3);
		uiManager.AddUI(timerLabel, 4);

		std::shared_ptr<Map> map(new Map(renderer, Renderer::BackgroundLayer, "dir/back_grass.tga", "dir/land_grass.tga"));


		std::vector<std::shared_ptr<Player> > players;
		
		players.push_back(std::make_shared<LocalPlayer>(std::make_shared<Tank>(renderer, "cannon/cannon.json", map, Point(300, 550)), GetStateData().window, bar1, bar2, bar3, angleView));
		//players.push_back(std::make_shared<LocalPlayer>(std::make_shared<Tank>(renderer, "cannon/cannon.json", map, Point(200, 550)), GetStateData().window, bar1, bar2, bar3, angleView));

		players.push_back(std::make_shared<AIPlayer>(std::make_shared<Tank>(renderer, "cannon/cannon.json", map, Point(1000, 550))));

		stage.reset(new Stage(GetStateData().window, renderer, players.begin(), players.end(), map));
	}
private:
	virtual void OnEnter() {}
	virtual void OnExit() {}
	virtual void Update(float deltaTime)
	{
		if(!stage->Update(deltaTime))
		{
			std::cout << "game is end" << std::endl;
			GetStateData().gameApp->Stop();
			return;
		}
		renderer->Update(deltaTime);
		renderer->Draw();
	}

	void Action(int tag) 
	{
		printf("어머, 버튼이 눌렸네?\n");
		printf("Tag는 UI를 구별하기 위한 식별자로 사용된다.\nTag: %d\n\n", tag);
	}


private:
	float totalTime;
	Renderer* renderer;
	std::shared_ptr<Stage> stage;

	UI::UIManager uiManager;
	std::shared_ptr<UI::Bar> bar1, bar2, bar3;
	std::shared_ptr<UI::AngleView> angleView;
	std::shared_ptr<UI::ImageView> imageView;
	std::shared_ptr<UI::Label> timerLabel;
};

class PangpangApp : public GameApplication
{
public:
	PangpangApp(int argc, char* argv[])
		:	GameApplication(argc, argv, L"Pangpang", Size(800,600)), renderer(this), stateManager(StateData(this, &renderer, this))
	{
		stateManager.SetState<TestMenu>();
	}
	~PangpangApp()
	{
	}
private:
	virtual void UpdateGame(double deltaTime)
	{
		stateManager.Update(deltaTime);
	}
private:
	Renderer renderer;
	StateManager stateManager;
};

// Test
int main(int argc, char *argv[]) {
#if defined(WIN32)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	try
	{
		Resource::Open("/file/not/exist");
	}
	catch(Resource::FileNotFound)
	{
		std::cout << "FileNotFound exception works well." << std::endl;
	}

	PangpangApp pangpangApp(argc, argv);

	pangpangApp.Run();
}

