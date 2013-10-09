/* This file is part of the Zenipex Library (zenilib).
 * Copyleft (C) 2011 Mitchell Keith Bloch (bazald).
 *
 * This source file is simply under the public domain.
 */

#include <zenilib.h>
#include <string>
#include "Buggy.h"
#include "Map.h"
#include "Score.h"

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

using namespace std;
using namespace Zeni;

//Global score variable
Score m_score;
std::string test;

class Play_State : public Gamestate_Base
{
	Play_State(const Play_State &);
	Play_State operator=(const Play_State &);

public:
	Play_State()
	  : game_resolution(1280.0f, 768.0f),
	  top_left(-256.0f, 0.0f),
	  map1("maps/map1.txt"),
	  map_scroll_speed(0.0f),
	  m_time_passed(0.0f),
	  m_max_time_step(1.0f / 20.0f), // make the largest physics step 1/20 of a second
	  m_max_time_steps(10.0f), // allow no more than 10 physics steps per frame
	  //      position,               size,                     theta,  speed,   min_speed, max_speed, acceleration
	  m_buggy(Point2f(50.0f, 250.0f), Vector2f(256.0f, 128.0f), 0,      100.0f,  20.0f,     300.0f,    90.0f,      &m_score)
  {
	  // If our game has no real time component in real life, allow the user to pause the game.
	  // This would be a BAD idea in a networked multiplayer mode for a game.
	  // (Or at the very least, this would be the wrong way to do it.)
	  set_pausable(true);

	  m_score.reset();
  }

private:
  Vector2f game_resolution;
  Point2f top_left;
  Map map1;
  float map_scroll_speed;

  Buggy m_buggy;


  void on_push() {
    //get_Window().mouse_grab(true);
    get_Window().mouse_hide(true);
    //get_Game().joy_mouse.enabled = false;

	m_chrono.start();
  }

  void on_pop() {
	m_chrono.stop();

    //get_Window().mouse_grab(false);
    get_Window().mouse_hide(false);
    //get_Game().joy_mouse.enabled = true;
  }

  Zeni::Time m_current_time;
  Chronometer<Time> m_chrono;
  float m_time_passed;
  float m_max_time_step;  //< Optional
  float m_max_time_steps; //< Optional

  void Play_State::on_key(const SDL_KeyboardEvent &event) {
	  m_buggy.on_key(event);

	  Gamestate_Base::on_key(event);
  }

  void adjust_map_scroll() {
	  //TODO: speed up map scroll when buggy gets to certain point
	  float distance = (m_buggy.get_position().x + m_buggy.get_size().x) - (top_left.x + game_resolution.x / 4);

	  map_scroll_speed = distance;

	  if (distance < 0)
		  map_scroll_speed *= 6;
  }

  void perform_logic() {
	  // Update time_passed
	  const float time_passed = m_chrono.seconds();
	  float time_step = time_passed - m_time_passed;
	  m_time_passed = time_passed;

	  /* Shrink time passed to an upper bound
	  *
	  * If your program is ever paused by your computer for 10
	  * minutes and then allowed to continue running, you don't want
	  * it to pause at the physics loop for another 10 minutes.
	  */
	  if (time_step / m_max_time_step > m_max_time_steps)
		  time_step = m_max_time_steps * m_max_time_step;

	  while (time_step > m_max_time_step) {
		  m_buggy.step(time_step, &map1);
		  adjust_map_scroll();
		  map1.step_all(time_step, game_resolution, top_left);
		  top_left.x += map_scroll_speed * time_step;

		  time_step -= m_max_time_step;
	  }

	  m_buggy.step(time_step, &map1);
	  adjust_map_scroll();
	  map1.step_all(time_step, game_resolution, top_left);
	  top_left.x += map_scroll_speed * time_step;
	  time_step = 0.0f;
  }

  void Play_State::render_bg()
  {
	  map1.render_all(game_resolution, top_left, &m_buggy);
  }

  void Play_State::render() {
	  Video &vr = get_Video();

	  vr.set_2d(make_pair(top_left, game_resolution + top_left));

	  render_bg();

	  m_buggy.render();
	  m_buggy.render_collisions(&map1);

	  m_score.render(top_left);

	  Zeni::Font &fr = get_Fonts()["title"];

	  fr.render_text(
		  test.c_str(),
		  Point2f(top_left.x + 100.0f + 0.5f * fr.get_text_width(test.c_str()), top_left.y + 400.0f - 0.5f * fr.get_text_height()),
		  get_Colors()["title_text"],
		  ZENI_CENTER);
  }

};

class Instructions_State : public Widget_Gamestate {
  Instructions_State(const Instructions_State &);
  Instructions_State operator=(const Instructions_State &);

public:
  Instructions_State()
    : Widget_Gamestate(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)))
  {
  }

private:
  void on_key(const SDL_KeyboardEvent &event) {
    if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED)
      get_Game().pop_state();
  }

  void render() {
    Widget_Gamestate::render();

    Zeni::Font &fr = get_Fonts()["system_36_x600"];

    fr.render_text(
		"Use the left and right arrows or A and D\n to adjust the Buggy's speed\n\n"
		"Press the up arrow or W to jump\n\n"
		"Avoid crashing the buggy or being hit by asteroids!\n\n"
		"Press "
#if defined(_WINDOWS)
        "ALT+F4"
#elif defined(_MACOSX)
        "Apple+Q"
#else
        "Ctrl+Q"
#endif
        " to Quit\n\n\n"
						   
		"Press escape to go back to the main menu",
                   Point2f(400.0f, 100.0f - 0.5f * fr.get_text_height()),
                   get_Colors()["title_text"],
                   ZENI_CENTER);
  }
};

class Bootstrap {
  class Gamestate_One_Initializer : public Gamestate_Zero_Initializer {
    virtual Gamestate_Base * operator()() {
      Window::set_title("zenilib Mars Buggy");

      get_Joysticks();
      get_Video();
      get_Textures();
      get_Fonts();
      get_Sounds();
      get_Game().joy_mouse.enabled = true;


	  get_Sound().set_BGM("sfx/cycle2_08");
	  get_Sound().set_BGM_looping(true);
	  //get_Sound().play_BGM();

      return new Title_State<Play_State, Instructions_State>("Zenipex Library\nMars Patrol");
    }
  } m_goi;

public:
  Bootstrap() {
    g_gzi = &m_goi;
  }
} g_bootstrap;

int main(int argc, char **argv) {
  return zenilib_main(argc, argv);
}
