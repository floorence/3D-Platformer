#include"FBO.h"
#include"util/Log.h"

FBO::FBO() {
    glGenFramebuffers(1, &ID);
}

void FBO::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void FBO::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::bindAndClear() {
    glBindFramebuffer(GL_FRAMEBUFFER, ID);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FBO::unbindAndClear() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FBO::attachTextureCube(GLuint textureID) {
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureID, 0);
}

void FBO::attachTexture2D(GLuint textureID) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
}

void FBO::attachRenderBuffer(GLuint rboID) {
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboID);
}

void FBO::checkStatus() {
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Log::err("FBO", "Depth framebuffer incomplete");
}

FBO::~FBO() {
    glDeleteFramebuffers(1, &ID);
}

FBO::FBO(FBO&& other) noexcept {
	ID = other.ID;
	other.ID = 0;
}

FBO& FBO::operator=(FBO&& other) noexcept {
	if (this != &other) {
		glDeleteBuffers(1, &ID);
		ID = other.ID;
		other.ID = 0;
	}
	return *this;
}
