/* This file is part of the Zenipex Library (zenilib).
 * Copyleft (C) 2011 Mitchell Keith Bloch (bazald).
 *
 * This source file is simply under the public domain.
 */

#include <zenilib.h>
#include "Buggy.h"

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

using namespace std;
using namespace Zeni;

class Play_State : public Gamestate_Base
{
	Play_State(const Play_State &);
	Play_State operator=(const Play_State &);

public:
  Play_State()
	  : m_time_passed(0.0f),
	  m_max_time_step(1.0f / 20.0f), // make the largest physics step 1/20 of a second
	  m_max_time_steps(10.0f), // allow no more than 10 physics steps per frame
	  m_buggy(Point2f(100.0f, 100.0f), Vector2f(256.0f, 128.0f), 0, 150.0f)
  {
	  // If our game has no real time component in real life, allow the user to pause the game.
	  // This would be a BAD idea in a networked multiplayer mode for a game.
	  // (Or at the very least, this would be the wrong way to do it.)
	  set_pausable(true);
  }

private:
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

  struct Controls {
	  Controls() : left(false), right(false), up(false), down(false) {}

	  bool left;
	  bool right;
	  bool up;
	  bool down;
  } m_controls;

  Zeni::Time m_current_time;
  Chronometer<Time> m_chrono;
  float m_time_passed;
  float m_max_time_step;  //< Optional
  float m_max_time_steps; //< Optional

  void Play_State::on_key(const SDL_KeyboardEvent &event) {
	  switch (event.keysym.sym) {
	  case SDLK_LEFT:
		  m_controls.left = event.type == SDL_KEYDOWN;
		  break;

	  case SDLK_RIGHT:
		  m_controls.right = event.type == SDL_KEYDOWN;
		  break;

	  case SDLK_UP:
		  m_controls.up = event.type == SDL_KEYDOWN;
		  break;

	  case SDLK_DOWN:
		  m_controls.down = event.type == SDL_KEYDOWN;
		  break;

	  default:
		  Gamestate_Base::on_key(event);
	  }
  }

  void step(const float &time_step) {
	  // without a multiplier, this will rotate a full turn after ~6.28s
	  m_buggy.turn_left((m_controls.up - m_controls.down) * time_step);
	  // without the '100.0f', it would move at ~1px/s
	  m_buggy.move_forward((m_controls.right - m_controls.left) * time_step * m_buggy.get_speed());
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
		  step(time_step);
		  time_step -= m_max_time_step;
	  }

	  step(time_step);
	  time_step = 0.0f;
  }

  void Play_State::render_bg(Video &vr)
  {
	  Color c = get_Colors()["green"];

	  Vertex2f_Color p0(Point2f(0.0f, 0.0f), c); //Point2f(0.0f, 0.0f));
	  Vertex2f_Color p1(Point2f(0.0f, get_Window().get_height()), c); //Point2f(0.0f, 1.0f));
	  Vertex2f_Color p2(Point2f(get_Window().get_width(), get_Window().get_height()), c); //Point2f(1.0f, 1.0f));
	  Vertex2f_Color p3(Point2f(get_Window().get_width(), 0.0f), c); //Point2f(1.0f, 0.0f));
	  //Material material("tire");

	  Quadrilateral<Vertex2f_Color> quad(p0, p1, p2, p3);
	  //quad.fax_Material(&material);

	  vr.render(quad);
  }

  void Play_State::render() {
	  Video &vr = get_Video();

	  vr.set_2d(make_pair(Point2f(), Point2f(1024.0f, 768.0f)));

	  render_bg(vr);

	  m_buggy.render();
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

    Zeni::Font &fr = get_Fonts()["title"];

    fr.render_text(
#if defined(_WINDOWS)
                   "ALT+F4"
#elif defined(_MACOSX)
                   "Apple+Q"
#else
                   "Ctrl+Q"
#endif
                           " to Quit",
                   Point2f(400.0f, 300.0f - 0.5f * fr.get_text_height()),
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

      return new Title_State<Play_State, Instructions_State>("Zenipex Library\nMars Buggy");
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
