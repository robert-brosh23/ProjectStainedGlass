
#include "raylib.h"
#define MAX_EDGES 5

// location parameter including coords, speed, and direction
struct location{
    double act_x;
    double act_y;
    int x;
    int y;
    int speed;
    float dir;
}typedef location;

struct line{
    Vector2 start;
    Vector2 end;
    int thickness;
    Color color;
}typedef line;

// The defining points of a body
struct body_coords{
    int num_points;
    Vector2 points[MAX_EDGES];
}typedef body_coords;

// Collision characteristics including edges and edge angles
struct collision_body{
    //Vector normal to edge
    int num_edges;
    float edge_angle[MAX_EDGES];
    Vector2 edge_start[MAX_EDGES];
    Vector2 edge_end[MAX_EDGES];
}typedef collision_body;

//Which collision body and which edge
struct collision_return{
    int body;
    int edge;
}typedef collision_return;

//Debugging functions
void print_text(char* text, float num_to_print, int x, int y, int font_size);

//Core functions
location update_location(location loc, collision_return col_ret, collision_body hitbox);
collision_return check_for_collision(location loc, body_coords* bodies, int num_bodies);
float convert_deg_to_rad(float deg);
bool faces_right(location loc);
bool faces_up(location loc);
body_coords create_rectangle(int x_upper, int x_lower, int y_upper, int y_lower);
collision_body assign_col_parameters(body_coords points);
float calculate_collision_dir(location loc, collision_body col, int collision_edge);
line rotate_user_line(line user_line, float user_dir, int user_line_length);

//Drawing functions
void draw_line_series(line* lines, int num_lines);
void draw_rectangles(body_coords* bodies, int num_bodies, int num_body_goal);
void draw_player(Vector2 current_loc);
void draw_game_state(int game_state, line* lines, int num_lines, body_coords* rectangles, int num_bodies, int num_body_goal);
