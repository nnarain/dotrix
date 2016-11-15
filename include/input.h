
/**
	\author Natesh Narain <nnaraindev@gmail.com>
	\date Nov 14 2016
*/

#ifndef DOTRIX_INPUT_H
#define DOTRIX_INPUT_H

#include <QKeyEvent>
#include <gameboycore/joypad.h>
#include <map>

/**
	\class Input
	\brief Input processor for Dotrix
*/
class Input
{
public:

	enum class State
	{
		PRESSED, RELEASED
	};

	Input() : joy_(nullptr)
	{
		// TODO: read key map from file
		keymap_.insert({ Qt::Key_W, gb::Joy::Key::UP });
		keymap_.insert({ Qt::Key_A, gb::Joy::Key::LEFT });
		keymap_.insert({ Qt::Key_D, gb::Joy::Key::RIGHT });
		keymap_.insert({ Qt::Key_S, gb::Joy::Key::DOWN });
		keymap_.insert({ Qt::Key_J, gb::Joy::Key::A });
		keymap_.insert({ Qt::Key_K, gb::Joy::Key::B });
		keymap_.insert({ Qt::Key_Return, gb::Joy::Key::START });
		keymap_.insert({ Qt::Key_Shift, gb::Joy::Key::SELECT });
	}

	void process(State state, int keycode)
	{
		if (!joy_) return;

		auto gbkey = keymap_[keycode];

		if (state == State::PRESSED)
			joy_->press(gbkey);
		else
			joy_->release(gbkey);
	}

	void setJoypad(gb::Joy::Ptr& joy)
	{
		joy_ = joy;
	}

	~Input()
	{
	}

private:
	gb::Joy::Ptr joy_;
	std::map<int, gb::Joy::Key> keymap_;
};

#endif //
