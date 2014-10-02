void main(void)
{
  vec4 vert=gl_Vertex;
  float x=vert.x/2;
  vert=vec4(x,vert.y,vert.z,vert.w);
  gl_Position = gl_ModelViewProjectionMatrix * vert;   
}
//glslhacker