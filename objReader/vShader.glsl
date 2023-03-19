#version 410

layout (location=0) in vec3 pos;
layout (location=1) in vec2 texCoord;
out vec2 tc;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

mat4 buildRotX(float rad);
mat4 buildRotY(float rad);
mat4 buildRotZ(float rad);
mat4 buildTrans(float x,float y,float z);

void main(){
	gl_Position=proj_matrix*mv_matrix*vec4(pos,1.0);
	tc=texCoord;
}

mat4 buildRotX(float rad){
	mat4 rotx=mat4(1.0,0.0,0.0,0.0,
				   0.0,cos(rad),-sin(rad),0.0,
				   0.0,sin(rad),cos(rad),0.0,
				   0.0,0.0,0.0,1.0
				   );
	return rotx;
}
mat4 buildRotY(float rad){
	mat4 roty=mat4(cos(rad),0.0,sin(rad),0.0,
				   0.0,1.0,0.0,0.0,
				   -sin(rad),0.0,cos(rad),0.0,
				   0.0,0.0,0.0,1.0
				   );
	return roty;
}
mat4 buildRotZ(float rad){
	mat4 rotz=mat4(cos(rad),-sin(rad),0.0,0.0,
				   sin(rad),cos(rad),0.0,0.0,
				   0.0,0.0,1.0,0.0,
				   0.0,0.0,0.0,1.0
				   );
	return rotz;
}
mat4 buildTrans(float x,float y,float z){
	mat4 trans=mat4(1.0,0.0,0.0,0.0,
				   0.0,1.0,0.0,0.0,
				   0.0,0.0,1.0,0.0,
				   x,y,z,1.0);
	return trans;
}
