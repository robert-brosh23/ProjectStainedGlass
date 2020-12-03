
#include "raylib.h"
#define MAX_EDGES 5

struct location{
    int x;
    int y;
    int speed;
    float dir;
}typedef location;

// The defining points of a body
struct body_coords{
    Vector2 points[MAX_EDGES+1];
}typedef body_coords;

// 
struct collision_body{
    float edge_dir[MAX_EDGES];
    Vector2 edge_start[MAX_EDGES];
    Vector2 edge_end[MAX_EDGES];
}typedef collision_body;

location update_location(location loc);
bool check_for_collision(location loc);
float convert_deg_to_rad(float deg);
collision_body assign_col_parameters(body_coords points);
