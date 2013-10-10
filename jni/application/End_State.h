#pragma once

#include <zenilib.h>

#include "Score.h"

using namespace Zeni;

class End_State : public Widget_Gamestate {
	End_State(const End_State &);
	End_State operator=(const End_State &);

	float score;

public:
	End_State(const float &score_)
		: Widget_Gamestate(std::make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f))),
		score(score_)
	{
	}

	void on_push() {
		get_Sound().stop_BGM();
		play_sound("end");
	}

private:
	void on_key(const SDL_KeyboardEvent &event) {
		if (event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED) {
			get_Game().pop_state();
			get_Game().pop_state();
		}
	}

	void render() {
		Widget_Gamestate::render();

		Zeni::Font &fr = get_Fonts()["title"];

		fr.render_text(
			"You win!!!\n\n"
			"Final Score: " + ftoa(score) + "\n\n"
			"Press escape to return to the main menu",
			Point2f(400.0f, 100.0f - 0.5f * fr.get_text_height()),
			get_Colors()["title_text"],
			ZENI_CENTER);
	}
};