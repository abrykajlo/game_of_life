#version 150

attribute vec3 Position;

uniform mat4 Scale;
uniform mat4 Translate;
uniform mat4 WorldTranslate;

void main(void) {
    vec4 pos = vec4(Position, 1.0);
    gl_Position = Translate * Scale * WorldTranslate * pos;
}