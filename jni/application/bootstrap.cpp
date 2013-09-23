/* This file is part of the Zenipex Library (zenilib).
 * Copyleft (C) 2011 Mitchell Keith Bloch (bazald).
 *
 * This source file is simply under the public domain.
 */

#include <zenilib.h>
#include <string>
#include "Buggy.h"
#include "map.h"

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
	  : game_resolution(1024.0f, 768.0f),
	  tile_size(64),
	  map1("assets/map1.txt"),
	  m_time_passed(0.0f),
	  m_max_time_step(1.0f / 20.0f), // make the largest physics step 1/20 of a second
	  m_max_time_steps(10.0f), // allow no more than 10 physics steps per frame
	  m_buggy(Point2f(50.0f, 200.0f), Vector2f(256.0f, 128.0f), Global::pi / 4, 150.0f)
  {
	  // If our game has no real time component in real life, allow the user to pause the game.
	  // This would be a BAD idea in a networked multiplayer mode for a game.
	  // (Or at the very least, this would be the wrong way to do it.)
	  set_pausable(true);
  }

private:
  Vector2f game_resolution;
  float tile_size;
  Map map1;

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
		  m_buggy.step(time_step);

		  time_step -= m_max_time_step;
	  }

	  m_buggy.step(time_step);
	  time_step = 0.0f;
  }

  void Play_State::render_tile(int x, int y)
  {
	  Video &vr = get_Video();

	  Vertex2f_Texture p0(Point2f(x, y), Point2f(0.0f, 0.0f));
	  Vertex2f_Texture p1(Point2f(x, y + tile_size), Point2f(0.0f, 1.0f));
	  Vertex2f_Texture p2(Point2f(x + tile_size, y + tile_size), Point2f(1.0f, 1.0f));
	  Vertex2f_Texture p3(Point2f(x + tile_size, y), Point2f(1.0f, 0.0f));
	  Material material(map1.get_texture(x, y).c_str());

	  Quadrilateral<Vertex2f_Texture> quad(p0, p1, p2, p3);
	  quad.fax_Material(&material);

	  vr.render(quad);
  }

  void Play_State::render_bg(Video &vr)
  {
	  for (int x = 0; x < game_resolution.x / tile_size; x++) {
		  for (int y = 0; y < game_resolution.y / tile_size; y++) {
			  //render_tile(x, y);
		  }
	  }

	  /*
	  Color c = get_Colors()["red"];

	  Vertex2f_Color p0(Point2f(0.0f, 0.0f), c); //Point2f(0.0f, 0.0f));
	  Vertex2f_Color p1(Point2f(0.0f, game_resolution.j), c); //Point2f(0.0f, 1.0f));
	  Vertex2f_Color p2(Point2f(game_resolution.i, game_resolution.j), c); //Point2f(1.0f, 1.0f));
	  Vertex2f_Color p3(Point2f(game_resolution.i, 0.0f), c); //Point2f(1.0f, 0.0f));
	  //Material material("tire");

	  Quadrilateral<Vertex2f_Color> quad(p0, p1, p2, p3);
	  //quad.fax_Material(&material);

	  vr.render(quad);

	  c = get_Colors()["brown"];

	  Vertex2f_Color p00(Point2f(0.0f, 500.0f), c); //Point2f(0.0f, 0.0f));
	  Vertex2f_Color p11(Point2f(0.0f, game_resolution.j), c); //Point2f(0.0f, 1.0f));
	  Vertex2f_Color p22(Point2f(game_resolution.i, game_resolution.j), c); //Point2f(1.0f, 1.0f));
	  Vertex2f_Color p33(Point2f(game_resolution.i, 500.0f), c); //Point2f(1.0f, 0.0f));
	  //Material material("tire");

	  Quadrilateral<Vertex2f_Color> quad2(p00, p11, p22, p33);
	  //quad.fax_Material(&material);

	  vr.render(quad2);*/
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
