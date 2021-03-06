
#include "stdafx.h"
#include <Box2D\Particle\b2ParticleSystem.h>
#include <Box2D\Box2D.h>
class DebugDraw:public b2Draw
{
public:
  static bool RenderSimple;
  DebugDraw();
  ~DebugDraw();
  void DrawAABB(const b2AABB& aabb,bool isSolid);

  void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
  void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	/// DebugDraw::Draw a circle.
  void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
	/// DebugDraw::Draw a solid circle.
  void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
	/// DebugDraw::Draw a particle array
  void DrawParticles(const b2Vec2 *centers, float32 radius, const b2ParticleColor *colors, int32 count);
	/// DebugDraw::Draw a line segment.
  void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
	/// DebugDraw::Draw a transform. Choose your own length scale.
	/// @param xf a transform.
  void DrawTransform(const b2Transform& xf);
private:

};
const float d=1;
#define vdec(v) v.x/d,v.y/d

void setColor(b2ParticleColor c){
  glColor3f(c.r,
  c.g,
  c.b);
}
void drawRect(const b2Vec2 centers, const b2ParticleColor colors){
  glPushMatrix();
    setColor(colors);
    auto center=centers;
    //drawCircle(centers[i],center);
    glTranslatef(vdec(center),0);
    glutSolidCube(0.1);
  glPopMatrix();
}

void DebugDraw::DrawAABB(const b2AABB& aabb, bool solid){
  if (solid)glRectf(vdec(aabb.lowerBound), vdec(aabb.upperBound));
  else{
    auto& lx = aabb.lowerBound.x;
    auto& ly = aabb.lowerBound.y;
    auto& ux = aabb.upperBound.x;
    auto& uy = aabb.upperBound.y;
    float verts[] = { lx, uy, ux, uy, ux, ly, lx, ly };
    GLuint elem[] = { 0, 1, 2, 3};
    auto lloop = GL_LINE_LOOP;
    if(1){
      glBegin(GL_LINE_LOOP);
      glVertex2f(lx, uy);
      glVertex2f(ux, uy);
      glVertex2f(ux, ly);
      glVertex2f(lx, ly);
      glEnd();
    }
    else{
     glEnableClientState(GL_VERTEX_ARRAY);
     glVertexPointer(2, GL_FLOAT, 0, verts);
     glDrawElements(lloop, 1, GL_UNSIGNED_INT, elem);
     //glDrawArrays(lloop, 0, 4);
    }


  }
}
void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color){
  glBegin(GL_LINE_LOOP);
  //数で場合分けして上手く三角分割もできるが面倒だしいいや
  glColor3f( 1 , 0 , 0 );
  for (int i = 0; i < vertexCount; i++)
  {
    auto v = vertices[i];
    glVertex2f(vdec(v));
  }
  glEnd();
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color){
  DrawPolygon( vertices , vertexCount , color );
}

/// DebugDraw::Draw a circle.
void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {;
  const float32 k_segments  = 16.0f;
  const float32 k_increment = 2.0f * b2_pi / k_segments;
  float32 theta             = 0.0f;
  glColor3f( color.r , color.g , color.b );
  glBegin(GL_LINE_LOOP);
  for (int32 i = 0 ; i < k_segments ; ++i)
  {
    b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
    glVertex2f(v.x, v.y);
    theta += k_increment;
  }
  glEnd();
  //glu
}
/// DebugDraw::Draw a solid circle.
void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {;
  DrawCircle( center , radius , color );
}
/// DebugDraw::Draw a particle array
void DebugDraw::DrawParticles(const b2Vec2 *centers, float32 radius, const b2ParticleColor *colors, int32 count) {;
  for (int i = 0; i < count; i++)
  {
    auto c = colors[i];
    //マスク使って背景を波にすればちょっと幻想的な
    DrawCircle( centers[i] , radius , b2Color(c.r,c.g,c.b ));
  }
}
/// DebugDraw::Draw a line segment.
void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {;
}
/// DebugDraw::Draw a transform. Choose your own length scale.
/// @param xf a transform.
void DebugDraw::DrawTransform(const b2Transform& xf) {;
}

DebugDraw::DebugDraw()
{
}

DebugDraw::~DebugDraw()
{
}