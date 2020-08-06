#ifndef __TRAITEMENT_COMMAND__
#define __TRAITEMENT_COMMAND__

#include "Context.hpp"

/**
 * Analyze all opencv keyboard event and apply the command associated to the pressed key
 * Commands: -q : quit
 *           -o : display the current focused object
 *           -s : save the current output image
 *           -+ : increase the difference in pixel depth required to be part of the same object
 *           -p : increase the difference in pixel depth required to be part of the same object
 *           -- : decrease the difference in pixel depth required to be part of the same object
 *           -m : decrease the difference in pixel depth required to be part of the same object
 *
 * @param ctx the context to modify Context
 */
void keyboard_command(Context* ctx);

/**
 * @brief create a callback that must be called by opencv when the user use the mouse
 * @param event one of the cv::MouseEventTypes constants.
 * @param x the x position of the mouse click
 * @param y the y position of the mouse click
 * @param flags one of the cv::MouseEventFlags constants.
 * @param userdata The optional parameter a Context in our case
 */
void callBackMouse(int event, int x, int y, int flags, void* userdata);

#endif
