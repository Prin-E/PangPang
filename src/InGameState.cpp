#include "Common.h"

#include "InGameState.h"
#include "Stage.h"
#include "LocalPlayer.h"
#include "AIPlayer.h"
#include "MainMenu.h"

namespace PangPang
{
InGameState::InGameState(StateManager* ptr)
	: State(ptr), totalTime(0.0f), renderer(GetStateData().renderer), 
	bar1(new UI::Bar()), bar2(new UI::Bar()), bar3(new UI::Bar()), angleView(new UI::AngleView()), uiManager(renderer), imageView(new UI::ImageView())
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
}

void InGameState::OnEnter(GameType type)
{
	std::shared_ptr<Map> map(new Map(renderer, Renderer::BackgroundLayer, "dir/back_grass.tga", "dir/land_grass.tga"));

	std::vector<std::shared_ptr<Player> > players;

	players.push_back(std::make_shared<LocalPlayer>(std::make_shared<Tank>(renderer, "cannon/cannon.json", map, Point(300, 550)), GetStateData().window, bar1, bar2, bar3, angleView));
	
	if(type == PvP)
		players.push_back(std::make_shared<LocalPlayer>(std::make_shared<Tank>(renderer, "cannon/cannon.json", map, Point(1000, 550)), GetStateData().window, bar1, bar2, bar3, angleView));
	else if(type == PvAI)
		players.push_back(std::make_shared<AIPlayer>(std::make_shared<Tank>(renderer, "cannon/cannon.json", map, Point(1000, 550))));

	stage.reset(new Stage(GetStateData().window, renderer, players.begin(), players.end(), map, timerLabel));
}

void InGameState::OnExit()
{
	stage.reset();
}

void InGameState::Update(float deltaTime)
{
	if(!stage->Update(deltaTime))
	{
		std::cout << "game is end" << std::endl;
		SetState<MainMenu>();
		return;
	}
	renderer->Update(deltaTime);
	renderer->Draw();
}
}

