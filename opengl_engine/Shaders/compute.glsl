#version 450 core

layout (local_size_x = 64) in; //  number of work item per work group

const float closest_allowed_dist = 0.05f;
const float max_velocity = 0.05f;

//  how each rule will affect the velocity of a blue square
const float rule1_weight = 0.05f;
const float rule2_weight = 0.3f;
const float rule3_weight = 0.04f;

const float timestep = 0.1f;

uniform vec3 repulsion = vec3(0.0f); //  position of the red square passed by game.cpp

const float repulsion_range = 0.2f;


struct flock_member
{
    vec3 position;
    vec3 velocity;
};


//  input datas (will be read)
layout (std430, binding = 0) readonly buffer members_in
{
    flock_member member[];
} input_data;

//  output datas (will be written on)
layout (std430, binding = 1) buffer members_out
{
    flock_member member[];
} output_data;

//  array to store position and velocity of others blue squares
shared flock_member shared_member[gl_WorkGroupSize.x];


//  this rule makes squares to repel each others
vec3 rule1(vec3 my_position, vec3 my_velocity, vec3 their_position, vec3 their_velocity)
{
    vec3 d = my_position - their_position;
    float l = length(d);
    if (l < closest_allowed_dist) return normalize(d);
    return vec3(0.0f);
}

//  this rule makes squares run away from the red square
vec3 rule2(vec3 repulsion, vec3 my_position)
{
    vec3 d = my_position - repulsion;
    if(length(d) < repulsion_range) return normalize(d);
    return vec3(0.0f);
}

//  this rule makes squares stay away from the border (not very clean code I know)
vec3 rule3(vec3 my_position)
{
    vec3 a = vec3(0.0f);
    if(my_position.x > 1.0f - closest_allowed_dist) a.x = -1.0f;
    else if(my_position.x < -1.0f + closest_allowed_dist) a.x = 1.0f;
    if(my_position.y > 1.0f - closest_allowed_dist) a.y = -1.0f;
    else if(my_position.y < -1.0f + closest_allowed_dist) a.y = 1.0f;
    return a;
}


void main()
{
    //  get the "position" of this square in the groups
    int global_id = int(gl_GlobalInvocationID.x);
    int local_id  = int(gl_LocalInvocationID.x);

    flock_member me = input_data.member[global_id]; //  input datas that will be read
    flock_member new_me; //  create datas that will be written on and outputed

    vec3 acceleration = vec3(0.0f); //  acceleration that will be computed to update velocity

    for (int i = 0; i < gl_NumWorkGroups.x; i++)
    {
        //  retrieve the other blue squares
        flock_member them = input_data.member[i * gl_WorkGroupSize.x + local_id];
        shared_member[local_id] = them;

        memoryBarrierShared();
        barrier();

        //  compute rule 1 according to the other blue squares
        for (int j = 0; j < gl_WorkGroupSize.x; j++)
        {
            them = shared_member[j];
            if (i * gl_WorkGroupSize.x + j != global_id)
            {
                acceleration += rule1(me.position, me.velocity, them.position, them.velocity) * rule1_weight;
            }
        }

        barrier();
    }

    //  compute rules 2 and 3
    acceleration += rule2(repulsion, me.position) * rule2_weight;
    acceleration += rule3(me.position) * rule3_weight;
    
    //  update position 
    new_me.position = me.position + me.velocity * timestep;

    //  clamp position
    if(new_me.position.x < -1.0f || new_me.position.x > 1.0f) new_me.position.x = me.position.x;
    if(new_me.position.y < -1.0f || new_me.position.y > 1.0f) new_me.position.y = me.position.y;
    new_me.position.z = 0.0f;
    
    // update velocity
    new_me.velocity = me.velocity + acceleration * timestep;
    if (length(new_me.velocity) > max_velocity) new_me.velocity = normalize(new_me.velocity) * max_velocity;
    new_me.velocity = mix(me.velocity, new_me.velocity, 0.4f);

    //  output written datas
    output_data.member[global_id] = new_me; 
}