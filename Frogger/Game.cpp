#include "Game.h"

namespace fg
{
    Game::Game()
    {
        window = new RenderWindow(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);
        FrameTime frameSlice = 1.0f, frameStep = 1.0f, lastFrameTime = 0.0f, currentSlice = 0.0f;

        bool dead = false, win = false;
		Clock deadClock;

        frog = new Frog(Vector2f(window->getSize()));
        // TODO: MULTIPLE TRUCKS
        truck = new Truck(0, float(window->getSize().y / 2));
    }

    // input phase
    void Game::input()
    {            
        Event event;

        while (window->pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed: window->close();
            case Event::KeyPressed:
				// TODO: PAUSE
                if (Keyboard::Key::Escape == event.key.code) window->close();                
                else frog->update(event);                
            break;
            }
        }
    }

    // update phase
    void Game::update()
    {                
        currentSlice += lastFrameTime;
        for (; currentSlice >= frameSlice; currentSlice--)
        {            
            truck->update(window->getSize());            

            // TODO: Collision class
            if (frog->getShape().getGlobalBounds().intersects(truck->getShape().getGlobalBounds()) && dead == false)
            {
				dead = true;
				deadClock.restart();
                frog->reset();
            }            
			else if (frog->getShape().getPosition().y < 0 && win == false)
			{
				win = true;
				deadClock.restart();
				frog->reset();
			}
        }        
    }

    // draw phase
    void Game::draw()
    {
        window->clear();            

        truck->draw(*window);   

		if (dead == true)
		{									
			if (deadClock.getElapsedTime().asSeconds() <= DEAD_TIME) frog->splat(*window);
			else dead = false;							
		} 
		else if (win == true)
		{
			if (deadClock.getElapsedTime().asSeconds() <= DEAD_TIME) frog->win(*window);
			else win = false;
		} else frog->draw(*window);

        window->display();
    }

    // Game loop
    void Game::start()
    {
        while (window->isOpen())
        {   
            // Start frame
            time_point<steady_clock> startTime = steady_clock::now();

            Game::input();
            Game::update();
            Game::draw();

            // End Frame
            time_point<steady_clock> endTime = steady_clock::now();            
            auto elapsedTime = (endTime - startTime);
            FrameTime ft = duration_cast<duration<float, milli>>(elapsedTime).count();
            lastFrameTime = ft;
            
            float fps(1.0f / (ft / 1000.0f));
            // window->setTitle("FT: " + to_string(lastFrameTime) + " - FPS: " + to_string(fps));
        }
    }
}