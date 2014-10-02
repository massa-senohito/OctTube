//void main(void)
//{
//  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
//}
uniform float Koge;
void main (void)
{
  if(Koge>0.9) Koge=0.9;
  gl_FragColor = vec4(1.0-Koge, 1.0-Koge, 1.0-Koge, 1.0);
}
