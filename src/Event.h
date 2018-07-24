#pragma once

#include <vector>
#include <list>
#include <memory>

#include "Types.h"

namespace PangPang
{
namespace Event
{
	enum KeyMask {
		EventShiftKeyMask = 1,
		EventControlKeyMask = 1 << 1,
		EventAltKeyMask = 1 << 2,
		EventOptionKeyMask = EventAltKeyMask,
		EventFunctionKeyMask = 1 << 3
	};

	struct Keyboard
	{
		typedef std::vector<Keyboard> List;

		enum Type {
			Down,
			Up
		};
		Type type;

		unsigned int keyCode;
		unsigned int flags;
	};

	struct Mouse
	{
		typedef std::vector<Mouse> List;

		enum Type {
			None,
			Move,
			LeftDown,
			LeftDragged,
			LeftUp,
			RightDown,
			RightDragged,
			RightUp
		};
		Type type;

		Point locationInWindow;
		int clickCount;
	};

	template <typename T>
	class Emitter;

	template <typename T>
	class Listener
	{
		friend class Emitter<T>;
	protected:
		Listener(Emitter<T>* emitter)
			:	emitter(emitter)
		{
			emitter->Register(this);
		}
		~Listener()
		{
			emitter->Unregister(this);
		}
	protected:
		virtual void Handler(const typename T::List& list) = 0;
	private:
		Emitter<T>* const emitter;
	};

	template <typename T>
	class Emitter
	{
		friend class Listener<T>;
	private:
		void Register(Listener<T>* listener)
		{
			listeners.push_back(listener);
		}
		void Unregister(Listener<T>* listener)
		{
			listeners.remove(listener);
		}
	protected:
		void Emit()
		{
			for(auto it = listeners.begin(); it != listeners.end(); ++it)
				(*it)->Handler(list);
			list.clear();
		}
	protected:
		typename T::List list;
		std::list<Listener<T>* > listeners;
	};
}
}

