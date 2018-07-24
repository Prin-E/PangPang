#pragma once

#include <map>
#include <memory>

#include "Window.h"
//#include "UIManager.h"


namespace PangPang
{
class GameApplication;
class Renderer;
class StateManager;

struct StateData
{
	StateData(GameApplication* gameApp, Renderer* renderer, Window* window)
		:	gameApp(gameApp), renderer(renderer), window(window) {}
	StateData(const StateData& rhs)
		:	gameApp(rhs.gameApp), renderer(rhs.renderer), window(rhs.window) {}
	//StateData(StateData&& rhs)
	//	:	renderer(rhs.renderer), window(rhs.window) {rhs.renderer = 0; rhs.window = 0;}

	GameApplication* gameApp;
	Renderer* renderer;
	Window* window;
private:
	//StateData& operator=(StateData&& data);
	StateData& operator=(const StateData& data);
};

class State
{
	friend class StateManager;
protected:
	State(StateManager* ptr_) : ptr(ptr_) {}
public:
	virtual ~State() {}
public:
	//virtual void OnEnter() = 0;
	virtual void OnExit() = 0;
	virtual void Update(float deltaTime) = 0;
protected:
	template<class S>
	void SetState();
	template<class S, typename A1>
	void SetState(A1 a1);
	template<class S, typename A1, typename A2>
	void SetState(A1 a1, A2 a2);
	template<class S, typename A1, typename A2, typename A3>
	void SetState(A1 a1, A2 a2, A3 a3);
	const StateData& GetStateData();
private:
	StateManager* ptr;
};

class StateManager
{
	friend class State;
public:
	typedef std::map<const std::type_info*, std::shared_ptr<State> > StateDic;
public:
	StateManager(const StateData& data) : stateDictionary(new StateDic), stateData(data) {}
	//StateManager(StateData&& data) : stateDictionary(new StateDic), stateData(data) {}
	StateManager(const StateManager& rhs)
		: stateDictionary(rhs.stateDictionary),
		stateData(rhs.stateData) {}
public:
	const StateManager& operator=(const StateManager& rhs)
	{
		stateDictionary = rhs.stateDictionary;
		return *this;
	}
public:
	template<class S>
	void SetState()
	{
		SwitchState<S>();
		static_cast<S*>(currentState.get())->OnEnter();
	}
	template<class S, typename A1>
	void SetState(A1 a1)
	{
		SwitchState<S>();
		static_cast<S*>(currentState.get())->OnEnter(a1);
	}
	template<class S, typename A1, typename A2>
	void SetState(A1 a1, A2 a2)
	{
		SwitchState<S>();
		static_cast<S*>(currentState.get())->OnEnter(a1, a2);
	}
	template<class S, typename A1, typename A2, typename A3>
	void SetState(A1 a1, A2 a2, A3 a3)
	{
		SwitchState<S>();
		static_cast<S*>(currentState.get())->OnEnter(a1, a2, a3);
	}
	void Update(float dt)
	{
		currentState->Update(dt);
	}
private:
	template<class S>
	void SwitchState()
{
		std::shared_ptr<State> prevState = currentState;

		auto it = stateDictionary->find(&typeid(S));
		if(it == stateDictionary->end())
		{
			State* temp = new S(this);
			std::shared_ptr<State> state(temp);

			stateDictionary->insert(std::make_pair(&typeid(S), state));
			currentState = state;
		}
		else
		{
			currentState = it->second;
		}

		if(prevState) prevState->OnExit();
	}
private:
	std::shared_ptr<State> currentState;
	std::shared_ptr<StateDic> stateDictionary;
	const StateData stateData;
};

template <class S>
void State::SetState()
{
	ptr->SetState<S>();
}
template<class S, typename A1>
void State::SetState(A1 a1)
{
	ptr->SetState<S>(a1);
}
template<class S, typename A1, typename A2>
void State::SetState(A1 a1, A2 a2)
{
	ptr->SetState<S>(a2);
}
template<class S, typename A1, typename A2, typename A3>
void State::SetState(A1 a1, A2 a2, A3 a3)
{
	ptr->SetState<S>(a3);
}
}

