#pragma once
#include "GlfwWindow.h"

class Keymovement
{
public:

	enum RotationAxis {
		ROTATION_AXIS_X = 0,
		ROTATION_AXIS_Y = 1,
		ROTATION_AXIS_Z = 2
	};

	enum RotationInput {
		ROTATION_ADD_X = 1,
		ROTATION_SUB_X = -1,
		ROTATION_ADD_Y = 2,
		ROTATION_SUB_Y = -2,
		ROTATION_ADD_Z = 3,
		ROTATION_SUB_Z = -3
	};

	enum ObjectDirection {
		MOVE_FORWARD = 1,
		MOVE_BACKWARD = -1,
		MOVE_LEFT = 2,
		MOVE_RIGHT = -2,
		MOVE_UP = 3,
		MOVE_DOWN = -3
	};

    struct MoveKeyMappings {
		bool moveLeftInX = false;
		bool moveRightInX = false;
		bool moveLeftInY = false;
		bool moveRightInY = false;
		bool moveUpInZ = false;
		bool moveDownInZ = false;

        bool spin_x = false;
        bool spin_y = false;
        bool spin_z = false;
    };

public:
    MoveKeyMappings moves{};
};

