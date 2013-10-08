#pragma once

#include <string>

#include <zenilib.h>

using namespace Zeni;

class Score {
private:
	int score;
	int ckpt;

public:
	Score::Score() 
		: score(0), ckpt(0)
	{
	}

	void Score::render(const Point2f &top_left) const {
		Zeni::Font &fr = get_Fonts()["title"];

		fr.render_text(
			itoa(score),
			Point2f(top_left.x + 50.0f, top_left.y + 60.0f - 0.5f * fr.get_text_height()),
			get_Colors()["title_text"],
			ZENI_CENTER);
	}

	void Score::inc(int amt) {
		score += amt;
	}

	void checkpoint() {
		ckpt = score;
	}

	void reset() {
		score = ckpt;
	}

};