//
//  breakout.c
//
//  Created by Idam Obiahu on 7/23/15.
//  Copyright (c) 2015 idamo.github. All rights reserved.
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library (SPL)
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);
    
    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    waitForClick();
  
    double x_velocity = 2.3;
    double y_velocity= 4.6;
    
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
       updateScoreboard(window, label, points);
       
       move(ball, x_velocity, -y_velocity);
       pause(10);
       
       GEvent event = getNextEvent(MOUSE_EVENT);
       
       if(event != NULL)
       {
            if(getEventType(event) == MOUSE_MOVED)
            {
                double x = getX(event) - getWidth(paddle)/2;
                double y = 500;
                setLocation(paddle, x, y);
            }
       }
       
       GObject object = detectCollision(window, ball);
       
       if(object != NULL)
       {
            if(object == paddle)
            {
                if(getX(ball) < getX(paddle)+40){
                y_velocity = -y_velocity;
                }
                else if(getX(ball) > getX(paddle)+40){
                y_velocity = -y_velocity;
                x_velocity = -x_velocity;
                }
                else{
                y_velocity = -y_velocity;
                x_velocity = 0;}
            }
            
            else if((strcmp(getType(object), "GRect")) == 0)
            {
                removeGWindow(window, object);
                y_velocity = -y_velocity;
                points++;            
            }
       }
       // bounce off right edge of window
        if (getX(ball) + getWidth(ball) >= getWidth(window))
        {
            x_velocity = -x_velocity;
        }

        // bounce off left edge of window
        else if (getX(ball) <= 0)
        {
            x_velocity = -x_velocity;
        }
        
        // bounce off top edge of window
        else if (getY(ball) <= 0)
        {
            y_velocity = -y_velocity;
        }
  
        
        if (getY(ball) >= HEIGHT-80)
        {
            lives--;
            setLocation(ball, (WIDTH/2), HEIGHT/2);
            setLocation(paddle, WIDTH/2-40, HEIGHT-80);
            if (lives > 0)
            waitForClick();
        }
        
    }
    setColor(label, "WHITE");
    label = newGLabel(" ");
    GLabel loser = newGLabel("GAME OVER");
    setFont(loser, "SansSerif-36");
    setColor(loser, "LIGHT_GRAY");
    add(window, loser);
    setLocation(loser, WIDTH/4, HEIGHT/2);
    
    // wait for click before exiting
    waitForClick();
   
    
    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    int x;//counter for the row of bricks
    int y;//counter for column of bricks
    GRect bricks;//define bricks
    
    /*Populate bricks*/
    for (x = 0; x <= WIDTH; x = x+20)
    {
        for (y = 15; y <= (HEIGHT/4); y = y+10)
        {
        /*This portion is the design of the bricks*/
            if ((x == 0 && y <= (HEIGHT/4)) || (x == WIDTH && y <= (HEIGHT/4)) || (y == 15 && x <= WIDTH) || (y == ((HEIGHT/4)-15) && x <= WIDTH))
            continue;
            else if ((x > (WIDTH/2)-20 && (y == HEIGHT/8-40)))
            continue;
            bricks = newGRect(x, y, (WIDTH/10), 10);
            setFilled(bricks, true);
            setColor(bricks, "BLACK");
            if ((x == (WIDTH-20) && y <= (HEIGHT/4)))
            setFillColor(bricks, "YELLOW");
            else if ((x < (WIDTH/2)-20 && (y == HEIGHT/8)))
            setFillColor(bricks, "ORANGE");
            else if ((x > (WIDTH/2)-20 && (y == HEIGHT/8+40)))
            setFillColor(bricks, "ORANGE");
            else if ((x >= 170 && x <= 210) && (y == 35))
            setFillColor(bricks, "GREEN");
            else
            setFillColor(bricks, "GRAY");
            setColor(bricks, "LIGHT_GRAY");
           
           /*Add the bricks to the window*/
            add(window, bricks);
        }
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    GOval ball = newGOval((WIDTH/2), HEIGHT/2, 10, 10);
    setColor(ball, "BLACK");
    setFilled(ball, true);
    add(window, ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    GRoundRect paddle = newGRoundRect(WIDTH/2-40, HEIGHT-80, 80, 10, 10);
    setColor(paddle, "DARK_GRAY");
    setFilled(paddle, true);
    add(window, paddle);
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel scoreboard = newGLabel("0");
    setFont(scoreboard, "SansSerif-36");
    setColor(scoreboard, "LIGHT_GRAY");
    add(window, scoreboard);
    setLocation(scoreboard, WIDTH/2, HEIGHT/2);
    return scoreboard;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    setLocation(label, WIDTH/2, HEIGHT/2);
    setFont(label, "SansSerif-36");
    setColor(label, "LIGHT_GRAY");
    add(window, label);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
