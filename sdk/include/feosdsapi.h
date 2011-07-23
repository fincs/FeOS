//
// FeOS Standard Library
// feosdsapi.h
//     FeOS DS-specific functions
//

#pragma once

typedef struct { int x, y; } styluspos_t;
typedef word_t keys_t;

enum
{
	KEY_A      = BIT(0),  // A button
	KEY_B      = BIT(1),  // B button
	KEY_SELECT = BIT(2),  // SELECT button
	KEY_START  = BIT(3),  // START button
	KEY_RIGHT  = BIT(4),  // RIGHT button
	KEY_LEFT   = BIT(5),  // LEFT button
	KEY_UP     = BIT(6),  // UP button
	KEY_DOWN   = BIT(7),  // DOWN button
	KEY_R      = BIT(8),  // R button
	KEY_L      = BIT(9),  // L button
	KEY_X      = BIT(10), // X button
	KEY_Y      = BIT(11), // Y button
	KEY_TOUCH  = BIT(12)  // Touchscreen pen down
};

keys_t FeOS_GetKeysDown();
keys_t FeOS_GetKeysHeld();
keys_t FeOS_GetKeysUp();
void FeOS_GetStylusPos(styluspos_t*);
