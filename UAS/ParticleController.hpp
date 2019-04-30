#ifndef _PARTICLECONTROLLER_
#define _PARTICLECONTROLLER_

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "GlobalLib.h"

using namespace glm;

class ParticleController {
private:
	Particle particles_container[PARTICLE_MAX_NUMBER];
	GLuint vertices_vbo; // billboard_vertex_buffer
	GLuint properties_vbo; // particles_position_buffer
	GLuint color_vbo; // particles_color_buffer
	int last_used_particle = 0;
	int particles_count = 0;
	double lastTime;

public:
	ParticleController();

	void init();
	void destroy();
	void loadVertices();
	void loadProperties();
	void loadColor();

	void bind();
	void draw();
	void drawParticles();

	int findUnusedParticle();
	void sortParticles();
};


ParticleController::ParticleController(){
	init();
}

void ParticleController::init(){
	lastTime = glfwGetTime();
	
	for(int i=0; i<PARTICLE_MAX_NUMBER; i++){
		particles_container[i].life = -1.0f;
		particles_container[i].cameradistance = -1.0f;
	}
}

void ParticleController::destroy(){
	glDeleteBuffers(1, &color_vbo);
	glDeleteBuffers(1, &properties_vbo);
	glDeleteBuffers(1, &vertices_vbo);
}

// The VBO containing the 4 vertices of the particles.
// Thanks to instancing, they will be shared by all particles.
void ParticleController::loadVertices(){
	glGenBuffers(1, &vertices_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}

// The VBO containing the positions and size of the particles
void ParticleController::loadProperties(){
	glGenBuffers(1, &properties_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, properties_vbo);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, PARTICLE_MAX_NUMBER * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
}

// The VBO containing the colors of the particles
void ParticleController::loadColor(){
	glGenBuffers(1, &color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, PARTICLE_MAX_NUMBER * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
}

void ParticleController::bind(){
	// Update the buffers that OpenGL uses for rendering.
	// There are much more sophisticated means to stream data from the CPU to the GPU, 
	// but this is outside the scope of this tutorial.
	// http://www.opengl.org/wiki/Buffer_Object_Streaming

	glBindBuffer(GL_ARRAY_BUFFER, properties_vbo);
	glBufferData(GL_ARRAY_BUFFER, PARTICLE_MAX_NUMBER * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, particles_count * sizeof(GLfloat) * 4, g_particule_position_size_data);

	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glBufferData(GL_ARRAY_BUFFER, PARTICLE_MAX_NUMBER * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, particles_count * sizeof(GLubyte) * 4, g_particule_color_data);

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

void ParticleController::drawParticles(){
	loadVertices();
	loadProperties();
	loadColor();
	
	double currentTime = glfwGetTime();
	double delta = currentTime - lastTime;
	lastTime = currentTime;
	
	int newparticles = (int)(delta*10000.0);
	if (newparticles > (int)(0.016f*10000.0))
		newparticles = (int)(0.016f*10000.0);
	
	for(int i=0; i<newparticles; i++){
		int particle_index = findUnusedParticle();
		particles_container[particle_index].life = 5.0f; // This particle will live 5 seconds.
		particles_container[particle_index].pos = glm::vec3(0,0,-20.0f);

		float spread = 1.5f;
		glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f);
		// Very bad way to generate a random direction; 
		// See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
		// combined with some user-controlled parameters (main direction, spread, etc)
		glm::vec3 randomdir = glm::vec3(
			(rand()%2000 - 1000.0f)/1000.0f,
			(rand()%2000 - 1000.0f)/1000.0f,
			(rand()%2000 - 1000.0f)/1000.0f
		);
		
		particles_container[particle_index].speed = maindir + randomdir*spread;

		// Very bad way to generate a random color
		particles_container[particle_index].r = rand() % 256;
		particles_container[particle_index].g = rand() % 256;
		particles_container[particle_index].b = rand() % 256;
		particles_container[particle_index].a = (rand() % 256) / 3;

		particles_container[particle_index].size = (rand()%1000)/2000.0f + 0.1f;
	}

	// Simulate all particles
	particles_count = 0;
	for(int i=0; i<PARTICLE_MAX_NUMBER; i++){
		Particle& p = particles_container[i]; // shortcut
		if(p.life > 0.0f){
			// Decrease life
			p.life -= delta;
			if (p.life > 0.0f){
				// Simulate simple physics : gravity only, no collisions
				p.speed += glm::vec3(0.0f,-9.81f, 0.0f) * (float)delta * 0.5f;
				p.pos += p.speed * (float)delta;
				p.cameradistance = glm::length2( p.pos - glm::vec3(5, 2.5, 5) ); // vec3 is CameraPosition, will need to get it from CameraController
				//particles_container[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

				// Fill the GPU buffer
				g_particule_position_size_data[4*particles_count+0] = p.pos.x;
				g_particule_position_size_data[4*particles_count+1] = p.pos.y;
				g_particule_position_size_data[4*particles_count+2] = p.pos.z;
				g_particule_position_size_data[4*particles_count+3] = p.size;
												
				g_particule_color_data[4*particles_count+0] = p.r;
				g_particule_color_data[4*particles_count+1] = p.g;
				g_particule_color_data[4*particles_count+2] = p.b;
				g_particule_color_data[4*particles_count+3] = p.a;
			}else{
				// Particles that just died will be put at the end of the buffer in sortParticles();
				p.cameradistance = -1.0f;
			}
			particles_count++;
		}
	}
	sortParticles();

	bind();
	draw();
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

void ParticleController::sortParticles(){
	std::sort(&particles_container[0], &particles_container[PARTICLE_MAX_NUMBER]);
}

#endif