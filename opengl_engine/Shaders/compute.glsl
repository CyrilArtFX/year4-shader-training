#version 450 core

layout (local_size_x = 32) in;

uniform float closest_allowed_dist = 0.01f;
uniform float rule1_weight = 0.18f;
uniform float rule2_weight = 0.05;
uniform float rule3_weight = 0.0f;
uniform vec3 repulsion = vec3(0.0f);
uniform float timestep = 0.1f;


struct flock_member
{
    vec3 position;
    vec3 velocity;
};


layout (std430, binding = 0) readonly buffer members_in
{
    flock_member member[];
} input_data;

layout (std430, binding = 1) buffer members_out
{
    flock_member member[];
} output_data;

shared flock_member shared_member[gl_WorkGroupSize.x];


vec3 rule1(vec3 my_position, vec3 my_velocity, vec3 their_position, vec3 their_velocity)
{
    vec3 d = my_position - their_position;
    if (dot(d, d) < closest_allowed_dist)
        return d;
    return vec3(0.0);
}

vec3 rule2(vec3 my_position, vec3 my_velocity, vec3 their_position, vec3 their_velocity)
{
     vec3 d = their_position - my_position;
     vec3 dv = their_velocity - my_velocity;
     return dv / (dot(d, d) + 10.0);
}

vec3 rule3(vec3 repulsion, vec3 my_position)
{
    return normalize(repulsion - my_position);
}


void main()
{
	uint i, j;
    int global_id = int(gl_GlobalInvocationID.x);
    int local_id  = int(gl_LocalInvocationID.x);

    flock_member me = input_data.member[global_id];
    flock_member new_me;
    vec3 accelleration = vec3(0.0);

    for (i = 0; i < gl_NumWorkGroups.x; i++)
    {
        flock_member them = input_data.member[i * gl_WorkGroupSize.x + local_id];
        shared_member[local_id] = them;

        memoryBarrierShared();
        barrier();

        for (j = 0; j < gl_WorkGroupSize.x; j++)
        {
            them = shared_member[j];
            if (i * gl_WorkGroupSize.x + j != global_id)
            {
                accelleration += rule1(me.position, me.velocity, them.position, them.velocity) * rule1_weight;
                accelleration += rule2(me.position, me.velocity, them.position, them.velocity) * rule2_weight;
            }
        }

        barrier();
    }
    
    new_me.position = me.position + me.velocity * timestep;
    new_me.position = vec3(clamp(new_me.position.x, -1.0f, 1.0f),
                           clamp(new_me.position.y, -1.0f, 1.0f),
                           0.0f);

    accelleration += rule3(repulsion, me.position) * rule3_weight;
    
    new_me.velocity = me.velocity + accelleration * timestep;
    if (length(new_me.velocity) > 10.0) new_me.velocity = normalize(new_me.velocity) * 10.0;
    new_me.velocity = mix(me.velocity, new_me.velocity, 0.4);

    output_data.member[global_id] = new_me;
}