#pragma once

#define MAX_POINT_LIGHTS 3
#define MAX_SPOT_LIGHTS 2
#define TEXTURE_INDEX 0
#define SHADOW_BASE_INDEX 1
#define SHADOW_DIRECTIONAL_INDEX (SHADOW_BASE_INDEX + MAX_SPOT_LIGHTS)
#define SHADOW_CUBE_BASE_INDEX (SHADOW_DIRECTIONAL_INDEX + 1)
#define CAMERA_SPEED 15.0f
#define CAMERA_ROTATION 120.0f
#define CUBE_MAP_DIMENSIONS 1024
#define FOV 60.0f
#define NEAR_PLANE 0.1f
#define FAR_PLANE 1000.0f
