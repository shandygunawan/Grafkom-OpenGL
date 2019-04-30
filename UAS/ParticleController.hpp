#ifndef _PARTICLECONTROLLER_
#define _PARTICLECONTROLLER_

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "GlobalLib.h"

using namespace std;

// CPU representation of a particle
struct Particle {
	glm::vec3 pos, speed;
	unsigned char r,g,b,a; // Color
	float size, angle, weight;
	float life; // Remaining life of the particle. if < 0 : dead and unused.
};

class ParticleController {
private:
	Particle particles_container[PARTICLE_MAX_NUMBER];
	GLuint vertices_vbo; // billboard_vertex_buffer
	GLuint properties_vbo; // particles_position_buffer
	GLuint color_vbo; // particles_color_buffer
	int last_used_particle = 0;
	int particles_count = 0;

public:
	ParticleController(GLfloat *particle_buffer_data); // g_vertex_buffer_data

	void loadVertices();
	void loadProperties();
	void loadColor();
	void bind();

	void draw();

	int findUnusedParticle();
}


ParticleController::ParticleController(GLfloat *particle_buffer_data){
	loadVertices();
	loadProperties();
	loadColor();

	bind();
}


// The VBO containing the 4 vertices of the particles.
// Thanks to instancing, they will be shared by all particles.
void ParticleController::loadVertices(){
	glGenBuffers(1, &vertices_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_buffer_data), particle_buffer_data, GL_STATIC_DRAW);
}

// The VBO containing the positions and size of the particles
void ParticleController::loadProperties(){
	glGenBuffers(1, &properties_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, properties_vbo);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, PARTICLE_MAX_NUMBER * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, properties_vbo);
	glBufferData(GL_ARRAY_BUFFER, PARTICLE_MAX_NUMBER * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, particles_count * sizeof(GLfloat) * 4, g_particule_position_size_data);

}

// The VBO containing the colors of the particles
void ParticleController::loadColor(){
	glGenBuffers(1, &color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, PARTICLE_MAX_NUMBER * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glBufferData(GL_ARRAY_BUFFER, PARTICLE_MAX_NUMBER * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, particles_count * sizeof(GLubyte) * 4, g_particule_color_data);
}

void ParticleController::bind(){
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(LOCATION_PARTICLE_VERTICES);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_vbo);
	glVertexAttribPointer(
	 LOCATION_PARTICLE_VERTICES,
	 3, // size
	 GL_FLOAT, // type
	 GL_FALSE, // normalized?
	 0, // stride
	 (void*)0 // array buffer offset
	);

	// 2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(LOCATION_PARTICLE_PROPERTIES);
	glBindBuffer(GL_ARRAY_BUFFER, properties_vbo);
	glVertexAttribPointer(
	 LOCATION_PARTICLE_PROPERTIES,
	 4, // size : x + y + z + size => 4
	 GL_FLOAT, // type
	 GL_FALSE, // normalized?
	 0, // stride
	 (void*)0 // array buffer offset
	);

	// 3rd attribute buffer : particles' colors
	glEnableVertexAttribArray(LOCATION_PARTICLE_COLOR);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glVertexAttribPointer(
	 LOCATION_PARTICLE_COLOR,
	 4, // size : r + g + b + a => 4
	 GL_UNSIGNED_BYTE, // type
	 GL_TRUE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
	 0, // stride
	 (void*)0 // array buffer offset
	);
}

void ParticleController::draw(){
	// These functions are specific to glDrawArrays*Instanced*.
	// The first parameter is the attribute buffer we're talking about.
	// The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
	glVertexAttribDivisor(LOCATION_PARTICLE_VERTICES, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(LOCATION_PARTICLE_PROPERTIES, 1); // positions : one per quad (its center) -> 1
	glVertexAttribDivisor(LOCATION_PARTICLE_COLOR, 1); // color : one per quad -> 1

	// Draw the particules !
	// This draws many times a small triangle_strip (which looks like a quad).
	// This is equivalent to :
	// for(i in particles_count) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4),
	// but faster.
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particles_count);
}

// Finds a Particle in particles_container which isn't used yet.
// (i.e. life < 0)
int ParticleController::findUnusedParticle(){
	for(int i=last_used_particle; i<PARTICLE_MAX_NUMBER; i++){
        if (particles_container[i].life < 0){
            last_used_particle = i;
            return i;
        }
    }

    for(int i=0; i<last_used_particle; i++){
        if (particles_container[i].life < 0){
            last_used_particle = i;
            return i;
        }
    }

    return 0; // All particles are taken, override the first one
}

#endif